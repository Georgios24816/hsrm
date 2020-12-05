#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	static volatile int status = 0;
	printf("Kind_2 Start: %d %d\n", getpid(), getppid());

	for(int i = 0; i < 7; i++)
	{
		while(status < 1000000000)
			status++;
		status = 0;
	}

	return EXIT_SUCCESS;
}
