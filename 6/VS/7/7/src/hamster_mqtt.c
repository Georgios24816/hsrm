#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include "MQTTClient.h"

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

#define M_MaxIdLen 64
#define M_MqttMessageLen 256
#define M_TimeoutMs 10000

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

typedef struct MQTT_Subscription
{
	const char* topic;
	uint8_t serviceQuality;
} MQTT_Subscription_t;

typedef struct MQTT_Connection
{
	MQTTClient client;
	const char* address;
	int clientId;
	const char* clientIdString;
} MQTT_Connection_t;

#define M_QosMax1 	0
#define M_QosMin1 	1
#define M_Qos1  	2

static int HamsterId = 0;

void hamsterFondle();
void hamsterPunish();

int mqtt_messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    //char* payloadptr = message->payload;
	char buffer[M_MqttMessageLen];
	snprintf(buffer, M_MqttMessageLen, "/pension/hamster/%d/fondle", HamsterId);

	if (strcmp(topicName, buffer) == 0)
	{
		hamsterFondle();
	}

	else
	{
		snprintf(buffer, M_MqttMessageLen, "/pension/hamster/%d/punish", HamsterId);

		if (strcmp(topicName, buffer) == 0)
		{
			hamsterPunish();
		}
	}
    
	MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

bool mqtt_connectionStructTest(MQTT_Connection_t* connection)
{
	return connection && connection->address && connection->clientId && connection->clientIdString;
}

bool mqtt_subscribe(MQTT_Connection_t* connection)
{
	char fondleBuffer[M_MqttMessageLen];
	snprintf(fondleBuffer, M_MqttMessageLen, "/pension/hamster/%d/fondle", connection->clientId);

	char punishBuffer[M_MqttMessageLen];
	snprintf(punishBuffer, M_MqttMessageLen, "/pension/hamster/%d/punish", connection->clientId);

	MQTTClient_subscribe(connection->client, fondleBuffer, M_Qos1);
	MQTTClient_subscribe(connection->client, punishBuffer, M_Qos1);

	MQTTClient_subscribe(connection->client, "/pension/room/+", M_QosMax1);

	return true;
}

bool mqtt_connect(MQTT_Connection_t* connection)
{
	assert(mqtt_connectionStructTest(connection));
	int rc;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

	rc = MQTTClient_create(&(connection->client), connection->address, connection->clientIdString,
        				MQTTCLIENT_PERSISTENCE_NONE, NULL);

	if (rc != MQTTCLIENT_SUCCESS)
	{
		exit(EXIT_FAILURE);
	}

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
	conn_opts.username = "hamster";

	MQTTClient_setCallbacks(connection->client, NULL, NULL, mqtt_messageArrived, NULL);
	
	if ((rc = MQTTClient_connect(connection->client, &conn_opts)) != MQTTCLIENT_SUCCESS)
	{
		exit(EXIT_FAILURE);
	}

	mqtt_subscribe(connection);

	return true;
}

void mqtt_disconnect(MQTTClient* client)
{
	if (client)
	{
		MQTTClient_disconnect(*client, M_TimeoutMs);
		MQTTClient_destroy(client);
	}
}

typedef enum HamsterRoom { HR_A, HR_B, HR_C, HR_D } HamsterRoom_t;
typedef enum HamsterState { HS_Running, HS_Sleeping, HS_Eating, HS_Mateing } HamsterState_t;

const char* hamsterRoomString(HamsterRoom_t room)
{
	static const char* Rooms[] = { "A", "B", "C", "D" };

	if (room >= (sizeof(Rooms) / sizeof(Rooms[0])))
	{
		assert(false && "wrong room");
	}

	return Rooms[room];
}

const char* hamsterStateString(HamsterState_t state)
{
	static const char* States[] = { "RUNNING", "SLEEPING", "EATING", "MATEING" };
	
	if (state >= (sizeof(States) / sizeof(States[0])))
	{
		assert(false && "wrong state");
	}

	return States[state];
}

MQTTClient_deliveryToken DeliveryToken;

bool isQosValid(int qos)
{
	return (qos == M_Qos1) || (qos == M_QosMin1) || (qos == M_QosMax1);
}

bool mqtt_publish(MQTTClient client, const char* topic, int qos, const char* message)
{
	assert(client && topic && isQosValid(qos) && message);
	const int messageLen = strlen(message);
	const int topicLen = strlen(topic);
	assert(messageLen > 0 && messageLen < M_MqttMessageLen);
	assert(topicLen > 0 && topicLen < M_MqttMessageLen);

	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	char buffer[M_MqttMessageLen];
	strcpy(buffer, message);

	pubmsg.payload = buffer;
	pubmsg.payloadlen = messageLen;
	pubmsg.qos = qos;
	pubmsg.retained = true;

	if (MQTTClient_publishMessage(client, topic, &pubmsg, &DeliveryToken) != MQTTCLIENT_SUCCESS)
	{
		return false;
	}

	return MQTTClient_waitForCompletion(client, DeliveryToken, M_TimeoutMs) == MQTTCLIENT_SUCCESS;
}

bool mqtt_publishLivestock(MQTTClient client, const char* id)
{
	return mqtt_publish(client, "/pension/livestock", M_Qos1, id);
}

bool mqtt_publishRounds(MQTTClient client, int hamsterId, int roundCount)
{
	char buffer[M_MqttMessageLen];
	snprintf(buffer, M_MqttMessageLen, "/pension/hamster/%d/wheels", hamsterId);

	char roundBuffer[M_MaxIdLen];
	snprintf(roundBuffer, M_MaxIdLen, "%d", roundCount);

	return mqtt_publish(client, buffer, M_QosMax1, roundBuffer);
}

bool hamsterSetRoom(MQTTClient client, int hamsterId, HamsterRoom_t room)
{
	char buffer[M_MqttMessageLen];
	snprintf(buffer, M_MqttMessageLen, "/pension/hamster/%d/position", hamsterId);

	return mqtt_publish(client, buffer, M_QosMin1, hamsterRoomString(room));
}

bool hamsterSetState(MQTTClient client, int hamsterId, HamsterState_t state)
{
	static int SecondCounter = 0;
	static int SecondCounterStart = -1;
	static int SecondCounterEnd = -1;

	static int RoundCounter = 0;
	static HamsterState_t oldState = HS_Running;
	char buffer[M_MqttMessageLen];
	snprintf(buffer, M_MqttMessageLen, "/pension/hamster/%d/state", hamsterId);

	if (SecondCounterStart == -1 && oldState == HS_Running)
	{
		SecondCounterStart = time(NULL);
	}

	if (mqtt_publish(client, buffer, M_QosMin1, hamsterStateString(state)) != MQTTCLIENT_SUCCESS)
	{
		//return false;
	}

	if (state != HS_Running && SecondCounterEnd == -1)
	{
		SecondCounterEnd = time(NULL);
	}

	if (state == HS_Running && oldState != state)
	{
		SecondCounter += SecondCounterEnd - SecondCounterStart;
		SecondCounterStart = time(NULL);
		SecondCounterEnd = -1;
		RoundCounter = SecondCounter / 60;
		mqtt_publishRounds(client, hamsterId, (int)(RoundCounter * 25));
	}

	oldState = state;

	//mqtt_publish(client, bu)
	return true;
}

void hamsterPunish()
{
	static int punishmentCounter = 0;
	printf("PUNISH %d\n", ++punishmentCounter);
}

void hamsterFondle()
{
	static unsigned int fondleCounter = 0;
	printf("FONDLE %d\n", ++fondleCounter);
}

static int make_hash(const char *s1, const char *s2)
{
	const int InitialValue = 5381;
	const int M = 33;

	unsigned int hash;
	int i;
    char key[2*(HMSTR_MAX_NAME+1)];
     /* copy both strings into a single buffer */
	memset(key, 0, sizeof(key));
	strcpy(&key[0], s1);
	strcpy(&key[HMSTR_MAX_NAME+1], s2);

        /* compute a hash value over the buffer's contents */
	hash = InitialValue;
	for(i = 0; i < 2*(HMSTR_MAX_NAME+1); ++i)
	        hash = M * hash + key[i];

	/* make sure always get a >= 0 number */
	return hash >> 1U;
}


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
		hamster_id = make_hash(owner, hamster);
		debug(("Calling: hamster_id = make_hash(\"%s\", \"%s\");", owner, hamster));
	}
	sprintf(clientid, "%d", hamster_id);
	printf("** Using ID %d for %s's Hamster %s **\n", hamster_id, owner, hamster);

	char hamsterIdString[M_MaxIdLen];
	snprintf(hamsterIdString, M_MaxIdLen, "%d", hamster_id);
	MQTT_Connection_t connection = { NULL, "tcp://localhost:1883", hamster_id, hamsterIdString };

//mosquitto_pub -t "/Test" -h hamsteriot.vs.cs.hs-rm.de -m "test2"

	/*
	* connect to MQTT broker
	* 
	*/
	assert(mqtt_connect(&connection) && "connection failed");
	HamsterId = hamster_id;
	/*
	* Get admission time, start RUNNING in room A
	*/

	// time_t admissionTime = time(NULL); //TODO

	hamsterSetState(connection.client, hamster_id, HS_Running);
	hamsterSetRoom(connection.client, hamster_id, HR_A);

	/*
	* publish our hamster ID on /pension/livestock: 
	*/
	mqtt_publishLivestock(connection.client, connection.clientIdString);

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
				hamsterSetRoom(connection.client, hamster_id, HR_A);
				break;
			case 'B': /* going to room B */
				/* Hier ist was zu tun: */
				hamsterSetRoom(connection.client, hamster_id, HR_B);
				break;
			case 'C': /* going to room C */
				/* Hier ist was zu tun: */
				hamsterSetRoom(connection.client, hamster_id, HR_C);
				break;
			case 'D': /* going to room D */
				/* Hier ist was zu tun: */
				hamsterSetRoom(connection.client, hamster_id, HR_D);
				break;
			case 'r': /* change to "RUNNING" state */
				/* Hier ist was zu tun: */
				hamsterSetState(connection.client, hamster_id, HS_Running);
				break;
			case 's': /* change to "SLEEPING" state */
				/* Hier ist was zu tun: */
				hamsterSetState(connection.client, hamster_id, HS_Sleeping);
				break;
			case 'e': /* change to "EATING" state */
				/* Hier ist was zu tun: */
				hamsterSetState(connection.client, hamster_id, HS_Eating);
				break;
			case 'm': /* change to "MATEING" state */
				/* Hier ist was zu tun: */
				hamsterSetState(connection.client, hamster_id, HS_Mateing);
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
	mqtt_disconnect(&(connection.client));

	return 0;
}
