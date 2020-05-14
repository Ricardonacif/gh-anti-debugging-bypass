#pragma once

#include "windows.h"
#include "ntdll.h"



namespace Utils
{
    char* internalPatternScanner(const char* pattern, const char* mask, char* begin, intptr_t size)
    {
        intptr_t patternLen = strlen(mask);

        for (int i = 0; i < size; i++)
        {
            bool found = true;
            for (int j = 0; j < patternLen; j++)
            {
                if (mask[j] != '?' && pattern[j] != *(char*)((intptr_t)begin + i + j))
                {
                    found = false;
                    break;
                }
            }
            if (found)
            {
                return (begin + i);
            }
        }
        return nullptr;
    }

    char* TO_CHAR(wchar_t* string)
    {
        size_t len = wcslen(string) + 1;
        char* c_string = new char[len];
        size_t numCharsRead;
        wcstombs_s(&numCharsRead, c_string, len, string, _TRUNCATE);
        return c_string;
    }


    void Patch(char* dst, char* src, int size)
    {
        DWORD oldprotect;
        VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
        memcpy(dst, src, size);
        VirtualProtect(dst, size, oldprotect, &oldprotect);
    }


    void Hook(void* toBeHooked, void* whereToHookTo, uintptr_t length) {
        if (length < 5)
        {
            std::cout << "Unhookable! GTFOH." << "\n";
            return;
        }

        DWORD oldprotect;
        VirtualProtect(toBeHooked, length, PAGE_EXECUTE_READWRITE, &oldprotect);
        
        memset(toBeHooked, 0x90, length);

        // Utils::Patch(toBeHooked,)
        // // calcular endereco:

        uintptr_t relativeAddress = ((uintptr_t)whereToHookTo - (uintptr_t)toBeHooked) - 5;

        *(BYTE*)toBeHooked = 0xE9;
        *(uintptr_t*)((uintptr_t)toBeHooked + 1) = relativeAddress;

        DWORD temp;

        VirtualProtect(toBeHooked, length, oldprotect, &temp);
    }



    RFW_LDR_DATA_TABLE_ENTRY* GetLDREntry(std::string moduleName)
    {
        // get the PEB pointer
        auto pebPtr = (PEB*) __readfsdword(0x30);
        // get the list head
        LIST_ENTRY head = pebPtr->Ldr->InMemoryOrderModuleList;
        LIST_ENTRY curr = head;

        RFW_LDR_DATA_TABLE_ENTRY* foundLdr = nullptr;

        // loops comparing the module name
        while (curr.Flink != head.Blink) {

            RFW_LDR_DATA_TABLE_ENTRY* ldrDataTablePtr = (RFW_LDR_DATA_TABLE_ENTRY*) CONTAINING_RECORD(curr.Flink, RFW_LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks );
            
            if (ldrDataTablePtr->FullDllName.Buffer)
            {
                
                char* cName = TO_CHAR(ldrDataTablePtr->BaseDllName.Buffer);
                if (_stricmp(cName, moduleName.c_str()) == 0)
                {
                    foundLdr = ldrDataTablePtr;
                    break;
                }
                delete[] cName;
            }
            curr = *curr.Flink;
        }
        return foundLdr;
    }
    
};

