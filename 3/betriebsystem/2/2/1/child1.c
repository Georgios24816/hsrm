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
	printf("Kind_1 Start: %d %d\n", getpid(), getppid());
	sleep(10);

	return EXIT_SUCCESS;
}
