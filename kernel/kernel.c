#include "gdt.h"
#include "vga.h"
#include "keyboard.h"
#include "shell.h"
#include "pic.h"
#include "idt.h"
#include "io.h"
#include "vga.h"
#include "pit.h"

static void kbd_dot_test(void) {
    vga_println("");
    vga_println("KBD TEST: press keys, should print dots...");

    while (1) {
        if (inb(0x64) & 1) {   // keyboard controller has data
            (void)inb(0x60);  // read scancode
            vga_putc('.');    // show activity
        }
    }
}

void kernel_main(void) {
    vga_clear();
    vga_println("EDEN OS - Genesis Phase (Multiboot)");
    vga_println("-----------------------------------");

    gdt_init();

    pic_remap();
    pic_set_masks(0xFC, 0xFF); // unmask IRQ0+IRQ1 (timer + keyboard)

    idt_init();

    __asm__ volatile ("sti");

    pit_init(100); // 100 Hz (10ms ticks). You can use 1000 later if you want.

    vga_println("Console online.");
    vga_println("Interrupts online.");
    vga_println("Keyboard (IRQ1) online.");

    keyboard_init();
    shell_run();

    while (1) {}
}
