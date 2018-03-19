

def create(targetip, savefile):
    with open(".\\payloads\\remote.bin", "rb") as f:
        with open(savefile, "wb") as fp:
            for i in f:
                fp.write(i.replace(b"10.10.1.115", targetip.encode()))  # bug: 替换ip地址后文件损坏

    print("[*] 文件构建在%s" % savefile)


def repair():
    with open(".\\payloads\\remote.exe", "rb") as f:
        with open(".\\payloads\\remote.bin", "wb") as fp:
            for i in f:
                fp.write(i)

    print("[*] 修复完成")
