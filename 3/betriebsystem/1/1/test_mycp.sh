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

if [ ! -e mycp ]; then
	c_err
	echo "Fehler, das Programm 'mycp' existiert nicht."
	c_norm
	exit 1
fi

c_menu
echo "Kopiere '/bin/ls' zu 'datei1' in Haeppchen von 4096 Bytes"
echo "---[Ausgaben des Programms]-------------"
c_norm
./mycp /bin/ls datei1 4096
c_menu
echo "----------------------------------------"
c_norm

if [ ! -e datei1 ]; then
	c_err
	echo "Fehler, 'datei1' existiert nicht."
	c_norm
	exit 1
fi


c_menu
echo -n "Vergleiche Groesse der Dateien: "
c_norm
size1="$(wc -c </bin/ls)"
size2="$(wc -c <datei1)"
if [ $size1 -ne $size2 ]; then
	c_err
	echo "Fehler, '/bin/ls' ist $size1 Bytes gross,"
	echo "   aber 'datei1'  ist $size2 Bytes gross."
	c_norm
	exit 1
fi
c_ok
echo "OK"
c_norm


c_menu
echo -n "Vergleiche Inhalt der Dateien: "
c_norm
delta="$(diff /bin/ls datei1)" || true
if [ -n "$delta" ]; then
	c_err
	echo "Fehler, '/bin/ls' und 'datei1' weisen Unterschiede auf."
	c_norm
	exit 1
fi
c_ok
echo "OK"
c_norm


c_menu
echo -n "Extra: Vergleiche Zugriffsrechte der Dateien: "
c_norm
rwx1="$(ls -l /bin/ls|awk '{print $1}')"
rwx2="$(ls -l datei1|awk '{print $1}')"
if [ "$rwx1" != "$rwx2" ]; then
	c_err
	echo "Fehler, '/bin/ls' hat Zugriffsrechte $rwx1,"
	echo "   aber 'datei1'  hat Zugriffsrechte $rwx2."
	c_norm
	exit 1
fi
c_ok
echo "OK"
c_norm


c_menu
echo "Extra: Fehlermeldung bei nicht-regulaerer Datei: "
echo "---[Ausgaben des Programms]-------------"
c_norm
./mycp /bin datei2 4096 || true
c_menu
echo "----------------------------------------"
c_norm
if [ -e datei2 ]; then
	c_err
	echo "Fehler, 'datei2' wurde erstellt."
	c_norm
	exit 1
fi
c_warn
echo "(Ausgabe manuell pruefen)"
c_norm


# Aufraemen
rm -f datei1 datei2 datei3
trap - EXIT
