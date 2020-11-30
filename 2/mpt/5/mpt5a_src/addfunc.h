/* addfunc.h 
 * C/ASM Wrapper for ADD/ADDS instruction (ARM Cortex-M3) 
 * LV  2522 Mikroprozessortechnik SS20
 * Marcus Thoss 
 */

#ifndef ADDSFN_H
#define ADDSFN_H

/** Executes ADDS on parameters a and b, returns ASPR (Status flags register) */
extern unsigned long adds(unsigned long a, unsigned long b);

/** Executes ADD on parameters a and b, returns result */
extern unsigned long add(unsigned long a, unsigned long b);

#endif /* ADDSFN_H */
