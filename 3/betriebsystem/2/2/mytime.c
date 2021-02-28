#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void printOnFail(const char* s)
{
	printf("Fehler : %s\n", s);
	exit(EXIT_FAILURE);
}

void perrorOnFail(const char* s)
{
	perror(s);
	exit(EXIT_FAILURE);
}

int waitChild(pid_t pid)
{
	int status = 0;

	while (1)
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			perrorOnFail("waitpid : ");	
		}

		if (WIFEXITED(status))
		{
			return (int) WEXITSTATUS(status);
		}
	}
	
}

/* Beachten Sie den geaenderten Prototypen von main(). Referenz: man 2 execve */
/* Sie koennen "envp" als Environment-Pointer an Kind-Prozesse weiterreichen. */
int main(int argc, char *argv[], char *envp[])
{
	if (argc < 2)
	{
		printOnFail("Nicht genug Argumente\n");
	}
	
	pid_t child = fork();
	
	if (child < 0) //error
	{
		perrorOnFail("Kindrozess nicht erstellt");
	}

	else if (child == 0) //child
	{
		if (execve(argv[1], &argv[1], envp) == -1)
		{
			perrorOnFail("Fehler bei execve");
		}	
	}
	
	else //parent
	{
		struct timespec start, end;

		if (clock_gettime(CLOCK_REALTIME, &start) == -1)
		{
			perrorOnFail("Fehler bei gettime(start)");
		}
		
		int childRet = waitChild(child);

		if (clock_gettime(CLOCK_REALTIME, &end) == -1)
		{
			perrorOnFail("Fehler bei gettime(end)");
			exit(EXIT_FAILURE);
		}

		time_t seconds = end.tv_sec - start.tv_sec;
		long nseconds = end.tv_nsec - start.tv_nsec;

		unsigned long long microseconds = (seconds * 1000000) + (nseconds / 1000);

		printf("Zeit : %llu Microsekunden\nExit : %d\n", microseconds, childRet);
	}

	return EXIT_SUCCESS;
}

/* Ausführungszeiten in Mikrosekunden
3005513
3005669
3005922
3005713
3005521
3005956

Die Zeiten schwanken, weil das Betriebssystem auch andere Prozesse schedulen muss und auf Anfragen von anderen Prozessen (die vielleicht höhere Prioritäten haben)
schnell antworten muss.
*/
