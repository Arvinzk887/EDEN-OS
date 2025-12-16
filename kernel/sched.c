#include "sched.h"
#include "vga.h"

// Very small round-robin scheduler: max 2 tasks for now (easy + stable).
#define MAX_TASKS 2
#define STACK_WORDS 4096  // 16KB stack (4096 * 4)

typedef struct {
    uint32_t esp;                 // saved ESP pointing at ds in our interrupt frame
    uint32_t stack[STACK_WORDS];  // kernel stack backing store
    int used;
} task_t;

static task_t g_tasks[MAX_TASKS];
static int g_current = 0;
static int g_count = 0;

static uint32_t make_initial_esp(task_t* t, void (*entry)(void)) {
    // Build a fake interrupt frame so our irq_common_stub can "return" into it.
    // Stack layout expected at ESP (top -> down):
    //   ds
    //   edi esi ebp esp_dummy ebx edx ecx eax   (popa expects this)
    //   int_no
    //   err_code
    //   eip
    //   cs
    //   eflags

    uint32_t* sp = &t->stack[STACK_WORDS];

    // CPU iret frame (no privilege change): eip, cs, eflags (pushed in reverse)
    *(--sp) = 0x00000202u;           // EFLAGS: IF=1
    *(--sp) = 0x00000008u;           // CS: kernel code selector
    *(--sp) = (uint32_t)entry;       // EIP

    *(--sp) = 0;                     // err_code
    *(--sp) = 32;                    // int_no (pretend IRQ0)

    // pusha frame (reverse of popa)
    *(--sp) = 0;  // eax
    *(--sp) = 0;  // ecx
    *(--sp) = 0;  // edx
    *(--sp) = 0;  // ebx
    *(--sp) = 0;  // esp_dummy (ignored by popa)
    *(--sp) = 0;  // ebp
    *(--sp) = 0;  // esi
    *(--sp) = 0;  // edi

    *(--sp) = 0x10; // ds: kernel data selector

    return (uint32_t)sp;
}

void sched_init(void) {
    for (int i = 0; i < MAX_TASKS; i++) g_tasks[i].used = 0;
    g_current = 0;
    g_count = 0;
}

void task_create(void (*entry)(void)) {
    if (g_count >= MAX_TASKS) return;

    task_t* t = &g_tasks[g_count];
    t->used = 1;
    t->esp = make_initial_esp(t, entry);

    g_count++;
}

// Called on each timer tick. Save current context, choose next, return next ESP.
uint32_t sched_on_tick(regs_t* r) {
    if (g_count < 2) {
        // Only one runnable task => no switching.
        return (uint32_t)r;
    }

    // Save current task's ESP (points to ds in the interrupt frame)
    g_tasks[g_current].esp = (uint32_t)r;

    // Next task
    g_current = (g_current + 1) % g_count;

    return g_tasks[g_current].esp;
}

uint32_t sched_first_esp(void) {
    return g_tasks[0].esp;
}

