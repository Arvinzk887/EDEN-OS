#include "isr.h"
#include "pic.h"
#include "keyboard.h"
#include "vga.h"

void isr_handler(regs_t* r) {
    (void)r;
    vga_println("CPU exception!");
    __asm__ volatile ("cli; hlt");
}

void irq_handler(regs_t* r) {
    int irq = (int)r->int_no - 32;

    if (irq == 1) {
        keyboard_irq_handler();  // read scancode & buffer it
    }

    pic_send_eoi(irq);          // send EOI ONCE, at the end
}
