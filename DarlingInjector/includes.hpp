#include <Windows.h>
#include <iostream>

const char* TEXT_PREFIX_INFO = "[+] -> ";
const char* TEXT_PREFIX_SUCCES = "[SUCCES] -> ";
const char* TEXT_PREFIX_BAD = "[ERROR] -> ";

namespace DynamicVars {

	std::string WindowName = "";

	HWND hWindow = NULL;
	DWORD pIdProcess = NULL;
}