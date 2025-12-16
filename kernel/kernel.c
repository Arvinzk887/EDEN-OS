#include "gdt.h"
#include "vga.h"
#include "keyboard.h"
#include "shell.h"
#include "pic.h"
#include "idt.h"
#include "io.h"
#include "vga.h"
#include "pit.h"
#include "sched.h"

extern void bg_task(void);

static void shell_task(void) {
    keyboard_init();
    shell_run();
}

void kernel_main(void) {
    vga_clear();
    vga_println("EDEN OS - Genesis Phase (Multiboot)");
    vga_println("-----------------------------------");

    gdt_init();

    pic_remap();
    pic_set_masks(0xFC, 0xFF); // unmask IRQ0+IRQ1 (timer + keyboard)

    idt_init();

    pit_init(100); // set PIT before enabling interrupts

    sched_init();
    task_create(shell_task);   // Task 0: shell
    task_create(bg_task);      // Task 1: background

    // jump into task 0 (never returns)
    sched_launch(sched_first_esp());

    vga_println("Console online.");
    vga_println("Interrupts online.");
    vga_println("Scheduler online.");

    __asm__ volatile ("sti");  // enable interrupts AFTER tasks exist

    
}
