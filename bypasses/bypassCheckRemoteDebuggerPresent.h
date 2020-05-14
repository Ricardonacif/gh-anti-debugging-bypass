#include "pch.h"
#include <iostream>
#include <windows.h>

#include "ntdll.h"
#include "Utils.h"


const LPVOID __declspec(naked) GetGateAddress() {
    __asm {
        mov eax, dword ptr fs:[0xC0];
        ret;
    }
}

LPVOID originalGateCodeBackup = nullptr;

LPVOID BackupOriginalGateCode( LPVOID gateAddress ) {
    originalGateCodeBackup = VirtualAlloc(nullptr, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(originalGateCodeBackup, (void*)gateAddress, 9);

    return originalGateCodeBackup;
}

DWORD Backup_Eax, Handle, ProcessInformationClass, ProcessInformationLength, *PProcessInformation;

void __declspec(naked) hk_zwQueryInformationProcess() {
    __asm {
        mov Backup_Eax, eax
        mov eax, [esp + 0x8]
        mov Handle, eax
        mov eax, [esp + 0xC]
        mov ProcessInformationClass, eax
        mov eax, [esp + 0x10]
        mov PProcessInformation, eax
        mov eax, [esp + 0x14]
        mov ProcessInformationLength, eax
        // mov eax, [esp + 0x18]
        // mov Old, eax
        mov eax, Backup_Eax
        pushad
    }

    printf("hk_zwQueryInformationProcess Handle: [%x] ProcessInformationClass: [0x%x]  PProcessInformation: [%d]  ProcessInformationLength: [0x%x]\n", Handle, ProcessInformationClass, *PProcessInformation, ProcessInformationLength);

    __asm {
        popad
        mov eax, 0xffffffff
        ret
        // int 3
        // jmp originalGateCodeBackup
    }

}

void __declspec(naked) hk_Wow64Trampoline() {

    __asm {
        cmp eax, 0x19 //id of zwQueryInformationProcess
        je hk_zwQueryInformationProcess;
        jmp originalGateCodeBackup;
    }
}

void bypassCheckRemoteDebuggerPresent() {
    LPVOID gateAddress  = GetGateAddress();
    std::cout << "gate Address: " << std::hex << gateAddress;

    std:: cout << "Original copy address: " << BackupOriginalGateCode(gateAddress);
    Utils::Hook((void*)gateAddress, (void*)hk_Wow64Trampoline, 7);
}
