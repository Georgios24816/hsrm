#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

void closeFile(int f1)
{
	if (close(f1) != 0)
	{
		perror("Datei konnte nicht geschlossen werden");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char *argv[])
{
	printf("Hallo, Praktikum!\n");
	printf("Kopiere Datei byteweise ...\n");

	if (argc != 3)
	{
		printf("die beiden Dateiname, muessen als Kommandozeilenparameter uebergeben werden");
		return EXIT_FAILURE;
	}

	char* inFilename = argv[1];
	char* outFilename = argv[2];

	int inFp = open(inFilename, O_RDONLY);

	if (inFp == -1)
	{
		perror("Eingabedatei konnte nicht gelesen werden");
		return EXIT_FAILURE;
	}

	int outFp = open(outFilename, O_WRONLY | O_APPEND);

	if (outFp == -1)
	{
		perror("Ausgabedatei konnte nicht gelesen werden");
		return EXIT_FAILURE;
	}

	char inBuffer;

	while (read(inFp, &inBuffer, 1) == 1)
	{
		ssize_t writeOutFp = write(outFp, &inBuffer, 1);

		if (writeOutFp != 1)
		{
			printf("Nicht alle Bytes konnten geschrieben werden");
			return EXIT_FAILURE;
		}
	}

	closeFile(inFp);
	closeFile(outFp);

	return EXIT_SUCCESS;
}
