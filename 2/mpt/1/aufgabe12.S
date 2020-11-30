#define __AVR_ATmega16__

.section .data

.section .text
.global main

main:	
	ldi r18, 0
	ldi r19, 3

comparison:
	cpi r19, 0
	breq equal
	add r18, r19
	dec r19
	rjmp comparison

equal:
	rjmp equal
