#include "power.h"
#include "io.h"
#include "vga.h"

__attribute__((noreturn))
void poweroff(void) {
    vga_println("poweroff(): attempting QEMU isa-debug-exit...");

    // QEMU isa-debug-exit device (your Makefile uses iobase=0xF4, iosize=4)
    outl(0xF4, 0x10);

    // Fallbacks (some setups respond to these instead)
    outw(0x604, 0x2000);   // QEMU/ACPI “power off”
    outw(0xB004, 0x2000);  // Bochs/QEMU alternative

    vga_println("poweroff(): exit did not trigger; halting CPU.");

    __asm__ volatile ("cli; hlt");
    for (;;) { __asm__ volatile ("hlt"); }
}
