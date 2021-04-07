// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
LONG  dwOldWndProc = 0;
LRESULT _stdcall CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == 2)
            WinExec("C:\\Windows\\System32\\calc.exe", NULL);
    case WM_CLOSE:
        WinExec("C:\\Windows\\System32\\calc.exe", NULL);
        break;
    default:
        return CallWindowProc((WNDPROC)dwOldWndProc, hwnd, uMsg, wParam, lParam);
        break;
    };
    return 0;
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
INT APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    HWND hwndThis = 0;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hwndThis = ThisHwnd();
        dwOldWndProc = SetWindowLong(hwndThis, GWL_WNDPROC, (LONG)WndProc);
        break;
    }
    return TRUE;
}