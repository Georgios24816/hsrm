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

if [ ! -e grow ]; then
	c_err
	echo "Fehler, das Programm 'grow' existiert nicht."
	c_norm
	exit 1
fi

echo "Erzeuge neue Datei 'datei1' von 42 Bytes Groesse: "
c_menu
echo "---[Ausgaben des Programms]-------------"
c_norm
./grow datei1 42
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
echo -n "Pruefe Groesse der Datei, erwartet 42 Bytes: "
c_norm
size1="$(wc -c <datei1)"
if [ $size1 -ne 42 ]; then
	c_err
	echo "Fehler, 'datei1' ist $size1 Bytes gross."
	c_norm
	exit 1
fi
c_ok
echo "OK"
c_norm


c_menu
echo "Vergroessere vorhandene Datei 'datei1' auf 1234 Bytes Groesse: "
echo "---[Ausgaben des Programms]-------------"
c_norm
./grow datei1 1234
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
echo -n "Pruefe Groesse der Datei, erwartet 1234 Bytes: "
c_norm
size1="$(wc -c <datei1)"
if [ $size1 -ne 1234 ]; then
	c_err
	echo "Fehler, 'datei1' ist $size1 Bytes gross."
	c_norm
	exit 1
fi
c_ok
echo "OK"
c_norm


c_menu
echo -n "Teste Zugriffsrechte der Datei: "
c_norm
rwx1="$(ls -l datei1|awk '{print $1}')"
rwx2="-rw-------"
if [ "$rwx1" != "$rwx2" ]; then
	c_err
	echo "Fehler, 'datei1' hat Zugriffsrechte $rwx1,"
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
