void kernel_main(void) {
    const char* msg = "EDEN OS - Genesis Phase (Multiboot)\n";
    volatile unsigned char* vga = (unsigned char*)0xB8000;

    for (int i = 0; msg[i] != '\0'; i++) {
        vga[i * 2] = (unsigned char)msg[i];
        vga[i * 2 + 1] = 0x0F;
    }

    while (1) {}
}
