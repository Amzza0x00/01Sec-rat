import platform


def os_check():
    osx = platform.system()
    if osx.lower() == "windows":
        return "w"
    else:
        return "l"
