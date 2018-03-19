#pragma once
#include <winsock2.h>
#include <vector>
#include <atlstr.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "Psapi.h"  
#include <Winuser.h>
#include <string>
#include <fstream>
#include <direct.h>
#include <Aclapi.h>
#pragma comment(lib,"Advapi32.lib")  
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Psapi.Lib")  
using namespace std;
const int KeyBoardValue = 0x80000000;
string GetKeyName(int);
bool JudgeShift();
