#include "vga.h"

static volatile uint16_t* const VGA = (uint16_t*)0xB8000;
static const int VGA_W = 80;
static const int VGA_H = 25;

static uint8_t vga_color = 0x0F; // white on black
static int cursor_row = 0;
static int cursor_col = 0;

static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

static void scroll_if_needed(void) {
    if (cursor_row < VGA_H) return;

    // Move rows 1..H-1 up to 0..H-2
    for (int r = 1; r < VGA_H; r++) {
        for (int c = 0; c < VGA_W; c++) {
            VGA[(r - 1) * VGA_W + c] = VGA[r * VGA_W + c];
        }
    }

    // Clear last row
    for (int c = 0; c < VGA_W; c++) {
        VGA[(VGA_H - 1) * VGA_W + c] = vga_entry(' ', vga_color);
    }

    cursor_row = VGA_H - 1;
}

void vga_clear(void) {
    for (int r = 0; r < VGA_H; r++) {
        for (int c = 0; c < VGA_W; c++) {
            VGA[r * VGA_W + c] = vga_entry(' ', vga_color);
        }
    }
    cursor_row = 0;
    cursor_col = 0;
}

void vga_putc(char ch) {
    if (ch == '\n') {
        cursor_col = 0;
        cursor_row++;
        scroll_if_needed();
        return;
    }

    if (ch == '\r') {
        cursor_col = 0;
        return;
    }

    if (ch == '\b') {
        // move cursor left one cell (if possible)
        if (cursor_col > 0) {
            cursor_col--;
        } else if (cursor_row > 0) {
            cursor_row--;
            cursor_col = VGA_W - 1;
        } else {
            return;
        }

        // do not erase here; caller may overwrite with space
        return;
    }

    VGA[cursor_row * VGA_W + cursor_col] = vga_entry(ch, vga_color);

    cursor_col++;
    if (cursor_col >= VGA_W) {
        cursor_col = 0;
        cursor_row++;
        scroll_if_needed();
    }
}

void vga_print(const char* s) {
    for (size_t i = 0; s[i] != '\0'; i++) {
        vga_putc(s[i]);
    }
}

void vga_println(const char* s) {
    vga_print(s);
    vga_putc('\n');
}

void vga_print_u64(uint64_t n) {
    char buf[21];
    int i = 0;

    if (n == 0) {
        vga_putc('0');
        return;
    }

    while (n > 0 && i < 20) {
        buf[i++] = (char)('0' + (n % 10));
        n /= 10;
    }

    while (i > 0) {
        vga_putc(buf[--i]);
    }
}
