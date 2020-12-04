#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <inttypes.h>

void closeFile(int fp)
{
	if (close(fp) == -1)
	{
		perror("Datei konnte nicht geschlossen werden");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char *argv[])
{
	printf("Dateigroesse setzen ...\n");

	if (argc != 3)
	{
		printf("2 Kommandozeilenparameter werden benoetigt");
		return EXIT_FAILURE;
	}


	char* filename = argv[1];
	off_t length = (off_t) strtoumax(argv[2], NULL, 10);
	
	int fp = open(filename, O_RDONLY);
	
	if (fp == -1)
	{
		printf("Datei existiert nicht und wird neu erstellt\n");
		fp = creat(filename, S_IRUSR | S_IWUSR);
	}

	if (fp == -1)
	{
		perror("Datei konnte nicht erstellt werden");
		return EXIT_FAILURE;
	}

	if (truncate(filename, length) == -1)
	{
		perror("Dateigroesse konnte nicht gesetzt werden");
		close(fp);
		return EXIT_FAILURE;
	}

	closeFile(fp);

	return EXIT_SUCCESS;
}
