# EDEN OS

**EDEN OS** is a minimal experimental operating system written in **C** and **x86 (i386) assembly**.

It is built from scratch to explore low-level system design, interrupt handling, deterministic scheduling, and hardware-controlled data experiments. The project prioritizes correctness, transparency, and intentional scope over feature completeness.

---

## Purpose & Vision

EDEN OS is designed as a **deterministic control and experiment platform**, not a general-purpose desktop operating system.

Its long-term goal is to serve as the software foundation for the **Eden Project**, a research effort exploring unconventional data persistence mechanisms — including experiments involving light, solid-state materials, and diamond-based storage concepts.

In this context, EDEN OS aims to provide:
- Precise timing and scheduling
- Reliable data capture and verification
- Clean, auditable execution paths
- Direct control over hardware interfaces

The operating system is intended to act as the **orchestrator and verifier** for experimental pipelines, rather than the storage medium itself.

---

## Genesis Phase (Current)

The Genesis phase establishes a stable kernel foundation:

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

## User Interface

EDEN OS currently operates in VGA text mode with a minimal interactive shell.

A graphical user interface is **not an immediate priority**. UI development will be considered only after the kernel core, scheduler, memory handling, and hardware interfaces are stable.

Planned future UI directions may include:
- Framebuffer graphics
- Simple windowing or visualization tools
- Experiment status and data inspection views

---

## Roadmap

Development proceeds in clearly defined phases:

### Genesis (Current)
- Core kernel bring-up
- Interrupt handling
- Preemptive scheduler
- Shell and input handling

### Exodus
- Task sleep and wake states
- Improved scheduler control
- Deterministic timing utilities
- Serial I/O for real hardware debugging
- Structured experiment records

### Leviticus
- Paging and memory management
- Kernel heap allocator
- Safer task isolation

### Numbers
- User-mode processes
- System call interface
- Experiment drivers as isolated services

### Deuteronomy
- Persistent storage support
- Data cataloging and verification
- Refinement, documentation, and stability work

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