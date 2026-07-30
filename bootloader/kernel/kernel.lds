ENTRY(start);

SECTIONS
{
  . = 0x500;
  .entry 0x500:{
    *(.entry)
  }
  .text :
  {
    *(.text*)
  }

  .data : { *(.data) }
  .bss  : { *(.bss) }
}
