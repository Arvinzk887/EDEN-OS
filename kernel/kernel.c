#include "vga.h"
#include "keyboard.h"

void kernel_main(void) {
    vga_clear();
    vga_println("EDEN OS - Genesis Phase (Multiboot)");
    vga_println("-----------------------------------");
    vga_println("Console online.");
    vga_println("Keyboard online.");
    vga_println("");
    vga_println("Type keys:");

    keyboard_init();

    while (1) {
        char c = keyboard_getchar();
        vga_putc(c);
    }
}
