#include "includes.hpp"

std::string OpenFileDialog() {
    OPENFILENAMEA ofn;
    char szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "DLL Files\0*.dll\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn) == TRUE) {
        return ofn.lpstrFile;
    }
    return "";
}

int main() {
    std::cout << "\n1. LoadLibraryA \t-> Work.\n2. Manual Map \t\t-> Soon...\n3. Thread Hjacking \t-> Soon...\n" << std::endl;

    std::cout << TEXT_PREFIX_INFO << "Please select injection method: ";
    std::cin >> DynamicVars::MenuItem;

    if (DynamicVars::MenuItem != 1) {
        std::cout << TEXT_PREFIX_BAD << "Sorry, but so far only LoadLibraryA has been developed." << std::endl;
        Sleep(5000);
        system("cls");
        return main();
    }

    std::cout << TEXT_PREFIX_INFO << "Hi user, please enter the name of the application window: ";
    std::cin.ignore();
    std::getline(std::cin, DynamicVars::WindowName);

    if (DynamicVars::WindowName.empty()) {
        std::cout << TEXT_PREFIX_BAD << "You have not entered anything." << std::endl;
        Sleep(5000);
        return 0;
    }

    while (true) {
        DynamicVars::hWindow = FindWindowA(NULL, DynamicVars::WindowName.c_str());
        if (DynamicVars::hWindow == NULL) {
            DWORD error = GetLastError();
            std::cout << TEXT_PREFIX_BAD << "Sorry, but I couldn't find the application window. Error code: " << error << std::endl;
            Sleep(5000);
            continue;
        }

        std::cout << TEXT_PREFIX_INFO << "Window handle found: " << DynamicVars::hWindow << std::endl;

        DWORD processId;
        GetWindowThreadProcessId(DynamicVars::hWindow, &processId);
        DynamicVars::pIdProcess = processId;

        if (DynamicVars::pIdProcess == 0) {
            DWORD error = GetLastError();
            std::cout << TEXT_PREFIX_BAD << "Sorry, but it didn't work out to get the process id. Error code: " << error << std::endl;
            Sleep(5000);
            continue;
        }

        std::cout << TEXT_PREFIX_INFO << "Process ID: " << DynamicVars::pIdProcess << std::endl;

        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DynamicVars::pIdProcess);
        if (hProcess == NULL) {
            DWORD error = GetLastError();
            std::cout << TEXT_PREFIX_BAD << "Failed to open the target process. Error code: " << error << std::endl;
            Sleep(5000);
            continue;
        }

        std::cout << TEXT_PREFIX_INFO << "Please select the DLL file." << std::endl;
        std::string dllPath = OpenFileDialog();

        if (dllPath.empty()) {
            std::cout << TEXT_PREFIX_BAD << "No DLL file selected." << std::endl;
            CloseHandle(hProcess);
            Sleep(5000);
            continue;
        }

        LPVOID pDllPath = VirtualAllocEx(hProcess, NULL, dllPath.length() + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (pDllPath == NULL) {
            std::cout << TEXT_PREFIX_BAD << "Failed to allocate memory in the target process." << std::endl;
            CloseHandle(hProcess);
            Sleep(5000);
            continue;
        }

        if (!WriteProcessMemory(hProcess, pDllPath, dllPath.c_str(), dllPath.length() + 1, NULL)) {
            std::cout << TEXT_PREFIX_BAD << "Failed to write to process memory." << std::endl;
            VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            Sleep(5000);
            continue;
        }

        LPTHREAD_START_ROUTINE pLoadLibraryA = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
        if (pLoadLibraryA == NULL) {
            std::cout << TEXT_PREFIX_BAD << "Failed to get LoadLibraryA address." << std::endl;
            VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            Sleep(5000);
            continue;
        }

        HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, pLoadLibraryA, pDllPath, 0, NULL);
        if (hThread == NULL) {
            std::cout << TEXT_PREFIX_BAD << "Failed to create remote thread." << std::endl;
            VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            Sleep(5000);
            continue;
        }

        WaitForSingleObject(hThread, INFINITE);

        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hThread);
        CloseHandle(hProcess);

        std::cout << TEXT_PREFIX_SUCCES << "DLL successfully injected!" << std::endl;
        break;
    }

    Sleep(5000);
    return 0;
}
