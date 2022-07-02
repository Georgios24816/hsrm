/*  LV  2522 Mikroprozessortechnik SS19
    mpt2a.S
    Aufgabe 2: Erster ARM-Assembler-Quellcode
    Marcus Thoss
*/

	.syntax	unified
	.arch	armv7-m

	.section .text

	// global address marker, later also entry point for C main()
	.global main

main:

	mov r0, #24 // use r0, r1 for paramters
	mov r1, #18
	bl addproc // add r0 and r1, result can be found in r0
endloop:
	b endloop

addproc:
  add r0, r0, r1 // r0 = r0 + r1
  bx lr // jump to address in register LR, (= AVR ret instruction)