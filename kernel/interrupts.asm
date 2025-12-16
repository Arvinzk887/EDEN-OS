BITS 32

extern irq_handler
extern isr_handler

global idt_flush
idt_flush:
    mov eax, [esp + 4]
    lidt [eax]
    ret

; -------------------------
; IRQ stubs (32-47)
; -------------------------
%macro IRQ 2
global irq%1
irq%1:
    cli
    push dword 0        ; err_code
    push dword %2       ; int_no (32+irq)
    jmp irq_common_stub
%endmacro

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

irq_common_stub:
    pusha

    xor eax, eax
    mov ax, ds
    push eax            ; ds (uint32_t)

    mov ax, 0x10        ; kernel data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call irq_handler
    add esp, 4

    ; IMPORTANT: irq_handler returns the ESP we should resume (for scheduler)
    mov esp, eax

    pop eax             ; restore ds
    mov ds, ax

    popa
    add esp, 8          ; int_no + err_code
    iretd

; -------------------------
; Minimal ISRs (0-31)
; These are only here so you don't triple fault if an exception happens.
; We keep them simple and consistent: push err_code=0, int_no, call isr_handler.
; -------------------------
%macro ISR 1
global isr%1
isr%1:
    cli
    push dword 0
    push dword %1
    jmp isr_common_stub
%endmacro

ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20
ISR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 29
ISR 30
ISR 31

isr_common_stub:
    pusha

    xor eax, eax
    mov ax, ds
    push eax            ; ds (uint32_t)

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call isr_handler     ; IMPORTANT: exceptions call isr_handler, not irq_handler
    add esp, 4

    pop eax
    mov ds, ax

    popa
    add esp, 8
    iretd