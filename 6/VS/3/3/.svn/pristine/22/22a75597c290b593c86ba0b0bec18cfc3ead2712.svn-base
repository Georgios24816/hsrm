#ifndef HAMSTERINTERFACE_H
#define HAMSTERINTERFACE_H

/**
 * @file	hamster.c
 * @brief	Hamsterverwaltungsprogramm
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h> //isdigit

#include "hamsterlib.h"

//routine used for handling error codes
bool errorCheck(int32_t retCode);

int32_t showAll(const char* owner);
int32_t collect(const char* owner);
int32_t giveTreats(const char* owner, const char* name, uint16_t treats);
int32_t newHamster(const char* owner, const char* name, uint16_t treats);
int32_t howsdoing(const char* owner, const char* name, struct hmstr_state* state);

bool isNumber(const char* str);

/**
 * @brief Main interface
int hamsterInterfaceHandler(int argc, char* argv[]);
 */


#endif