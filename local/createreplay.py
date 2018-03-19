

def create(targetip, savefile):
    with open(".\\payloads\\client.bin", "rb") as f:
        with open(savefile, "wb") as fp:
            for i in f:
                fp.write(i.replace(b"10.10.1.115", targetip.encode()))

    print("[*] 文件构建在%s" % savefile)


def repair():
    with open(".\\payloads\\client.exe", "rb") as f:
        with open(".\\payloads\\client.bin", "wb") as fp:
            for i in f:
                fp.write(i)

    print("[*] 修复完成")
