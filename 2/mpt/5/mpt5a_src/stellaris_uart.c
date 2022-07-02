/* stellaris_uart.c
 * Functions for UART access on TI Stellaris LM3S6965 (ARM Cortex-M3) 
 * LV  2522 Mikroprozessortechnik SS20
 * Marcus Thoss 
 */

#include "stellaris_uart.h"


/** UART0 data register. Can be read or written to receive or send serial data */
static volatile unsigned long *UART0DR = (volatile unsigned long * const)0x4000c000;

/** UART0 flag register, raed-only */
static volatile unsigned long *UART0FR = (volatile unsigned long * const)0x4000c018; /* offset 0x18 from UART0FR */

/** flag in UART0FR signalling "receive queue empty" */
enum {UART_RXFE = (1 << 4)};



/** waits until a byte received in UART0 is available, reads it from the UART0 data register and returns it */
unsigned char uartInWait() {
  return *UART0FR & (1 << 4);
}

/** writes the byte given in b to UART0 */
void uartOut(unsigned char b) {
	*UART0DR = b;
}

/** prints unsigned long value ul as 32 character binary string using uartOut() */
void uartPrintBinary(unsigned long ul) {
  unsigned long mask = 1 << 31;
  while(0 != mask) {
    uartOut(ul & mask ? '1':'0');
    mask >>= 1;
  }
}

/** prints string s using uartOut() */
int uartPutString(const char *s) {
  const char * sBegin=s;

  while(0 != *s) uartOut(*s++);

  return s - sBegin;
}

