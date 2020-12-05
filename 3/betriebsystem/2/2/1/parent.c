#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Beachten Sie den geaenderten Prototypen von main(). Referenz: man 2 execve */
/* Sie koennen "envp" als Environment-Pointer an Kind-Prozesse weiterreichen. */
int main(int argc, char *argv[], char *envp[])
{
	pid_t kind1;
	pid_t kind2;
	pid_t beendet;
	printf("Jetzt Forken\n");

	kind1 = fork();
	if (kind1 < 0)
	{
		perror("Fehler bei Fork von kind1");
		return EXIT_FAILURE;
	}
	else if (kind1 == 0)
	{
		execl("./child1", "child1", NULL);
	}

	kind2 = fork();
	if (kind2 < 0)
	{
		perror("Fehler bei Fork von kind2");
		return EXIT_FAILURE;
	}
	else if (kind2 == 0)
	{
		execl("./child2", "child2", NULL);
	}

	printf("Elternprozess: %d, kind1: %d, kind2: %d\n", getpid(), kind1, kind2);

	for (int i = 0; i < 15; i++)
	{
		sleep(1);
		system("date");
		system("ps f");
	}

	for (int i = 0; i < 2; i++)
	{
		beendet = wait(NULL);
		if (beendet == kind1)
		{
			printf("Kind1 beendet\n");
			system("date");
		}
		else if (beendet == kind2)
		{
			printf("Kind2 beendet\n");
			system("date");
		}
		else
			printf("anderer Prozess %d beendet\n", beendet);
	}

	/*Aufgabe 1B
	Das Programm fängt mit dem kind2 an und bekommt den Status Run, 
	während parent und kind1 den Status Waiting for event. Danach 
	wechselt kind1 den Status zu zombie. Zuletzt wechselt dann kind2 den
	Status zu zombie.
	Ich denke, wenn der Prozess in dem sleep() Modus ist, dann bekommt
	er den Status Waiting for event. Während kind2 arbeitet in seinem 
	Process. Also wäre kind1 eigentlich auch im Status Run, aber er ist 
	im sleep Modus und deshalb bekommt er den Status Waiting for event.
	*/
	return EXIT_SUCCESS;
}
