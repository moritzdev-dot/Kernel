ENTRY(_start)
OUTPUT_FORMAT(binary)
SECTIONS {
  . = 1M;

  .text ALIGN(4K) : 
  {
    *(.text)
  }

  .rodata : ALIGN(4K) 
  {
    *(.rodata)
  }

  .data : ALIGN(4K) 
  {
    *(.data)
  }

  .bss : ALIGN(4K) 
  {
    *(COMMON)
    *(.bss)
  }

  .asm : ALIGN(4K) 
  {
    *(.asm)
  }
}

