#include "isr.h"
#include "pic.h"
#include "keyboard.h"
#include "vga.h"
#include "pit.h"

void isr_handler(regs_t* r) {
    (void)r;
    vga_println("CPU exception!");
    __asm__ volatile ("cli; hlt");
}

void irq_handler(regs_t* r) {
    int irq = (int)r->int_no - 32;

    if (irq == 0) {
        pit_irq_handler();
    } else if (irq == 1) {
        keyboard_irq_handler();
    }

    pic_send_eoi(irq);
}