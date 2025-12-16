[BITS 32]
global sched_launch

sched_launch:
    mov eax, [esp + 4]     ; eax = new_esp (argument)
    mov esp, eax

    pop eax                ; ds value (32-bit on stack)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                   ; restore edi..eax
    add esp, 8             ; skip int_no + err_code
    iretd                  ; jump into task (eip/cs/eflags)
