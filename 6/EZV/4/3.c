/* *
 * Hochschule RheinMain
 * Fachbereich DCSM
 * Studienbereich Informatik
 *
 * Lehrveranstaltung Echzeitverarbeitung
 *
 * Sommersemester 2021
 *
 * Programm-Stub fuer Antwortzeit-Jitter-Messungen von Posix-Threads.
 *
 * Uebersetzen mit:
 * gcc -std=c99 -Wall PROGRAMMNAME.c -o PROGRAMMNAME -lpthread -D_GNU_SOURCE 
 *
 * Stub von kai.beckmann@hs-rm.de
 *
 * Anleihen aus den Aufgabe des PRLT - Echtzeit - Praktikum des Institut fuer
 * Automatisierungstechnik von der Technischen Universitaet Dreden
 */
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <argp.h>
#include <stdbool.h>
#include <string.h>


struct timerMessStruct {
	int cycletime;
	int worktime;
	int workdeviation;
	char name[20];
	int iterations;
	int id;
	int sched_type;
	int prio;
};

long calcDiff(struct timeval* start, struct timeval* stop) {
	return (((stop->tv_sec - start->tv_sec) * 1000000) + (stop->tv_usec - start->tv_usec));
}
// Simuliert Arbeit. Parameter als Microsekunden
void doWork(int worktime, int workdeviation) {

	struct timeval start, stop;

	int workLength = (random() % workdeviation) + worktime;

	gettimeofday(&start, NULL);

	do {
		gettimeofday(&stop, NULL);
	} while (calcDiff(&start, &stop) < workLength);

	return ;

}

// globale Datenstruktur fuer die Response-Time-Messungen
struct timeval** responseTimeArray;

// Berechung und Ausgabe der Response-Jitter-Werte
// Parameter:
// 		id			ID des Threads
// 		iteration	Anzahl der Iterationen der Messung (groesse des Arrays)
void printResult(int id, int iteration) {

	int relativeJitter = 0;
	int absolutJitter = 0;
	int maxResponseTime = 0;
	int minResponseTime = INT_MAX;

	for (int i = 0; i < iteration; i++) {

		int usec = responseTimeArray[id][i].tv_usec;
		if (usec > 2 && usec < minResponseTime) {
			minResponseTime = usec;
		}
		if (usec > maxResponseTime) {
			maxResponseTime = usec;
		}

		// abbruch, wenn letzter Wert erreicht (t+1 wuerde sonst ueber das Array 
		// hinausgehen)
		if (i + 1 >= iteration)
			continue;

		// Berechnung des Jitters in Verhaeltnis zum naechsten Wert
		unsigned int jitter = abs( (responseTimeArray[id][i+1].tv_usec - usec ));

//		printf("jitter %u - t: %d t+1 %d \n", jitter, usec, 
//		responseTimeArray[0][i+1].tv_usec);

	// MAX Jitter	
		if (jitter > relativeJitter) 
			relativeJitter = jitter;
	}

	absolutJitter = maxResponseTime - minResponseTime;
	printf("Relativer Jitter %d, absoluter Jitter %d maximaler Jitter %d minimaler Jitter %d \n", 
			relativeJitter, absolutJitter, maxResponseTime, minResponseTime);
}


void* threadFunction(void* d);

struct timeval startZeit;

const char* argp_program_version = "respJitterMessurement 1.0";
const char* argp_program_bug_address = "kai.beckmann@hs-rm.de";
static char doc[] = "todo documentation";
static char args_doc[] = "doo";

static struct argp_option options[] = {
		{"verbose",					'v', 0, 				0, 	"Verbose/Debug Output"},
		{"iterations",				'i', "ITERATIONS", 		0, "Anzahl Interationen"},
		{"sched-policy-round-robin", 'r',0, 				0, "Realtime Scheduling Policy Round Robin"},
	    {"sched-policy-fifo",		'f', 					0, 0, "Realtime Scheduling Policy FIFO"},
		{"prio-thread1",			'p', "PRIO-THREAD1", 	0, "PrioritÃt des Thread 1"},
		{"prio-thread2", 			'P', "PRIO-THREAD2", 	0, "PrioritÃt des Thread 2"},
		{"cycle-thread1",			'c', "CYClE-THREAD1", 	0, "Zyklusdauer von Thread 1"},
		{"cycle-thread2", 			'C', "CYCLE-THREAD2", 	0, "Zyklusdauer von Thread 2"},
		{"work-load-thread1",		'w', "WORK-THREAD1", 	0, "Dauer der Arbeit von Thread 1 in us"},	
		{"work-load-thread2",		'W', "WORK-THREAD2", 	0, "Dauer der Arbeit von Thread2 in us"},	
		{"work-dev-thread1",		'd', "WORK-DEV-THREAD1", 0, "Variation der Arbeitsdauer von Thread1 in us"},	
		{"work-dev-thread2",		'D', "WORK-DEV-THREAD2", 0, "Variation der Arbeitsdauer von Thread2 in us"},	
		{0}
};

struct arguments {
	int verbose;
	int iterations;
	int scheduler;
	int prioT1;
	int prioT2;
	int cycleT1;
	int cycleT2;
	int workT1;
	int workT2;
	int workDevT1;
	int workDevT2;

};

static error_t parse_opt(int key, char* arg, struct argp_state* state) {
	struct arguments* arguments = state->input;

	switch (key) {
		case 'v' : arguments->verbose = 1;
				   break;
		case 'i' : arguments->iterations = atoi(arg); // don't use atoi, it's not save!
				   break;
		case 'r' : arguments->scheduler = SCHED_RR; break;
		case 'f' : arguments->scheduler = SCHED_FIFO; break;
		case 'p' : arguments->prioT1 = atoi(arg); break;
		case 'P' : arguments->prioT2 = atoi(arg); break;
		case 'c' : arguments->cycleT1 = atoi(arg); break;
		case 'C' : arguments->cycleT2 = atoi(arg); break;
		case 'w' : arguments->workT1 = atoi(arg); break;
		case 'W' : arguments->workT2 = atoi(arg); break;
		case 'd' : arguments->workDevT1 = atoi(arg); break;
		case 'D' : arguments->workDevT2 = atoi(arg); break;
		case ARGP_KEY_ARG: return 0;
		default : return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0 };

int main(int argc, char** argv) {

	// parsing der Argumente
	int schedPolicy = SCHED_RR;
	
	if (argc > 1 && strcmp(argv[1], "FIFO") == 0)
	{
		schedPolicy = SCHED_FIFO;
		printf("FIFO SCHEDULING ACTIVE\n");
	}

	struct arguments arguments;
	// default
	memset(&arguments, 0, sizeof(struct arguments));
	arguments.iterations = 1000;
	arguments.scheduler = SCHED_OTHER;
	arguments.prioT1 = 0;
	arguments.prioT2 = 0;
	arguments.cycleT1 = 2000;
	arguments.cycleT2 = 4000;
	arguments.workT1 = 60;
	arguments.workT2 = 120;
	arguments.workDevT1 = 30;
	arguments.workDevT2 = 20;

	// parse ...
	argp_parse(&argp, argc, argv, 0, 0, &arguments);
	
	pthread_attr_t attr;
	struct sched_param param1, param2;

	pthread_attr_init(&attr);
	pthread_attr_setschedpolicy(&attr, schedPolicy);
	
	param1.sched_priority = arguments.prioT1;
	param2.sched_priority = arguments.prioT2;

	pthread_t thread1;
	pthread_t thread2;

	responseTimeArray = calloc(2, sizeof(struct timeval*));
	if (responseTimeArray == NULL) return -1;
	responseTimeArray[0] = calloc(arguments.iterations, sizeof(struct timeval));
	if (responseTimeArray[0] == NULL) return -1;
	responseTimeArray[1] = calloc(arguments.iterations, sizeof(struct timeval));
	if (responseTimeArray[1] == NULL) return -1;


	struct timerMessStruct t1_struct = {
		arguments.cycleT1,
		arguments.workT1,
		arguments.workDevT1,
		"thread1", 
		arguments.iterations,
		0, 
		arguments.scheduler, 
		arguments.prioT1};
	
	struct timerMessStruct t2_struct = {
		arguments.cycleT2,
		arguments.workT2,
		arguments.workDevT2,
		"thread2", 
		arguments.iterations,
		1, 
		arguments.scheduler, 
		arguments.prioT2};

	gettimeofday(&startZeit, NULL);
	pthread_attr_setschedparam(&attr, &param1);
	pthread_create(&thread1, &attr, threadFunction, &t1_struct);
	
	pthread_attr_setschedparam(&attr, &param2);
	pthread_create(&thread2, NULL, threadFunction, &t2_struct);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("Utilization: %f %%\n", 
		((double)arguments.workT1/arguments.cycleT1  +
		 (double)arguments.workT2/arguments.cycleT2) * 100.0);
	// ausgabe der ergebnisse
	printf("Response time thread 1\n");

	printResult(0, arguments.iterations);

	printf("Response time thread 2\n");

	printResult(1, arguments.iterations);

	pthread_attr_destroy(&attr);

	return 0;
}


void* threadFunction(void* d) {

	struct timerMessStruct* conf = (struct timerMessStruct*) d;


	printf("Config %s: CycleTime %d WorkDuration %d WorkDerivation %d\n", 
			conf->name, conf->cycletime, conf->worktime, conf->workdeviation);
	static struct timeval thread1StartedTime;	
	static struct timeval thread2StartedTime;
	
	static bool thread1Started = false;
	static bool thread2Started = false;

	bool thread1Active = (bool)(conf->cycletime == 2000);

	if (!thread1Started && conf->cycletime == 2000)
	{
		thread1Started = true;
		gettimeofday(&thread1StartedTime, NULL);
	}

	if (!thread2Started && conf->cycletime == 4000)
	{
		thread2Started = true;
		gettimeofday(&thread2StartedTime, NULL);
	}


	// hier Zeit-Strukturen anlegen
	struct timeval startTimeTarget;

	// get thread id
	pthread_t myPTid = pthread_self();

	// hier Scheduling und Prioritaet setzen
	

	int deadlineMissed = 0;
	struct timeval time1, time2, timeDiff;
	
	for (int i = 0; i < conf->iterations; i++) {

		// Hier ggf. Code einfuegen

		// Hier Response-Zeit berechnen
		// und responseTimeArray[conf->id][i] zuweisen 
		
		// do work
		gettimeofday(&time1, NULL);
		doWork(conf->worktime, conf->workdeviation);
		gettimeofday(&time2, NULL);

		timersub(&time2, &time1, &time2);

		while (time2.tv_usec <= conf->cycletime) 
		{
			gettimeofday(&time2, NULL);
			timersub(&time2, &time1, &time2);

		}

		if (thread1Started)
		{
			timeradd(&thread1StartedTime, &time2, &thread2StartedTime);
		}

		else
		{
			timeradd(&thread2StartedTime, &time2, &thread2StartedTime);
		}
	}
	printf("%s: Anzahl verpasster Deadlines: %d \n", conf->name, deadlineMissed);

	return NULL;	
}
