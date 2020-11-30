Aufgabe_e_c.elf: Aufgabe_e_c.o
	arm-none-eabi-ld -g -T startup.ld Aufgabe_e_c.o -o Aufgabe_e_c.elf

Aufgabe_e_c.o: Aufgabe_e_c.S
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -g -Wall -c Aufgabe_e_c.S -o Aufgabe_e_c.o

sim: Aufgabe_e_c.elf
	qemu-system-arm -M lm3s6965evb --kernel Aufgabe_e_c.elf -nographic -S -s

gdb: Aufgabe_e_c.elf
	gdb-multiarch -tui Aufgabe_e_c.elf
