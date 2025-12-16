#pragma once
#include <stdint.h>

void uart_init(void);
void uart_putc(char c);
void uart_print(const char* s);

int  uart_can_read(void);
char uart_getc(void);
