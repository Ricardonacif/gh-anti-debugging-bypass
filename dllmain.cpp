// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <windows.h>

#include "ntdll.h"
#include "bypasses/setPEBDebuggerBypass.h"
#include "bypasses/bypassNtGlobalFlag.h";
#include "bypasses/bypassCreateToolhelp32Snapshot.h";
#include "bypasses/bypassCheckRemoteDebuggerPresent.h";
#include "bypasses/bypassUnhandledExceptionMethod.h";
#include "bypasses/setPEB64HeavensGateDebuggerBypass.h";

DWORD WINAPI AntiAntiDebugThread(HMODULE hModule)
{
    //Create Console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    setPEBDebuggerBypass();
    bypassNtGlobalFlag();
    bypassCreateToolhelp32Snapshot();
    bypassCheckRemoteDebuggerPresent();
    bypassUnhandledExceptionMethod();
    setPEB64HeavensGateDebuggerBypass(); //TO BE DONE
    
    return 0;
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)AntiAntiDebugThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

