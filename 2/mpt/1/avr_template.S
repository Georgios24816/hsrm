        ;;  LV  2522 Mikroprozessortechnik SS20
        ;;  Rumpf-Assemblercode
        ;;  Marcus Thoss


;; processor selection. Instead, you can use -mmcu=atmega16 as avr-gcc option instead
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
        ldi r16,0x01
        out SPH,r16
        ldi r16,0x7f
        out SPL,r16

        ;; rest of main code goes here

endloop:
        rjmp endloop

        ;; more subroutines can be placed below

