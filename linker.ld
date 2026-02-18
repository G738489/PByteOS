ENTRY(start)
OUTPUT_FORMAT(elf32-i386)

SECTIONS {
    . = 1M;
    .text : {
        *(.multiboot)
        *(.text)
    }
    .data : { *(.data) }
    .bss  : { *(.bss)  }
}