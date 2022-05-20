#include "hamsterInterface.h"
#include <stdbool.h>

bool errorCheck(int32_t retCode)
{
	return (bool)(retCode < 0);

	/*
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

	exit(EXIT_FAILURE);
	*/
}

int32_t showAll(const char* owner)
{
    int32_t entry = -1;
	int32_t entryCount = -1;
	int32_t treatsLeft = 0;
	
	char ownerBuffer[HMSTR_MAX_NAME] = { 0 };
	char name[HMSTR_MAX_NAME] = { 0 };

	int16_t price;
	
	while (entryCount != HMSTR_ERR_NOTFOUND)
	{
		entryCount = hmstr_directory(&entry, owner, NULL);

		if (entryCount == HMSTR_ERR_NOTFOUND)
		{
			return 0; //iterate the hamsters until no more hamster are left
		}

		else
		{
			if (errorCheck(entryCount))
			{
				return false;
			}
		}

		treatsLeft = hmstr_readentry(entryCount, ownerBuffer, name, &price);
		errorCheck(treatsLeft);

		printf("owner: %s name: %s price: %d treats left: %d\n", ownerBuffer, name, price, treatsLeft);
	}

	return true;
}

int32_t collect(const char* owner)
{
	if (!owner)
	{
		printf("No owner specified\n");
		return false;
	}

	int32_t price = hmstr_collect(owner);
	
	if (errorCheck(price))
	{
		return false;
	}

	printf("owner %s has to pay %d â‚¬\n", owner, price);
	return true;
}

int32_t giveTreats(const char* owner, const char* name, uint16_t treats)
{
    if (!owner || !name)
	{
		printf("no owner or hamster specified\n");
		return false;
	}

	if (treats == 0)
	{
		printf("you have to give at least 1 treat\n");
		return false;
	}

	int32_t hamster = hmstr_lookup(owner, name);
	
	if (errorCheck(hamster))
	{
		return false;
	}

	int32_t retCode = hmstr_givetreats(hamster, treats);
	
	if (errorCheck(retCode))
	{
		return false;
	}

	return true;
}

int32_t newHamster(const char* owner, const char* name, uint16_t treats)
{
	return hmstr_new(owner, name, treats);
}

int32_t howsdoing(const char* owner, const char* name, struct hmstr_state* state)
{
	int32_t hamster = hmstr_lookup(owner, name);
	
	if (errorCheck(hamster))
	{
		return hamster;
	}

	return hmstr_howsdoing(hamster, state);
}

bool isNumber(const char* str)
{
    if (!str)
    {
        return false;
    }

    for (size_t i = 0; str[i] != 0; i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }

    return true;
}

/*
int hamsterInterfaceHandler(int argc, char* argv[])
{
	if (argc == 2)
	{
		if (strcmp(argv[1], "-l") != 0)
		{
			//rtfm(argv);
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
			//rtfm(argv);	
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
			//rtfm(argv);
		}
	}

	else if (argc == 5)
	{
		uint16_t treatCount = (uint16_t)strtoul(argv[4], NULL, 10);
		if (!isNumber(argv[4]))
		{
			printf("arg[4] should be a number\n");
			//rtfm(argv);
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
			//rtfm(argv);
		}
	}

	else
	{
		//rtfm(argv);
	}
	
	return 0;
}
*/