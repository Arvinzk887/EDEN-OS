# EDEN OS

**EDEN OS** is a minimal experimental operating system written in **C** and **x86 (i386) assembly**, built from scratch to explore kernel architecture, interrupts, and scheduling.

The project prioritizes clarity, correctness, and incremental development over completeness.

---

## Genesis Phase

The Genesis phase establishes a functional kernel foundation:

- Multiboot-compliant kernel
- GRUB bootloader
- VGA text-mode console
- Freestanding C environment (no libc)
- Global Descriptor Table (GDT)
- Interrupt Descriptor Table (IDT)
- Programmable Interrupt Controller (PIC) remapping
- Programmable Interval Timer (PIT)
- Keyboard input via IRQ1
- Interactive kernel shell
- PIT-driven **preemptive multitasking**
- Simple round-robin scheduler
- Multiple kernel tasks with independent stacks

---

## Architecture Overview

- **Boot**: GRUB → Multiboot → kernel entry
- **Interrupts**: Hardware IRQs handled via IDT stubs and common handlers
- **Scheduling**: Timer interrupts trigger context switches by swapping kernel stacks
- **Tasks**: Each task runs on its own kernel stack
- **Shell**: Runs as a scheduled kernel task

---

## Build & Run

### Requirements
- `i686-elf-gcc`
- `nasm`
- `qemu-system-i386`
- `grub-mkrescue`

### Build
```bash
make
make run