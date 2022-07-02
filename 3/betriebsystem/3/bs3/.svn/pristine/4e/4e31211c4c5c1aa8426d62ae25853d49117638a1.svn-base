# Makefile Aufgabe "Threads"

CFLAGS = -std=c11 -D_GNU_SOURCE -Og -g -W -Wall -Wno-unused-parameter \
         -Werror=implicit-function-declaration -Wfatal-errors -pthread

.PHONY: all clean distclean

all: gibaus gibaus_sem gibaus_mutex baeckerei

gibaus: gibaus.c
	$(CC) $(CFLAGS) -o $@ $<

gibaus_sem: gibaus_sem.c
	$(CC) $(CFLAGS) -o $@ $<

gibaus_mutex: gibaus_mutex.c
	$(CC) $(CFLAGS) -o $@ $<

baeckerei: baeckerei.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f gibaus gibaus_sem gibaus_mutex baeckerei

distclean: clean
