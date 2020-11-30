	.syntax	unified
	.arch	armv7-m

	.section .text

	.global main
main:
		mov r1, #0x2E
		mov r0, #0x13
		b myfunc

endloop:
		b endloop

myfunc:
		eor r0, r0, r1
		bx lr
