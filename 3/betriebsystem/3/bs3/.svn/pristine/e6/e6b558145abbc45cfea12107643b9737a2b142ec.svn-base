#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

/* Warten von 1 bis 10000 Millisekunden (10 Sekunden) */
void msleep(unsigned int ms)
{
	struct timespec ts;

	assert(ms > 0);
	assert(ms <= 10000);

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms * 1000000) % 1000000000;
	nanosleep(&ts, NULL);
}

int main(int argc, char *argv[])
{
	printf("gibaus_mutex.c\n");

	return EXIT_SUCCESS;
}
