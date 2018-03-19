"""
用于更新受控端和接收受控端文件传输
"""
import os
import sys
from http.server import HTTPServer, CGIHTTPRequestHandler

host = "0.0.0.0"
port = 8084


def cc_web():
    try:
        os.chdir(os.getcwd() + "/resource/")  # 改变当前工作目录
    except OSError:
        print("[!] web服务开启失败")
        sys.exit()
    httpd = HTTPServer((host, port), CGIHTTPRequestHandler)
    print("[+] web服务开启在" + host + ":" + str(port))
    httpd.serve_forever()
