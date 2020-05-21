extern "C" void bypass_peb_64( );

void setPEB64HeavensGateDebuggerBypass() {
    bypass_peb_64();
}