#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <tlhelp32.h>
#include <string.h>
typedef LRESULT(CALLBACK* Win)(HWND,UINT,WPARAM,LPARAM);//(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL IsProcessRunning(DWORD pid)
{
    HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
    DWORD ret = WaitForSingleObject(process, 0);
    CloseHandle(process);
    return ret == WAIT_TIMEOUT;
}

DWORD MyGetProcessId(LPCTSTR ProcessName) // non-conflicting function name
{
    PROCESSENTRY32 pt;
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pt.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hsnap, &pt)) { // must call this first
        do {
            if (!lstrcmpi(pt.szExeFile, ProcessName)) {
                CloseHandle(hsnap);
                return pt.th32ProcessID;
            }
        } while (Process32Next(hsnap, &pt));
    }
    CloseHandle(hsnap); // close handle on failure
    return 0;
}
BOOL WINAPI Inject(DWORD id, char* ppathDLL)
{
    HANDLE  targetProcess;
    std::wcout << "Process id: " << id << "\n";
    targetProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, id);
    BOOL success = FALSE;
    if (targetProcess)
    {
        HINSTANCE  dll = LoadLibrary(TEXT("NotepadDLL.dll"));
        if (dll != NULL) {
            std::wcout << "The DLL found\n";
        }
        HOOKPROC addr = (HOOKPROC)GetProcAddress(dll, "");
        //Win addr = (Win)GetProcAddress(dll, "LoadLibraryA");
        if (addr != NULL) {
            std::wcout << "The function found\n";
        }
        //HMODULE appInstance = GetModuleHandle(L"NotepadDLL.dll");
        //DWORD threadid = GetWindowThreadProcessId((HWND)targetProcess, NULL);
        HHOOK handle = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)addr, dll, 0);
        if (handle != NULL) {
            std::wcout << "The function found\n";
        }
        std::wcout << "Program successfully hooked.\nPress enter to unhook the function and stop the program\n";
        std::wcout << "hooked to threadid-->%lu:" << handle << "\n";
        getchar();
        UnhookWindowsHookEx(handle);
        return success = TRUE;
    }
    else
    {
        success = FALSE;
    }
    return success;
}
int _tmain(int argc, _TCHAR* argv[])
{
    //DWORD attach;
    DWORD pid = MyGetProcessId(TEXT("Notepad.exe"));
    std::cout << "Find pid: " << pid << "\n";
    //std::cin >> attach;
    //PWSTR pszLibFileRemote = NULL;
    char pathDLL[] = "..\\x64\\Debug\\NotepadDLL.dll";
    char* ppathDLL = pathDLL;
    std::cout << "waiting ..." << "\n";
    if (Inject(pid, ppathDLL) == TRUE)
    {
        std::cout << "Injection successful" << std::endl;
    }
    return 0;
}

