ENTRY(start);

SECTIONS
{
  . = 0x500;
  
  .text 0x600 : ALIGN(0x100)
  {
    *(.text.start)   /* entry first */
    *(.text*)        /* then everything else */
  }

  .data : { *(.data) }
  .bss  : { *(.bss) }
}
