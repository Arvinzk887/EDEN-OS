#include "pic.h"
#include "io.h"
#include <stdint.h>

#define PIC1         0x20
#define PIC2         0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2+1)

#define ICW1_INIT    0x10
#define ICW1_ICW4    0x01
#define ICW4_8086    0x01

static void io_wait(void) {
    outb(0x80, 0);
}

void pic_remap(void) {
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4); io_wait();

    outb(PIC1_DATA, 0x20); io_wait(); // Master offset 32
    outb(PIC2_DATA, 0x28); io_wait(); // Slave offset 40

    outb(PIC1_DATA, 4);    io_wait(); // tell Master about Slave at IRQ2
    outb(PIC2_DATA, 2);    io_wait(); // tell Slave its cascade identity

    outb(PIC1_DATA, ICW4_8086); io_wait();
    outb(PIC2_DATA, ICW4_8086); io_wait();

}

void pic_send_eoi(int irq) {
    if (irq >= 8) outb(PIC2_COMMAND, 0x20);
    outb(PIC1_COMMAND, 0x20);
}

void pic_set_masks(unsigned char master_mask, unsigned char slave_mask) {
    outb(0x21, master_mask); // PIC1_DATA
    outb(0xA1, slave_mask);  // PIC2_DATA
}
