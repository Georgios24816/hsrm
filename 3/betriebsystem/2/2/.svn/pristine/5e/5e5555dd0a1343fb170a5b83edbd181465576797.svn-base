#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	printf("Kind_2 ...\n");

	printf("Kind_2 : %d | Elternprozess : %d\n", (int)getpid(), 1);
	
	for (volatile size_t i = 0; i < 1000000; i++)
	{
		for (volatile size_t f = 0; f < 8000; f++)
		{
		}
	}
	
	printf("Kind2 Ende\n");

	return EXIT_SUCCESS;
}
