#define __AVR_ATmega16__

.section .data

.section .text
.global main

main:	
	ldi r18, 0x03	;Ziel: r19 = 0x27
	dec r18			;r18=2
	mov r19, r18 	;r18=2 r19=2
	mul r18, r18	;r18=4
	mov r20, r18	;r20=4
	mul r18, r18	;r18=16
	mul r18, r19	;r18=32
	mov r19, r18	;r19=32
	add r19, r20	;r19=36
	add r19, r20	;r19=40
	dec r19			;r19=39=0x27
equal:
	rjmp equal
