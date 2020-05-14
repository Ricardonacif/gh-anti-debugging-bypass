#include "Utils.h"

void bypassUnhandledExceptionMethod() {
    RFW_LDR_DATA_TABLE_ENTRY* mainModuleData = Utils::GetLDREntry("anti-debugging.exe");

    char* cName = Utils::TO_CHAR(mainModuleData->BaseDllName.Buffer);
    
    void* patternLocation = Utils::internalPatternScanner("\xC6\x05\x00\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\xCC\xA0\x00\x00\x00\x00\xC3", "xx?????xx????xx????x", (char*)mainModuleData->DllBase, mainModuleData->SizeOfImage);

    if (patternLocation == 0x00000000)
    {
        std::cout << "address not found! " << std::hex << patternLocation << "\n";
        return;
    }
    std::cout << "found address " << std::hex << patternLocation << "\n";
    unsigned char newBytecode[8] = "\x90\xB0\x00\x90\x90\x90\xC3";

   Utils::Patch((char*)patternLocation+13, (char*)&newBytecode ,7);  
}
