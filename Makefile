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

eden.bin: boot.o kernel.o vga.o keyboard.o str.o shell.o gdt.o gdt_asm.o idt.o isr.o pic.o interrupts.o
	$(CC) $(LDFLAGS) boot.o kernel.o vga.o keyboard.o str.o shell.o gdt.o gdt_asm.o idt.o isr.o pic.o interrupts.o -o eden.bin

eden.iso: eden.bin grub.cfg
	mkdir -p iso/boot/grub
	cp eden.bin iso/boot/eden.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	i686-elf-grub-mkrescue -o eden.iso iso

run:
	qemu-system-i386 -cdrom eden.iso

clean:
	rm -rf *.o eden.bin iso eden.iso

io.o:
	$(CC) $(CFLAGS) -c kernel/io.h -o io.o

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
