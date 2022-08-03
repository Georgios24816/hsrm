#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

typedef struct CmdArgs
{
	unsigned int cycleTime;
	unsigned int target;
	float p;
	float i;
	float d;
} CmdArgs_t;

volatile CmdArgs_t Config;

CmdArgs_t parseArgs(int argc, char* argv[])
{
	if (argc < 3)
	{
		assert(false);
	}

	CmdArgs_t ret;

	int index = 1;

	if (index + 2 <= argc && strcmp(argv[index], "-c") == 0)
	{
		ret.cycleTime = atoi(argv[++index]);
		index++;
	}

	else if (index + 2 <= argc && strcmp(argv[index], "-s") == 0)
	{
		ret.target = atoi(argv[++index]);
		index++;
	}

	else if (index + 2 <= argc && strcmp(argv[index], "-p") == 0)
	{
		ret.p = atof(argv[++index]);
		index++;
	}
	
	else if (index + 2 <= argc && strcmp(argv[index], "-i") == 0)
	{
		ret.i = atoi(argv[++index]);
		index++;
	}
	
	else if (index + 2 <= argc && strcmp(argv[index], "-d") == 0)
	{
		ret.d = atoi(argv[++index]);
		index++;
	}

	return ret;
}

void configureArgs()
{
	char c;
	
	switch (c)
	{
		case 'S': Config.target++; break;
		case 's': Config.target--; break;
		case 'P': Config.p++; break;
		case 'p': Config.p--; break;
		case 'I': Config.i++; break;
		case 'i': Config.i--; break;
		case 'D': Config.d++; break;
		case 'd': Config.d--; break;
	};
}

int main(int argc, char* argv[])
{
	Config = parseArgs(argc, argv);

	return 0;
}
