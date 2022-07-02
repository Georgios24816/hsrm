# Makefile Aufgabe "Prozesse"

CFLAGS = -std=c11 -D_GNU_SOURCE -Og -g -W -Wall -Wno-unused-parameter \
         -Werror=implicit-function-declaration -Wfatal-errors -pthread

.PHONY: all clean distclean

all: parent child1 child2 mytime

parent: parent.c
	$(CC) $(CFLAGS) -o $@ $<

child1: child1.c
	$(CC) $(CFLAGS) -o $@ $<

child2: child2.c
	$(CC) $(CFLAGS) -o $@ $<

mytime: mytime.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f parent child1 child2 mytime

distclean: clean
