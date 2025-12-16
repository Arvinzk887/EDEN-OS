#include "pit.h"
#include <stdint.h>

volatile uint32_t bg_ticks = 0;

void bg_task(void) {
    while (1) {
        bg_ticks++;
        __asm__ volatile ("sti; hlt");  // yield until next interrupt
    }
}
