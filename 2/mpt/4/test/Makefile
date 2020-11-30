all: Aufgabe1.hex Aufgabe_e.elf Aufgabe_e.elf

Aufgabe1.hex: Aufgabe1.S
	avr-gcc -g -Wall Aufgabe1.S -o Aufgabe1.hex

Aufgabe_e.elf: Aufgabe_e.o
	arm-none-eabi-ld -g -T startup.ld Aufgabe_e.o -o Aufgabe_e.elf

Aufgabe_e.o: Aufgabe_e.S
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -g -Wall -c Aufgabe_e.S -o Aufgabe_e.o

Aufgabe_e.elf: Aufgabe_e.o
	arm-none-eabi-ld -g -T startup.ld Aufgabe_e.o -o Aufgabe_e.elf

Aufgabe_e.o: Aufgabe_e.S
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -g -Wall -c Aufgabe_e.S -o Aufgabe_e.o

sim1: 
	simulavr -d atmega16 simulavr-disp -g

gdb1: Aufgabe1.hex
	avr-gdb -tui Aufgabe1.hex

sim2: Aufgabe_e.elf
	qemu-system-arm -M lm3s6965evb --kernel Aufgabe_e.elf --serial null -nographic -S -s

gdb2: Aufgabe_e.elf
	gdb-multiarch -tui Aufgabe_e.elf

sim_f: Aufgabe_e.elf
	qemu-system-arm -M lm3s6965evb --kernel Aufgabe_e.elf --serial null -nographic -S -s

gdb_f: Aufgabe_e.elf
	gdb-multiarch -tui Aufgabe_e.elf
