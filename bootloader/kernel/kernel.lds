ENTRY(start);

SECTIONS
{
  . = 0x500;

  .text  :
  {
    *(.text.start)   /* entry first */
    *(.text*)        /* then everything else */
  }

  .data : { *(.data) }
  .bss  : { *(.bss) }
}
