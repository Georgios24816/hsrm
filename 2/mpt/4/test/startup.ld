/* startup.ld */
/* Linker script for TI Stellaris LM3S6965 (ARM Cortex-M3) */
/* Matching startup.c */
/* LV  2522 Mikroprozessortechnik SS20 */
/* Marcus Thoss */

/* like for the original HW processor, we define 256 Flash and 64K RAM memory */
MEMORY
{
 FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
 SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

/* Define labels for the beginning and end addresses of sections.
 * Section lables match those used by ARM compiler toolchain 
 */
SECTIONS
{
 .text :
 {
    KEEP(*(.isr_vector))
    *(.text*)
    *(.rodata*)
    __etext = .;
  } > FLASH

  .data : AT (__etext)
  {
    __data_start__ = .;
    *(vtable)
    *(.data*)
    __data_end__ = .;
  } > SRAM

  .bss :
  {
    __bss_start__ = .;
    *(.bss*)
    *(COMMON)
    __bss_end__ = .;
  } > SRAM
}

