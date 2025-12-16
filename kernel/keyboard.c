#include "keyboard.h"
#include "io.h"

static const char scancode_map[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/',0,
    '*',0,' '
};

char keyboard_getchar(void) {
    while (1) {
        // wait until output buffer is full
        if (!(inb(0x64) & 1)) {
            continue;
        }

        uint8_t scancode = inb(0x60);

        // ignore key releases
        if (scancode & 0x80) {
            continue;
        }

        char c = scancode_map[scancode];
        if (c) {
            return c;
        }
    }
}

void keyboard_init(void) {
    // nothing needed for polling
}
