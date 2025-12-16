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

eden.bin: boot.o kernel.o vga.o
	$(CC) $(LDFLAGS) boot.o kernel.o vga.o -o eden.bin

eden.iso: eden.bin grub.cfg
	mkdir -p iso/boot/grub
	cp eden.bin iso/boot/eden.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	i686-elf-grub-mkrescue -o eden.iso iso

run:
	qemu-system-i386 -cdrom eden.iso

clean:
	rm -rf *.o eden.bin iso eden.iso
