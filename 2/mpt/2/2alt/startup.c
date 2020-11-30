/* startup.c 
 * Minimal startup code for TI Stellaris LM3S6965 (ARM Cortex-M3) 
 * Matching startup.c 
 * LV  2522 Mikroprozessortechnik SS19 
 * Marcus Thoss 
 */


/* the main stack size we will set up */
#define SIMPLE_MAIN_STACK_SIZE 512

void ResetHandler(void);
void NMIHandler(void);
void ISRDefaultHandler(void);


/* C main() function entry point */
extern int main(void);


/* this array is actually going to be the main stack */
static unsigned long simpleMainStack[512];


/* The vector table. Note the placement of the stack pointer at the 
 * stack end address (top of stack, an empty stack) as the first 
 * table entry.
 */

__attribute__ ((section(".isr_vector")))
void (* const pISRVectors[])(void) =
{
  /* stack pointer */
  (void (*)(void))((unsigned long)simpleMainStack + sizeof(simpleMainStack)),
  /* reset handler */
  ResetHandler,
  /* NMI handler */
  NMIHandler,
  /* fault handlers: Hard, MPU, Bus, Usage */
  ISRDefaultHandler,
  ISRDefaultHandler,
  ISRDefaultHandler,
  ISRDefaultHandler,
  /* reserved values */
  0,
  0,
  0,
  0,
  /* System Service call handler */
  ISRDefaultHandler,
  /* Debug Monitor handler */
  ISRDefaultHandler,
  /* reserved */
  0,
  /* Pendable request for system service handler */
  ISRDefaultHandler,
  /* System tick timer handler */
  ISRDefaultHandler
};

     

/* linker generatd symbols for section addresses */
extern unsigned long __etext;
extern unsigned long __data_start__;
extern unsigned long __data_end__;
extern unsigned long __bss_start__;
extern unsigned long __bss_end__;

     

/* minimalist reset handler, intializing data segment, 
 * clearing BSS and jumping into main() 
 */
void ResetHandler(void) {
  
  unsigned long *pDataFromFlash , *pDataInSRAM;

  /* data segment initial values must be copied from Flash memory to SRAM */
  pDataFromFlash = &__etext;
  pDataInSRAM = &__data_start__; 

  while(pDataInSRAM < &__data_end__ ) {
      *pDataInSRAM++ = *pDataFromFlash++;
  }

  /* write all zeroes to BSS segment */
  __asm("ldr	r1, =__bss_start__\n"
	"ldr	r2, =__bss_end__\n"

	"movs	r0, 0\n"
	
	".L_loop3:\n"
	"cmp	r1, r2\n"
	"itt	lt\n"
	"strlt	r0, [r1], #4\n"
	"blt	.L_loop3\n");

  /* call C main() function */
  main();
}

     

/* dummy NMI handler, getting stuck in infinite loop */
void NMIHandler(void) {
  while(1)
    ;
}

/* dummy handler for any other ISR, getting stuck in infinite loop */
void ISRDefaultHandler(void) {
  while(1)
    ;
}

