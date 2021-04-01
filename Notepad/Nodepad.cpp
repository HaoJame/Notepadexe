#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <tlhelp32.h>
#include <string.h>

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
    if (targetProcess)
    {
        HMODULE dll = LoadLibrary(TEXT("NotepadDLL.dll"));
        if (dll != NULL) {
            std::wcout << "The DLL found\n";
        }
        HOOKPROC addr = (HOOKPROC)GetProcAddress(dll, "dwOldWndProc");
        if (addr != NULL) {
            std::wcout << "The function found\n";
        }
        HHOOK handle = SetWindowsHookEx(WH_KEYBOARD, addr, dll, 0);
        if (handle != NULL) {
            std::wcout << "The function found\n";
        }
        std::wcout <<"Program successfully hooked.\nPress enter to unhook the function and stop the program\n";
        UnhookWindowsHookEx(handle);
    }
    return FALSE;
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
    if (Inject(pid, ppathDLL)==TRUE)
    {
        std::cout << "Injection successful" << std::endl;
    }
    /*
    * Load File DLL test
    */
    //HMODULE hLibrary = LoadLibrary(TEXT("Hook_Calc.dll"));
    //HOOKPROC hprc = (HOOKPROC)GetProcAddress(hLibrary, "SpawnCalc");
    //if (hLibrary == NULL)
    //{
    //    printf("[-] LoadLibraryA has failed: %d\n", GetLastError());
    //    return 1;
    //}
    //printf("[+] Handle of the loaded DLL: 0x%p\n", hLibrary);
    return 0;
}

