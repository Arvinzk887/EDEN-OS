#include "shell.h"
#include "vga.h"
#include "keyboard.h"
#include "str.h"

#define LINE_MAX 80

static void prompt(void) {
    vga_print("eden> ");
}

static void exec_cmd(const char* line) {
    if (kstreq(line, "")) return;

    if (kstreq(line, "help")) {
        vga_println("Commands:");
        vga_println("  help        - show this help");
        vga_println("  clear       - clear screen");
        vga_println("  halt        - halt CPU");
        vga_println("  echo <txt>  - print text");
        return;
    }

    if (kstreq(line, "clear")) {
        vga_clear();
        return;
    }

    if (kstreq(line, "halt")) {
        vga_println("Halting...");
        __asm__ volatile ("cli; hlt");
        return;
    }

    if (kstartswith(line, "echo ")) {
        vga_println(line + 5);
        return;
    }

    vga_print("Unknown command: ");
    vga_println(line);
}

void shell_run(void) {
    char line[LINE_MAX];
    size_t len = 0;

    vga_println("");
    vga_println("EDEN shell online. Type 'help'.");
    prompt();

    while (1) {
        char c = keyboard_getchar();

        // Enter
        if (c == '\n') {
            vga_putc('\n');
            line[len] = '\0';
            exec_cmd(line);
            len = 0;
            prompt();
            continue;
        }

        // Backspace
        if (c == '\b') {
            if (len > 0) {
                len--;

                // move cursor back visually: backspace, overwrite with space, backspace again
                vga_putc('\b');
                vga_putc(' ');
                vga_putc('\b');
            }
            continue;
        }

        // Printable range (keep it simple)
        if (c >= ' ' && c <= '~') {
            if (len < LINE_MAX - 1) {
                line[len++] = c;
                vga_putc(c);
            }
        }
    }
}
