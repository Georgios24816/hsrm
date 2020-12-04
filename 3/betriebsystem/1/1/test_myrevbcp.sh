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
echo -n "Hallo, rueckwaerts" > datei1
echo -n "streawkceur ,ollaH" > datei3

if [ ! -e myrevbcp ]; then
	c_err
	echo "Fehler, das Programm 'myrevbcp' existiert nicht."
	c_norm
	exit 1
fi

c_menu
echo "Kopiere 'datei1' zu 'datei2' (rueckwaerts)"
echo "---[Ausgaben des Programms]-------------"
c_norm
./myrevbcp datei1 datei2
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
echo -n "Vergleiche Inhalt 'datei2' zu 'datei3' (Vergleichsmuster): "
c_norm
delta="$(diff datei2 datei3)" || true
if [ -n "$delta" ]; then
	c_err
	echo "Fehler, 'datei1' und 'datei3' weisen Unterschiede auf."
	c_norm
	exit 1
fi
c_ok
echo "OK"
c_norm


# Aufraemen
rm -f datei1 datei2 datei3
trap - EXIT
