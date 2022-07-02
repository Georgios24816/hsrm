main.elf: startup.o mpt1b_regs.o
	arm-none-eabi-ld -g -T startup.ld startup.o mpt1b_regs.o -o main.elf

startup.o: startup.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -g -c startup.c -o startup.o

mpt1b_regs.o: mpt2a.S
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -g -c mpt1b_regs.S -o mpt2a.o

clean:
	rm *.o main.elf

qemu: main.elf 
	qemu-system-arm -M lm3s6965evb --kernel main.elf --serial null -nographic -S -s

gdb: main.elf
	gdb-multiarch main.elf
