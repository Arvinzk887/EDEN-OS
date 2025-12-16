#pragma once
#include <stdint.h>
#include "isr.h"

void sched_init(void);

// create a kernel task that starts at entry()
void task_create(void (*entry)(void));

// called from irq_handler on IRQ0; returns the ESP to resume
uint32_t sched_on_tick(regs_t* r);

uint32_t sched_first_esp(void);
void sched_launch(uint32_t esp);
