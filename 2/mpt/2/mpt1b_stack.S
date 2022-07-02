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
        ;; init stackpointer to 0x017f 
		ldi r24, 55
		ldi r25, 25
		rcall ggt_

endloop:
		rjmp endloop


ggt_:
		push r24
		push r25
ggt: 					;ggT(a, b): a=r24, b=r25
		cmp r24, r25	
		breq exitGgt	; a == b
		brlt lessGgt	;a < b brlt
		brge moreGgt 	;b < a
		ret

exitGgt:
	pop r25
	pop r24
	ret

lessGgt:
		sub r25, r24 	;r25 -= r24
		rjmp ggt
moreGgt:
		sub r24, r25		;r24 -= r25
		rjmp ggt
