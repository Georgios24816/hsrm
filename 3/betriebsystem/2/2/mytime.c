#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

/* Beachten Sie den geaenderten Prototypen von main(). Referenz: man 2 execve */
/* Sie koennen "envp" als Environment-Pointer an Kind-Prozesse weiterreichen. */
int main(int argc, char *argv[], char *envp[])
{
	/*
	printf("mytime ...\n");

	if (argc < 2)
	{
		printf("Nicht genug Argumente\n");
		exit(EXIT_FAILURE);
	}
	
	struct timespec start, end;
	
	if (clock_gettime(CLOCK_REALTIME, &start) == -1)
	{
		perror("Fehler bei gettime(start)");
		exit(EXIT_FAILURE);
	}
	
	pid_t child = fork();
	
	if (child == 0) //child
	{
		if (execve(argv[1], &argv[1], envp) == -1)
		{
			perror("Fehler bei execve");
			exit(EXIT_FAILURE);
		}	
	}
	
	else if (pid < 0) //error
	{
		perror("Kindrozess nicht erstellt");
		exit(EXIT_FAILURE);
	}
	
	else //parent
	{
		
	}
	
	if (clock_gettime(CLOCK_REALTIME, &end) == -1)
	{
		perror("Fehler bei gettime(end)");
		exit(EXIT_FAILURE);
	}
	*/
	return EXIT_SUCCESS;
}
