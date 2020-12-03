Praktikum Betriebssysteme
==========================

Aufgabe 1 -- Einfuehrung
-------------------------

Herzlich Willkommen im Praktikum Betriebssystem und Rechnerarchitektur!

Im SVN-Repository finden Sie fuer jedes Aufgabenblatt ein eigenes Verzeichnis
mit allen notwendigen Materialien. Fuer die meisten Aufgaben finden Sie
sogar eine Beispiel-Datei vor, die Sie einfach nur noch abaendern muessen.

Bitte benutzen Sie ausschliesslich die vorhandenen Dateien und benennen
Sie diese nicht um, da nur so eine schnelle Ueberpruefung der Abgaben
sichergestellt werden kann!


Verzeichnisstruktur
--------------------

- README.TXT            Diese Datei
- BEWERTUNG.TXT         Ihre Bewertung (vom Dozenten ausgefuellt)
- Makefile              Makefile

- mybcp.c               Quellcode Byteweise Kopieren
- mybappend.c           Quellcode Byteweise Anhaengen
- myrevbcp.c            Quellcode Rueckwaerts Kopieren
- mycp.c                Quellcode Kopieren
- filelength.c          Quellcode Dateigroesse ausgeben
- grow.c                Quellcode Dateigroesse setzen

- test_mybcp.sh         Tests fuer mybcp
- test_mybappend.sh     Tests fuer mybappend
- test_myrevbcp.sh      Tests fuer myrevbcp
- test_mycp.sh          Tests fuer mycp
- test_filelength.sh    Tests fuer filelength
- test_grow.sh          Tests fuer grow

- datei[123]            Temporaer angelegte Dateien der Tests

Hinweise
---------

Das mitgelieferte Makefile erlaubt die Kompilierung aller Programme auf einmal:

  $ make
  cc -std=c99 -g -W -Wall -Wno-unused-parameter -Wfatal-errors -o mybcp mybcp.c
  ...
