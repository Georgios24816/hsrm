mpt5a.elf: mpt5a.o
	arm-none-eabi-ld -g -T startup.ld mpt5a.o -o mpt5a.elf -L /usr/lib/arm-none-eabi/lib/thumb/v7-m/ -lc 

mpt5a.o: mpt5a.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -g -Wall -lgcc -lc -c mpt5a.c -o mpt5a.o  --specs=rdimon.specs -lgcc -lc -lm -lrdimon
