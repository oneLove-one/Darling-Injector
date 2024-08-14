#include <Windows.h>
#include <iostream>
#include <string>

#include <commdlg.h>

#pragma comment(lib, "comdlg32.lib")

const char* TEXT_PREFIX_INFO = "[+] -> ";
const char* TEXT_PREFIX_SUCCES = "[SUCCES] -> ";
const char* TEXT_PREFIX_BAD = "[ERROR] -> ";

namespace DynamicVars {

	std::string WindowName = "";
	unsigned short MenuItem = 0;

	HWND hWindow = NULL;
	DWORD pIdProcess = NULL;
}