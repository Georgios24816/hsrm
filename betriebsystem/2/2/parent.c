#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Beachten Sie den geaenderten Prototypen von main(). Referenz: man 2 execve */
/* Sie koennen "envp" als Environment-Pointer an Kind-Prozesse weiterreichen. */
int main(int argc, char *argv[], char *envp[])
{
	printf("Elternprozess ...\n");

	pid_t children[2];
	child[0] = fork();

	if (child[0] < 0)
	{
		perror("Fehler bei Kind1");
		exit(EXIT_FAILURE);
	}

	printf("Elternprozess : %d\nKind1 : %d\nKind2 : %d\n", (int)getpid(), (int)children[0]);

	return EXIT_SUCCESS;
}
