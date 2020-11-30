/* addfunc.S
 * ASM impl of add/adds C fn (ARM Cortex-M3) 
 * LV  2522 Mikroprozessortechnik SS20
 * Marcus Thoss 
 */

	.syntax unified
 	.arch armv7-m
	.global adds, add /* make the (subroutine jump target) symbols adds and add globally known */

/* return values are always in r0*/

/* Executes ADDS on parameters in r0 and r1, returns APSR (Status flags register) */
adds:
	adds r2, r0, r1
	mov r0, APSR

/* Executes ADD on parameters in r0 and r1, returns result */
add:
  	add r0, r1
	bx lr
	

