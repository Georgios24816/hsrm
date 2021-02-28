#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

bool isChildFinished(pid_t pid)
{
	int status = 0;
	pid_t ret = waitpid(pid, &status, WNOHANG);

	if (ret == pid)
	{
		return true;
	}

	return false;
}

void exitOnError(const char* errorMsg, char usePerror)
{
	if (usePerror > 0)
	{
		perror(errorMsg);
	}

	else
	{
		printf("Fehler : %s", errorMsg);
	}

	exit(EXIT_FAILURE);
}

/* Beachten Sie den geaenderten Prototypen von main(). Referenz: man 2 execve */
/* Sie koennen "envp" als Environment-Pointer an Kind-Prozesse weiterreichen. */
int main(int argc, char *argv[], char *envp[])
{
	printf("Elternprozess ...\n");

	pid_t child1 = fork();


	if (child1 < 0)
	{
		exitOnError("Fehler bei Kind1", 1);
	}
	
	else if (child1 == 0)
	{
		if (execve("./child1", argv, envp) == -1)
		{
			exitOnError("Fehler Kind1", 1); 
		}
	}
	
	else
	{
		//printf("Elternprozess : %d | Kind1 : %d\n", (int)getpid(), (int)child1);
		pid_t child2 = fork();

		if (child2 < 0)
		{
			exitOnError("Fehler bei Kind2", 1);
		}
		
		else if (child2 == 0)
		{
			if (execve("./child2", argv, envp) == -1)
			{
				exitOnError("Fehler Kind2", 1); 
			}
		}

		else
		{
			//printf("Elternprozess : %d | Kind2 : %d\n", (int)getpid(), (int)child2);

			for (size_t i = 0; i < 15; i++)
			{
				if (system("date") == -1)
				{
					exitOnError("system(date)", 1);
				}

				if (system("ps f") == -1)
				{
					exitOnError("system(ps f)", 1);
				}

				sleep(1);
			}

			size_t status = 0;

			while (status < 2)
			{
				if (isChildFinished(child1) == true)
				{
					printf("child1 fertig\n");
					status++;
				}

				if (isChildFinished(child2) == true)
				{
					printf("child2 fertig\n");
					status++;
				}
			}

			return EXIT_SUCCESS;
		}

	}

	return EXIT_SUCCESS;
}
