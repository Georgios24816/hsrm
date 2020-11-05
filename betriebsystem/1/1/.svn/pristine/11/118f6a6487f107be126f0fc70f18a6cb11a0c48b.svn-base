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

if [ ! -e mybcp ]; then
	c_err
	echo "Fehler, das Programm 'mybcp' existiert nicht."
	c_norm
	exit 1
fi

c_menu
echo "Kopiere 'datei1' zu 'datei2'"
echo "---[Ausgaben des Programms]-------------"
c_norm
./mybcp datei1 datei2
c_menu
echo "----------------------------------------"
c_norm
if [ ! -e datei2 ]; then
	c_err
	echo "Fehler, 'datei2' existiert nicht."
	c_norm
	exit 1
fi


c_menu
echo -n "Vergleiche Groesse der Dateien: "
c_norm
size1="$(wc -c <datei1)"
size2="$(wc -c <datei2)"
if [ $size1 -ne $size2 ]; then
	c_err
	echo "Fehler, 'datei1' ist $size1 Bytes gross,"
	echo "   aber 'datei2' ist $size2 Bytes gross."
	c_norm
	exit 1
fi
c_ok
echo "OK"
c_norm


c_menu
echo -n "Vergleiche Inhalt der Dateien: "
c_norm
delta="$(diff datei1 datei2)" || true
if [ -n "$delta" ]; then
	c_err
	echo "Fehler, 'datei1' und 'datei2' weisen Unterschiede auf."
	c_norm
	exit 1
fi
c_ok
echo "OK"
c_norm


c_menu
echo -n "Teste Zugriffsrechte der Datei: "
c_norm
rwx1="$(ls -l datei2|awk '{print $1}')"
rwx2="-rw-------"
if [ "$rwx1" != "$rwx2" ]; then
	c_err
	echo "Fehler, 'datei2' hat Zugriffsrechte $rwx1,"
	echo "  aber es wurden die Zugriffsrechte $rwx2 erwartet."
	c_norm
	exit 1
fi
c_ok
echo "OK"
c_norm


# Aufraemen
rm -f datei1 datei2 datei3
trap - EXIT
