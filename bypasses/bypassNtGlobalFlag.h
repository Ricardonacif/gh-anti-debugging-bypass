void bypassNtGlobalFlag() {
    auto peb = (char*) __readfsdword(0x30);
    *(peb + 0x68) = (*(peb + 0x68) &= ~(1UL << 5));
}