	.syntax	unified
	.arch	armv7-m

	.section .text

	.global main
main:
		mov r0, #0x80000000
		mov r1, #0x80000000
		adds r0, r0, r1		

endloop:
		b endloop
