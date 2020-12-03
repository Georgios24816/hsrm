#!/bin/bash

set -e

function c_norm() { echo -n -e "\e[0m"; }
function c_menu() { echo -n -e "\e[34;1m"; }
function c_err()  { echo -n -e "\e[31;1m"; }
function c_ok()   { echo -n -e "\e[32;1m"; }
function c_warn() { echo -n -e "\e[35;1m"; }
function clean_up { c_err; echo "Abbruch"; c_norm; exit 0; }
trap clean_up EXIT

# Vorbereitungen: alte Testdateien loeschen
rm -f datei1 datei2 datei3
echo "Hallo, Betriebssystemepraktikum!" > datei1

if [ ! -e filelength ]; then
	c_err
	echo "Fehler, das Programm 'filelength' existiert nicht."
	c_norm
	exit 1
fi

c_menu
echo "Pruefe Groesse von 'datei1': "
echo "---[Ausgaben des Programms]-------------"
c_norm
./filelength datei1 | tee datei2
c_menu
echo "----------------------------------------"
c_norm
if [ ! -e datei2 ]; then
	c_err
	echo "Fehler, 'filelength' hat keine Ausgaben produiert."
	c_norm
	exit 1
fi

size1="$(wc -c <datei1)"
size2="$(grep "$size1" datei2)" || true
if [ -z "$size2" ]; then
	c_err
	echo "Fehler: Die Zahl '$size1' wurde in der Ausgabe erwartet"
	c_norm
	exit 1
fi
c_ok
echo "OK"
c_norm


# Aufraemen
rm -f datei1 datei2 datei3
trap - EXIT
