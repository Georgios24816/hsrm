Praktikum Betriebssysteme
==========================

Aufgabe 2 -- Prozesse
----------------------

Bitte benutzen Sie ausschliesslich die vorhandenen Dateien und benennen
Sie diese nicht um, da nur so eine schnelle Ueberpruefung der Abgaben
sichergestellt werden kann!


Verzeichnisstruktur
--------------------

- README.TXT            Diese Datei
- BEWERTUNG.TXT         Ihre Bewertung (vom Dozenten ausgefuellt)
- Makefile              Makefile

- parent.c              Quellcode Elternprozess
- child1.c              Quellcode Kindprozess 1
- child2.c              Quellcode Kindprozess 2
- mytimes.c             Quellcode Zeitmessung

- cleanup.sh            Aufraeum-Skript


Hinweise
---------

Sie koennen abgestuerzte Prozesse mittels des Skripts cleanup.sh beenden:

  $ ./cleanup.sh
  parent: Kein Prozess gefunden
  mytime: Kein Prozess gefunden
  [1]-  Getoetet               ./child2
  [2]+  Getoetet               ./child1
