#pragma once
#include <stdint.h>

typedef struct regs {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} regs_t;

uint32_t isr_handler(regs_t* r);
uint32_t irq_handler(regs_t* r);
