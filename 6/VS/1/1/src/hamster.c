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

/**
 * @brief Kurze Hilfe ausgeben
 */
static void rtfm(char* argv[])
{
	printf("Usage: %s {<Option>} <param1> {<param2>}\n", argv[0]);
	printf("Function: Hamster management\n");
	printf("Optionen:\n");
	printf("     -l {<owner>}                    - show current list of hamsters\n");
	printf("     -n <owner> <hamster> [<treats>] - add new hamster\n");
	printf("     -f <owner> <hamster> <treats>   - feed treats to hamster\n");
	printf("     -s <owner> <hamster>            - how is my hamster doing?\n");
	printf("     -b <owner>                      - the bill please!\n");
}

/**
 * @brief: routine used for handling error codes
 *
 */
void errorCheck(int32_t retCode)
{
	if (retCode >= 0) //no error, exit function
	{
		return;
	}

	if (retCode == HMSTR_ERR_NAMETOOLONG)
	{
		printf("Error: HMSTR_ERR_NAMETOOLONG\n");
	}

	else if (retCode == HMSTR_ERR_NOTFOUND)
	{
		printf("Error: HMSTR_ERR_NOTFOUND\n");
	}

	else if (retCode == HMSTR_ERR_CORRUPT)
	{
		printf("Error: HMSTR_ERR_CORRUPT\n");
	}

	else if (retCode == HMSTR_ERR_STORE)
	{
		printf("Error: HMSTR_ERR_STORE\n");
	}

	else if (retCode == HMSTR_ERR_EXISTS)
	{
		printf("Error: HMSTR_ERR_EXISTS\n");
	}

	else
	{
		printf("Error: UNKNOW ERROR\n");
	}

	exit(EXIT_FAILURE);
}


/**
 * @brief: show all hamsters depending on owner
 *
 */
void showAll(char* owner)
{
	int32_t entry = -1;
	int32_t entryCount = -1;
	int32_t treatsLeft = 0;
	
	char ownerBuffer[HMSTR_MAX_NAME] = { 0 };
	char name[HMSTR_MAX_NAME] = { 0 };

	int16_t price;

	uint32_t index = 0;
	
	while (entryCount != HMSTR_ERR_NOTFOUND)
	{
		entryCount = hmstr_directory(&entry, owner, NULL);

		if (entryCount == HMSTR_ERR_NOTFOUND)
		{
			if (index == 0)
			{
				printf("No hamsters found\n");
			}

			return; //iterate the hamsters until no more hamster are left
		}

		else
		{
			errorCheck(entryCount);
		}

		index++;

		treatsLeft = hmstr_readentry(entryCount, ownerBuffer, name, &price);
		errorCheck(treatsLeft);

		printf("owner: %s name: %s price: %d treats left: %d\n", ownerBuffer, name, price, treatsLeft);
	}
}
/**
 * @brief: owner can collect hamsters
 */
void collect(const char* owner)
{
	if (!owner)
	{
		printf("No owner specified\n");
		return;
	}

	int32_t price = hmstr_collect(owner);
	errorCheck(price);

	printf("owner %s has to pay %d â‚¬\n", owner, price);
}

/**
 * @brief: owner can give treats to their hamster
 *
 *
 */
void giveTreats(const char* owner, const char* name, uint16_t treats)
{
	if (!owner || !name)
	{
		printf("no owner or hamster specified\n");
		return;
	}

	if (treats == 0)
	{
		printf("you have to give at least 1 treat\n");
		return;
	}

	int32_t hamster = hmstr_lookup(owner, name);
	errorCheck(hamster);

	int32_t retCode = hmstr_givetreats(hamster, treats);
	errorCheck(retCode);

	printf("treats : %d\n", hamster);
}

/**
 * @brief: register a new hamster-owner pair in the db
 */
void newHamster(const char* owner, const char* name, uint16_t treats)
{
	if (!owner || !name)
	{
		printf("no owner or hamster specified\n");
		return;
	}

	int32_t retCode = hmstr_new(owner, name, treats);
	errorCheck(retCode);
}

/**
 * @brief: get hamster status
 *
 */
void howsdoing(const char* owner, const char* name)
{
	if (!owner || !name)
	{
		printf("no owner or hamster specified\n");
		return;
	}

	int32_t hamster = hmstr_lookup(owner, name);
	errorCheck(hamster);

	struct hmstr_state state;

	int32_t retCode = hmstr_howsdoing(hamster, &state);
	errorCheck(retCode);

	printf("owner: %s hamster: %s treats: %d; rounds: %d; cost: %d\n", owner, name, (int)state.treats_left, state.rounds, (int)state.cost);
}

/**
 * brief: check if a string is a number
 */
bool isNumber(const char* str)
{
	for (size_t i = 0; str[i] != 0; i++)
	{
		if (!isdigit(str[i]))
		{
			return false;
		}
	}

	return true;
}

/**
 * @brief Main program
 */
int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		if (strcmp(argv[1], "-l") != 0)
		{
			rtfm(argv);
			return 0;
		}

		showAll(NULL);
	}

	else if (argc == 3)
	{
		if (strcmp(argv[1], "-l") == 0)
		{
			showAll(argv[2]);
		}

		else if (strcmp(argv[1], "-b") == 0)
		{
			collect(argv[2]);
		}

		else
		{
			rtfm(argv);	
		}
	}

	else if (argc == 4)
	{
		if (strcmp(argv[1], "-n") == 0)
		{
			newHamster(argv[2], argv[3], 0);
		}

		else if (strcmp(argv[1], "-s") == 0)
		{
			howsdoing(argv[2], argv[3]);
		}

		else
		{
			rtfm(argv);
		}
	}

	else if (argc == 5)
	{
		uint16_t treatCount = (uint16_t)strtoul(argv[4], NULL, 10);
		if (!isNumber(argv[4]))
		{
			printf("arg[4] should be a number\n");
			rtfm(argv);
			exit(EXIT_FAILURE);
		}
		
		if (strcmp(argv[1], "-n") == 0)
		{
			newHamster(argv[2], argv[3], treatCount);
		}

		else if (strcmp(argv[1], "-f") == 0)
		{
			giveTreats(argv[2], argv[3], treatCount);
		}

		else
		{
			rtfm(argv);
		}
	}

	else
	{
		rtfm(argv);
	}
	
	return 0;
}
