CC = i686-elf-gcc
AS = nasm
CFLAGS = -ffreestanding -O2 -Wall -Wextra -m32
LDFLAGS = -T linker.ld -nostdlib -ffreestanding -m32

all: eden.iso

boot.o:
	$(AS) -f elf32 boot/boot.asm -o boot.o

kernel.o:
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel.o

vga.o:
	$(CC) $(CFLAGS) -c kernel/vga.c -o vga.o

eden.bin: boot.o kernel.o vga.o keyboard.o str.o shell.o gdt.o gdt_asm.o idt.o isr.o pic.o interrupts.o power.o pit.o sched.o sched_asm.o bg.o uart.o
	$(CC) $(LDFLAGS) boot.o kernel.o vga.o keyboard.o str.o shell.o gdt.o gdt_asm.o idt.o isr.o pic.o interrupts.o power.o pit.o sched.o sched_asm.o bg.o uart.o -o eden.bin

eden.iso: eden.bin grub.cfg
	mkdir -p iso/boot/grub
	cp eden.bin iso/boot/eden.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	i686-elf-grub-mkrescue -o eden.iso iso

run:
	qemu-system-i386 -cdrom eden.iso -device isa-debug-exit,iobase=0xf4,iosize=0x04 -no-reboot -no-shutdown

clean:
	rm -rf *.o eden.bin iso eden.iso

keyboard.o:
	$(CC) $(CFLAGS) -c kernel/keyboard.c -o keyboard.o

str.o:
	$(CC) $(CFLAGS) -c kernel/str.c -o str.o

shell.o:
	$(CC) $(CFLAGS) -c kernel/shell.c -o shell.o

interrupts.o:
	$(AS) -f elf32 kernel/interrupts.asm -o interrupts.o

idt.o:
	$(CC) $(CFLAGS) -c kernel/idt.c -o idt.o

isr.o:
	$(CC) $(CFLAGS) -c kernel/isr.c -o isr.o

pic.o:
	$(CC) $(CFLAGS) -c kernel/pic.c -o pic.o

gdt.o:
	$(CC) $(CFLAGS) -c kernel/gdt.c -o gdt.o

gdt_asm.o:
	$(AS) -f elf32 kernel/gdt.asm -o gdt_asm.o

power.o:
	$(CC) $(CFLAGS) -c kernel/power.c -o power.o

pit.o:
	$(CC) $(CFLAGS) -c kernel/pit.c -o pit.o

sched.o:
	$(CC) $(CFLAGS) -c kernel/sched.c -o sched.o

bg.o:
	$(CC) $(CFLAGS) -c kernel/bg.c -o bg.o

sched_asm.o:
	$(AS) -f elf32 kernel/sched_asm.asm -o sched_asm.o

uart.o:
	$(CC) $(CFLAGS) -c kernel/uart.c -o uart.o
