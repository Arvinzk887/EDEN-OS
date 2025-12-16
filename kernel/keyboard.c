#include "keyboard.h"
#include "io.h"

static const char scancode_map[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/',0,
    '*',0,' '
};

#define KBD_BUF_SIZE 128
static volatile char kbd_buf[KBD_BUF_SIZE];
static volatile int kbd_head = 0;
static volatile int kbd_tail = 0;

static int buf_empty(void) { return kbd_head == kbd_tail; }
static int buf_full(void)  { return ((kbd_head + 1) % KBD_BUF_SIZE) == kbd_tail; }

static void buf_push(char c) {
    if (buf_full()) return;
    kbd_buf[kbd_head] = c;
    kbd_head = (kbd_head + 1) % KBD_BUF_SIZE;
}

static char buf_pop(void) {
    if (buf_empty()) return 0;
    char c = kbd_buf[kbd_tail];
    kbd_tail = (kbd_tail + 1) % KBD_BUF_SIZE;
    return c;
}

void keyboard_irq_handler(void) {
    // only read if controller says data is available
    if (!(inb(0x64) & 1)) return;

    uint8_t sc = inb(0x60);

    // ignore releases
    if (sc & 0x80) return;

    if (sc < 128) {
        char c = scancode_map[sc];
        if (c) buf_push(c);
    }
    
}

char keyboard_getchar(void) {
    // Now this polls the buffer, not hardware ports.
    while (buf_empty()) {
        __asm__ volatile ("sti; hlt"); // sleep until next interrupt
    }
    return buf_pop();
}

void keyboard_init(void) {
    // nothing needed yet; IRQ wiring is in the IDT/PIC setup
}

char keyboard_getchar_poll(void) {
    // wait for output buffer full
    while (!(inb(0x64) & 1)) {}
    unsigned char sc = inb(0x60);

    if (sc & 0x80) return 0; // ignore release

    if (sc < 128) return scancode_map[sc];
    return 0;
}
