BITS 32

section .multiboot_header
align 4
dd 0x1BADB002
dd 0
dd -(0x1BADB002)

section .text
global _start
extern kernel_main

_start:
    cli
    call kernel_main
.hang:
    hlt
    jmp .hang
