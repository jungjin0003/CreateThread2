#include <stdio.h>
#include "CreateThread2.h"

int __stdcall test(int a, int b)
{
    printf("%d %d\n", a, b);
    Sleep(1000);
    return 101;
}

int main(int argc, char *argv[])
{
    HMODULE hModule = GetModuleHandleA("kernel32.dll");

    HANDLE hThread = CreateThread2(NULL, 0, (LPTHREAD_START_ROUTINE)test, 0, NULL, 2, 1, 2);
    WaitForSingleObject(hThread, INFINITE);
    printf("[*] Terminated!\n");
    DWORD ExitCode;
    GetExitCodeThread(hThread, &ExitCode);
    printf("[*] Exit Code : %d\n", ExitCode);
}   