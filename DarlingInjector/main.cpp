#include "includes.hpp"

int main() {

	std::cout << TEXT_PREFIX_INFO << "Hi user, please enter the name of the application window: ";
	std::cin >> DynamicVars::WindowName;

	if (DynamicVars::WindowName.length() == 0) {
		std::cout << TEXT_PREFIX_BAD << "You have not entered anything." << std::endl;
		Sleep(5000);
		return 0;
	}

	DynamicVars::hWindow = FindWindowA(NULL, DynamicVars::WindowName.c_str());

	if (DynamicVars::hWindow == NULL) {
		std::cout << TEXT_PREFIX_BAD << "Sorry, but I couldn't find the application window, maybe the window title is wrong." << std::endl;
		Sleep(5000);
		return 0;
	}

	DynamicVars::pIdProcess = GetProcessId(DynamicVars::hWindow);

	if (DynamicVars::pIdProcess == NULL) {
		std::cout << TEXT_PREFIX_BAD << "Sorry, but it didn’t work out to get the process id." << std::endl;
		Sleep(5000);
		return 0;
	}

	return 0;
}