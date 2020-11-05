# Makefile Aufgabe "Dateien"
#
# Das Makefile fuer das erste Praktikum.
#
# Zeilen mit "#" am Anfang sind Kommentare.
# Alle Zeichen nach dem Kommentarmarker "#" werden vom make-Programm ignoriert.

# Die folgende Anweisung legt eine Variable "CFLAGS" an,
# hier die Standard-Argumente fuer den Compiler-Aufruf
CFLAGS = -std=c11 -D_GNU_SOURCE -Og -g -W -Wall -Wno-unused-parameter \
         -Werror=implicit-function-declaration -Wfatal-errors -pthread

.PHONY: all clean distclean

# Die folgende Anweisung beschreibt eine Regel:
# - die linke Seite vom Doppelpunkt beschreibt ein sogenanntes "Target"
# - die rechte Seite listet eine oder mehrere Abhaengigkeiten auf
# - die Abhaengigkeiten sind entweder Targets oder Dateien
#
# Die erste Regel wird per default ausgefuehrt:
# Es sollen alle Programme kompiliert werden.
all: mybcp myrevbcp mybappend mycp filelength grow

# Anweisung, wie kompiliert werden soll
# Dabei werden Variablen in $(variablenname) passend ersetzt
mybcp: mybcp.c
	$(CC) $(CFLAGS) -o mybcp mybcp.c

# wie oben, die eingebaute Variable $@ enthaelt das Target der Regel
myrevbcp: myrevbcp.c
	$(CC) $(CFLAGS) -o $@ myrevbcp.c

# wie oben, die Variable $< enthaelt die erste Abhaengigkeit der Regel
mybappend: mybappend.c
	$(CC) $(CFLAGS) -o $@ $<

mycp: mycp.c
	$(CC) $(CFLAGS) -o $@ $<

filelength: filelength.c
	$(CC) $(CFLAGS) -o $@ $<

grow: grow.c
	$(CC) $(CFLAGS) -o $@ $<

# Die "Aufraeumregel": Loesche erzeugte Binaerdateien
clean:
	rm -f mybcp myrevbcp mybappend mycp filelength grow

distclean: clean
