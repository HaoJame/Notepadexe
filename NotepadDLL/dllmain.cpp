// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
WNDPROC  dwOldWndProc = 0;
//extern "C" void __declspec(dllexport) SpawnCalc(void)
//{
//    WinExec("calc.exe", 0);
//}

extern "C" 
{
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case 0x0111:
            if(wParam == 2)
                WinExec("C:\\Windows\\System32\\calc.exe", NULL);
            break;
        default:
            return CallWindowProc((WNDPROC)dwOldWndProc, hwnd, uMsg, wParam, lParam);
            break;
        };
        return 0;
    }
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    DWORD dwPid = 0;
    GetWindowThreadProcessId(hwnd, &dwPid);
    if (dwPid == GetCurrentProcessId())
    {
        *((HWND*)lParam) = hwnd;
        return FALSE;
    };
    return TRUE;
};
HWND ThisHwnd()
{
    HWND hWnd = 0;
    EnumWindows(EnumWindowsProc, (LPARAM)&hWnd);
    return hWnd;
};
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    HWND hwndThis = 0;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hwndThis = ThisHwnd();
        dwOldWndProc =(WNDPROC) SetWindowLong(hwndThis, GWLP_WNDPROC, (LONG)WindowProc);
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}