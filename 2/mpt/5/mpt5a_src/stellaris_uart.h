/* stellaris_uart.h 
 * Functions for UART access on TI Stellaris LM3S6965 (ARM Cortex-M3) 
 * LV  2522 Mikroprozessortechnik SS20
 * Marcus Thoss 
 */

#ifndef STELLARIS_UART_H
#define STELLARIS_UART_H

/** writes the byte given in b to UART0 */
void uartOut(unsigned char b);

/** prints unsigned long value ul as 32 character binary string */
void uartPrintBinary(unsigned long ul);

/** prints string s using uartOut() */
int uartPutString(const char *s);

/** waits until a byte received in UART0 is available, reads it from the UART0 data register and returns it */
unsigned char uartInWait();

#endif /* STELLARIS_UART_H */
