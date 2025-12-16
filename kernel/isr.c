#include "isr.h"
#include "pic.h"
#include "keyboard.h"
#include "pit.h"
#include "sched.h"
#include "vga.h"

uint32_t isr_handler(regs_t* r) {
    (void)r;
    vga_println("CPU exception!");
    __asm__ volatile ("cli; hlt");
    return (uint32_t)r; // never reached, but keeps signature consistent
}

uint32_t irq_handler(regs_t* r) {
    int irq = (int)r->int_no - 32;

    if (irq == 0) {
        pit_irq_handler();
        pic_send_eoi(irq);          // EOI BEFORE switching away
        return sched_on_tick(r);    // may return different ESP
    }

    if (irq == 1) {
        keyboard_irq_handler();
        pic_send_eoi(irq);
        return (uint32_t)r;
    }

    pic_send_eoi(irq);
    return (uint32_t)r;
}
