#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>

#define THREAD_COUNT 6

pthread_t threads[THREAD_COUNT];
int threadOut[THREAD_COUNT] = {1, 2, 3, 4, 5, 6};

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

void* threadFunc(void* arg)
{
	assert(arg);
	
	for (size_t lines = 0; lines < 5; lines++)
	{
		for (size_t charCount = 0; charCount < 10; charCount++)
		{
			printf("%d", *((int*)arg));
			fflush(stdout);
			
			unsigned int sleepMs = 101 + rand() % 500; //101 - 499
			msleep(sleepMs);
		}
	}
	
	return NULL;
}

void createThread(size_t i)
{
	assert(i < THREAD_COUNT);
	
	pthread_t tid;
	int ret = pthread_create(&tid, NULL, threadFunc, (void*) &threadOut[i]);
	
	assert(ret == 0);
	threads[i] = tid;
}

void joinThread(size_t i)
{
	assert(i < THREAD_COUNT);

	int ret = pthread_join(threads[i], NULL);
	assert(ret == 0);
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	printf("gibaus.c\n");
	
	for (size_t i = 0; i < THREAD_COUNT; i++)
	{
		createThread(i);
	}

	for (size_t i = 0; i < THREAD_COUNT; i++)
	{
		joinThread(i);
	}
	
	return EXIT_SUCCESS;
}

/*
Die Ausgaben der Threads sind durcheinander, weil sie nicht synchronisiert sind
*/
