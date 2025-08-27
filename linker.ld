ENTRY(_start)

SECTIONS {
    . = 1M;

    __kernel_start = .;

    .multiboot2 :
    {
        KEEP(*(.multiboot2))
    }

    .text :
    {
        *(.text)
    }

    .rodata :
    {
        *(.rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss COMMON)
    }

    __kernel_end = .;
}
