#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

off_t fileSize(int fd)
{
	off_t oldPos = lseek(fd, 0, SEEK_CUR);

	off_t start = lseek(fd, 0, SEEK_SET);
	off_t end = lseek(fd, 0, SEEK_END);

	lseek(fd, oldPos, SEEK_CUR);

	return (end - start);
}

int main(int argc, char *argv[])
{
	printf("Die Laenge der Datei ist ...\n");

	if (argc != 2)
	{
		printf("Ein Dateiname muss angegeben werden\n");
		return EXIT_FAILURE;
	}

	int file = open(argv[1], O_RDONLY);

	if (file == -1)
	{
		perror("Datei konnte nicht geoeffnet werden");
		return EXIT_FAILURE;
	}

	printf("%d\n", (int) fileSize(file));

	return EXIT_SUCCESS;
}
