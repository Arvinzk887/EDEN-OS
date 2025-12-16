#include "vga.h"

void kernel_main(void) {
    vga_clear();
    vga_println("EDEN OS - Genesis Phase (Multiboot)");
    vga_println("-----------------------------------");
    vga_println("Console online.");
    vga_println("");
    vga_println("Next: keyboard input.");

    while (1) {}
}
