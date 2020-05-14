void setPEBDebuggerBypass() {
    auto peb = (char*) __readfsdword(0x30);
    *(peb+0x2) = 0x0;
}