#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <inttypes.h>

off_t fileSize(int file)
{
	off_t start = lseek(file, 0, SEEK_SET);
	off_t end = lseek(file, 0, SEEK_END);

	lseek(file, 0, SEEK_SET);

	return (end - start);
}

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

	if (argc != 4)
	{
		perror("die beiden Dateinamen und Puffergroesse, muessen als Kommandozeilenparameter uebergeben werden");
		return EXIT_FAILURE;
	}

	char* inFilename = argv[1];
	char* outFilename = argv[2];
	ssize_t bufferSize = (ssize_t) strtol(argv[3], NULL, 10);

	if (bufferSize == 0)
	{
		perror("Puffergroesse ungueltig");
		return EXIT_FAILURE;
	}

	int inFp = open(inFilename, O_RDONLY);

	if (inFp == -1)
	{
		perror("Eingabedatei konnte nicht gelesen werden");
		return EXIT_FAILURE;
	}

	struct stat inStatStruct;

	if (stat(inFilename, &inStatStruct) == -1)
	{
		perror("Stats(Eingabedatei) konnten nicht gelesen werden");
		return EXIT_FAILURE;
	}

	if (!S_ISREG(inStatStruct.st_mode))
	{
		perror("keine regulaere Eingabedatei");
		return EXIT_FAILURE;
	}

	int outFp = creat(outFilename, S_IRUSR | S_IWUSR);

	if (outFp == -1)
	{
		perror("Ausgabedatei konnte nicht gelesen werden");
		closeFile(inFp);
		return EXIT_FAILURE;
	}

	struct stat outStatStruct;
	
	if (stat(outFilename, &outStatStruct) == -1)
	{
		perror("Stats(Ausgabedatei) konnten nicht gelesen werden");
		return EXIT_FAILURE;
	}

	if (!S_ISREG(outStatStruct.st_mode))
	{
		perror("keine regulaere Ausgabedatei");
		return EXIT_FAILURE;
	}

	if (chmod(outFilename, inStatStruct.st_mode) == -1)
	{
		perror("Rechte konnten nicht gesetzt werden");
		return EXIT_FAILURE;
	}

	ssize_t inFileSize = (ssize_t)fileSize(inFp);

	if (inFileSize > 0)
	{

		while (inFileSize > 0)
		{
			size_t curBufferSize = (bufferSize < inFileSize) ? bufferSize : inFileSize;

			char inBuffer[curBufferSize];

			ssize_t readInFp = read(inFp, inBuffer, curBufferSize);

			if (readInFp != (ssize_t)curBufferSize)
			{
				perror("Nicht alles Bytes konnten gelesen werden");
				
				closeFile(inFp);
				closeFile(outFp);
				
				return EXIT_FAILURE;
			}

			ssize_t writeOutFp = write(outFp, inBuffer, curBufferSize);

			if (writeOutFp != (ssize_t)curBufferSize)
			{
				perror("Nicht alle Bytes konnten geschrieben werden");

				closeFile(inFp);
				closeFile(outFp);

				return EXIT_FAILURE;
			}

			inFileSize -= curBufferSize;
		}
	}

	closeFile(inFp);
	closeFile(outFp);

	return EXIT_SUCCESS;
}
