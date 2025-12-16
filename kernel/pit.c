#include "pit.h"
#include "io.h"

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43

static volatile uint64_t g_ticks = 0;
static volatile uint64_t g_ms = 0;

static uint32_t g_hz = 0;

// Fixed-point accumulator: add 1000 each tick, carry into ms by subtracting hz
static uint32_t g_accum = 0;

void pit_init(uint32_t freq_hz) {
    if (freq_hz == 0) freq_hz = 100;
    g_hz = freq_hz;

    // PIT input clock ~ 1,193,182 Hz
    uint32_t divisor = 1193182u / freq_hz;
    if (divisor == 0) divisor = 1;
    if (divisor > 0xFFFF) divisor = 0xFFFF;

    outb(PIT_COMMAND, 0x36); // ch0, lobyte/hibyte, mode 3
    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));

    g_ticks = 0;
    g_ms = 0;
    g_accum = 0;
}

void pit_irq_handler(void) {
    g_ticks++;

    // Add 1000 "ms-units" per tick, convert to real ms without division:
    // Each time accum >= hz, we earned 1ms.
    g_accum += 1000;

    while (g_accum >= g_hz) {
        g_accum -= g_hz;
        g_ms++;
    }
}

uint64_t pit_ticks(void) {
    return g_ticks;
}

uint32_t pit_hz(void) {
    return g_hz;
}

uint64_t pit_uptime_ms(void) {
    return g_ms;
}

void pit_sleep_ms(uint32_t ms) {
    uint64_t start = g_ms;
    while ((g_ms - start) < (uint64_t)ms) {
        __asm__ volatile ("sti; hlt");
    }
}
