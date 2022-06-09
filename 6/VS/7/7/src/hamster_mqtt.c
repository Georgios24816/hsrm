#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <assert.h>


/**
 * @brief Debug support macros
 *
 * Output messages if @c verbose is higher than a given level.
 *
 * NOTE: Usage @c debug((<printf-params>)); i.e. use @e two brackets
 *       to enclose the printf parameter list!
 */
#ifdef EBUG
#define debug(x) do { printf x ; fflush(stdout);  } while (0)
#else
#define debug(x)
#endif

#define HMSTR_MAX_NAME  31
static char clientid[32];



/*
 * Code stolen and adapted from 5/sunrpc/hamster_cli.c: 
 */
static void cli_commands() {
	printf("\nCommands:\n");
	printf("        A                           - Go to room A\n");
	printf("        B                           - Go to room B\n");
	printf("        C                           - Go to room C\n");
	printf("        D                           - Go to room D\n");
	printf("        r                           - change state to RUNNING\n");
	printf("        s                           - change state to SLEEPING\n");
	printf("        e                           - change state to EATING\n");
	printf("        m                           - change state to MATING\n");
	printf("        q                           - terminate hamster\n");
	printf("> ");
	fflush(stdout);
}


static void flushinput(void)
{
	while(getchar() != '\n')
		;	
}

void rtfm(char** argv) {
	printf("Usage: %s hamster_client_id {<Options>}\n", argv[0]);
	printf("Function: Hamster instrumentation device software\n");
	printf("Options:\n");
	printf("     -p <port>                      - port of the mqtt server (default (no tls): 1883\n");
	printf("     -s <IP address>                - IP address to run the server on (default: 127.0.0.1\n");
	printf("     -v                             - Connect with Certificate based SSL/TLS Support to the MQTT server \n");
	printf("     -V                             - Connect with Certificate based client authentification to the MQTT server \n");
	printf("     -o <owner name>                - Hamster owner's name (default: $USER)\n");
	printf("     -n <hamster name>              - Hamster's name (default: myhamster)\n");
	printf("     -i <hamster ID>                - Hamster's ID (overrides owner & hamster names)\n");
	printf("     -h                             - This help \n");
}



/*
 * Declare these static global to suppress compiler warnings
 * about variables being "set but not used". Ultimately, your
 * program *should* use these variables so you should be able
 * move these into main() without warnings.
 */
static int  doAuthenticate = 0;
static int  doEncrypt = 0;
static unsigned int port = 1883;
static char *ipaddr = "127.0.0.1";


int main(int argc, char** argv) 
{
	int hamster_id = -1;
	char *owner = getenv("USER");
	char *hamster = "myhamster";
	char cmd;


	/*
	* parse args:
	*/
	while ((cmd = getopt(argc, argv, "?p:i:s:o:n:hVv")) != -1)
	{
		switch(cmd)
		{
			case 'p':
			{
				char* end;
				port = strtoul(optarg, &end, 0);
				if (optarg == end) {
					printf("%s: Not a number: %s\n", argv[0], optarg);
					exit(EXIT_FAILURE);
				}
				break;
			}
			case 'i':
			{
				char* end;
				hamster_id = strtoul(optarg, &end, 0);
				if (optarg == end) {
					printf("%s: Not a number: %s\n", argv[0], optarg);
					exit(EXIT_FAILURE);
				}
				break;
			}
			case 's':
				ipaddr = optarg;
				break;
			case 'v':
				doEncrypt = 1;
				break;
			case 'V':
				doAuthenticate = 1;
				doEncrypt = 1;
				break;
			case 'h':
			case '?':
				rtfm(argv);
				return EXIT_SUCCESS;
				break;
			case 'o':
			{
				int l = strlen(optarg);
				if(l > 0 && l < HMSTR_MAX_NAME) {
					owner = optarg;
				}
				else {
					printf("%s: owner name empty or too long: %s\n", argv[0], optarg);
					exit(EXIT_FAILURE);
				}
				break;
			}
			case 'n':
			{
				int l = strlen(optarg);
				if(l > 0 && l < HMSTR_MAX_NAME) {
					hamster = optarg;
				}
				else {
					printf("%s: owner name empty or too long: %s\n", argv[0], optarg);
					exit(EXIT_FAILURE);
				}
				break;
			}
		}
	}

	if(hamster_id == -1)
	{
		/* Hier ist was zu tun: */

		debug(("Calling: hamster_id = make_hash(\"%s\", \"%s\");", owner, hamster));
	}
	sprintf(clientid, "%d", hamster_id);
	printf("** Using ID %d for %s's Hamster %s **\n", hamster_id, owner, hamster);

	/*
	* connect to MQTT broker
	* 
	*/

	/* Hier ist was zu tun: */

	/*
	* Get admission time, start RUNNING in room A
	*/

	/* Hier ist was zu tun: */

	/*
	* publish our hamster ID on /pension/livestock:
	*/

	/* Hier ist was zu tun: */

	/*
	* Main Command loop
	*/
	do {
		cli_commands();
		cmd = getchar();
		flushinput();
		switch (cmd) {
			case 'A': /* going to room A */

				/* Hier ist was zu tun: */

				break;
			case 'B': /* going to room B */

				/* Hier ist was zu tun: */

				break;
			case 'C': /* going to room C */

				/* Hier ist was zu tun: */

				break;
			case 'D': /* going to room D */

				/* Hier ist was zu tun: */

				break;
			case 'r': /* change to "RUNNING" state */

				/* Hier ist was zu tun: */

				break;
			case 's': /* change to "SLEEPING" state */

				/* Hier ist was zu tun: */

				break;
			case 'e': /* change to "EATING" state */

				/* Hier ist was zu tun: */

				break;
			case 'm': /* change to "MATEING" state */

				/* Hier ist was zu tun: */

				break;
			case 'q':
				printf("quit\n");
				break;
			default:
				printf("unknown command\n\n");
		}
	}
	while(cmd != EOF && cmd != 'q');

	/* Hier ist was zu tun: */

	return 0;
}
