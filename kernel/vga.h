#pragma once
#include <stddef.h>
#include <stdint.h>

void vga_clear(void);
void vga_putc(char c);
void vga_print(const char* s);
void vga_println(const char* s);
void vga_print_u64(uint64_t n);