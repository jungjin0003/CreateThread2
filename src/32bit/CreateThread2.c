#include "CreateThread2.h"

__declspec(naked) VOID Helper(PDynamic_Parameter DynamicParameter)
{
    __asm__ __volatile__ (
        "mov ecx, %[free]\n\t"
        "xor edx, edx\n\t"
        "mov dl, 0x4\n\t"
        "mov eax, dword ptr ss:[esp+4]\n\t"
        "mov eax, dword ptr ds:[eax+4]\n\t"
        "mul edx\n\t"
        "mov esi, eax\n\t"
        "mov edi, dword ptr ss:[esp]\n\t"
        "xor eax, eax\n\t"
        "mov ebx, dword ptr ss:[esp+4]\n\t"
        "set:\n\t"
        "sub esi, 0x4\n\t"
        "push dword ptr ds:[ebx+esi+8]\n\t"
        "cmp esi, 0\n\t"
        "jne set\n\t"
        "push edi\n\t"
        "mov edi, dword ptr ss:[ebp-8]\n\t"
        "mov edi, dword ptr ss:[edi]\n\t"
        "push ebx\n\t"
        "call ecx\n\t"
        "add esp, 0x4\n\t"
        "jmp dword ptr ds:[edi]\n\t"
        :
        : [free] "r" (free)
    );
}

/*void BaseThreadInitThunkHook()
{
    HMODULE hModule = GetModuleHandleA("kernel32.dll");
    PVOID BaseThreadInitThunk = GetProcAddress(hModule, "BaseThreadInitThunk");
    DWORD OldProtect;

    BYTE Hook[] = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0xEB, 0xF9 };

    ULONG Offset = (ULONG)Helper - (ULONG)BaseThreadInitThunk - 2;

    memcpy(Hook + 1, &Offset, 4);

    if (VirtualProtect((ULONG)BaseThreadInitThunk - 5, 7, PAGE_EXECUTE_READWRITE, &OldProtect) != TRUE)
    {
        printf("[-] VirtualProtect Failed!\n");
        return -1;
    }

    memcpy((ULONG)BaseThreadInitThunk - 5, Hook, sizeof(Hook));

    if (VirtualProtect((ULONG)BaseThreadInitThunk - 5, 7, OldProtect, &OldProtect) != TRUE)
    {
        printf("[-] VirtualProtect Failed!\n");
        return -1;
    }
}*/

HANDLE CustomWINAPI CreateThread2(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, DWORD dwCreationFlags, LPDWORD lpThreadId, SIZE_T dwParameterCount, ...)
{
    Dynamic_Parameter *DynamicParameter = malloc(8 + dwParameterCount * 4);

    DynamicParameter->Function = lpStartAddress;
    DynamicParameter->Count = dwParameterCount;

    memcpy((ULONG)DynamicParameter + 8, (ULONG)&dwParameterCount + 4, dwParameterCount * 4);

    return CreateThread(lpThreadAttributes, dwStackSize, (LPTHREAD_START_ROUTINE)Helper, DynamicParameter, dwCreationFlags, lpThreadId);
}