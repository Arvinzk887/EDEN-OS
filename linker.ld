ENTRY(_start)

SECTIONS {
    . = 1M;

    .multiboot_header : {
        KEEP(*(.multiboot_header))
    }

    .text : {
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(COMMON)
        *(.bss*)
    }
}
