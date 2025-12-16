#include "power.h"
#include "io.h"

void poweroff(void) {
    // QEMU isa-debug-exit device: write to 0xF4 to exit
    outb(0xF4, 0x00);
    __asm__ volatile ("cli; hlt");
}
