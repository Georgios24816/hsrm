#define __AVR_ATmega16__

;; special function registers: the two halves of the stack pointer address
        SPH = 0x3e
        SPL = 0x3d
        
        ;; program data follows
        .section .data

        ;; program code follows
        .section .text

        ;; global address marker, later also entry point for C main()
        .global main

main:
        ;; init stackpointer to 0x017f (extra low address to better see it in the simulator - normally: 0x08ff, end of RAM)
		ldi r25, 55
		ldi r24, 25
		rcall ggt

endloop:
		rjmp endloop

ggt:	mov r26, r25
ggt_: 					;ggT(a, b): a=r24, b=r25
		cp r24, r25
		breq exitGgt	; a == b
		brlt lessGgt	;a < b brlt
		brge moreGgt 	;b < a
		ret

exitGgt:
	mov r25, r26
	ret

lessGgt:
		sub r25, r24 	;r25 -= r24
		rjmp ggt_
moreGgt:
		sub r24, r25		;r24 -= r25
		rjmp ggt_
