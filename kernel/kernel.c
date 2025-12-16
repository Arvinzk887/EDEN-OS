#include "vga.h"
#include "keyboard.h"
#include "shell.h"

void kernel_main(void) {
    vga_clear();
    vga_println("EDEN OS - Genesis Phase (Multiboot)");
    vga_println("-----------------------------------");
    vga_println("Console online.");
    vga_println("Keyboard online.");

    keyboard_init();
    shell_run();

    while (1) {}
}
