#pragma once
#include <stdint.h>

void pit_init(uint32_t freq_hz);
void pit_irq_handler(void);

uint64_t pit_ticks(void);
uint32_t pit_hz(void);

uint64_t pit_uptime_ms(void);
void pit_sleep_ms(uint32_t ms);
