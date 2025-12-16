#include "uart.h"
#include "io.h"

#define COM1 0x3F8

static int tx_ready(void) { return inb(COM1 + 5) & 0x20; }
static int rx_ready(void) { return inb(COM1 + 5) & 0x01; }

void uart_init(void) {
    outb(COM1 + 1, 0x00); // Disable interrupts
    outb(COM1 + 3, 0x80); // Enable DLAB
    outb(COM1 + 0, 0x03); // Divisor low  (115200/3 = 38400 baud)
    outb(COM1 + 1, 0x00); // Divisor high
    outb(COM1 + 3, 0x03); // 8 bits, no parity, one stop bit (8N1)
    outb(COM1 + 2, 0xC7); // Enable FIFO, clear them, 14-byte threshold
    outb(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

void uart_putc(char c) {
    while (!tx_ready()) {}
    outb(COM1, (uint8_t)c);
}

void uart_print(const char* s) {
    while (*s) uart_putc(*s++);
}

int uart_can_read(void) {
    return rx_ready();
}

char uart_getc(void) {
    while (!rx_ready()) {}
    return (char)inb(COM1);
}
