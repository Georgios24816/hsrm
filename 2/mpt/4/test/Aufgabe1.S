#define __AVR_ATmega16__

.section .data

.section .text

.global main
main:
		ldi r22, 0x2E
		ldi r24, 0x13
		rcall myfunc

endloop:
		rjmp endloop


myfunc:
		eor r24, r22
		ret
