#include "pch.h"
#include <iostream>

#include "Utils.h"
#include <array>

void bypassCreateToolhelp32Snapshot() {

    std::array<std::string, 22> debuggerNames = {  "ollydbg.exe", "ida.exe", "ida64.exe", "idag.exe", "idag64.exe", "idaw.exe", "idaw64.exe", "idaq.exe", "idaq64.exe", "idau.exe", "idau64.exe", "scylla.exe", "scylla_x64.exe", "scylla_x86.exe", "protection_id.exe", "x64dbg.exe", "x32dbg.exe", "windbg.exe", "reshacker.exe", "ImportREC.exe", "IMMUNITYDEBUGGER.EXE", "devenv.exe"}; 

    RFW_LDR_DATA_TABLE_ENTRY* mainModuleData = Utils::GetLDREntry("anti-debugging.exe");
    
    for(const auto& name: debuggerNames) {

        std::cout << name << ' ';
        int stringSize = name.size();
        char *mask = (char*)malloc(stringSize + 1);
        memset((void*)mask, 'x', stringSize);
        mask[stringSize] = '\0';
        
        void* patternLocation = Utils::internalPatternScanner(name.c_str(),(const char*) mask, (char*)mainModuleData->DllBase, mainModuleData->SizeOfImage);
        
        if (patternLocation == 0x00000000)
        {
            std::cout << "address not found aki! " << std::hex << patternLocation << "\n";      
            return;
        }
        std::cout << "Found!! " << std::hex << patternLocation << "\n";
        char replacement[12] = "UWONTFINDME";

        Utils::Patch((char*)patternLocation, (char*)replacement ,stringSize);  
    }

}