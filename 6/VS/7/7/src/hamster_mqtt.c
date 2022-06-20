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
	printf("        *                           - randomAction\n");
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

/**
 * @brief fondle hamster
 * 
 * @param count counts of fondles
 */
void hamsterFondle(int count);

/**
 * @brief punish hamster
 * 
 * @param count punishment count
 */
void hamsterPunish(int count);

/**
 * @brief Callback to be called when a MQTT message arrives
 * 
 * @param context 
 * @param topicName 
 * @param topicLen 
 * @param message 
 * @return int 
 */
int mqtt_messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	assert(topicName && message);
    char* payloadptr = message->payload;
	char buffer[M_MqttMessageLen];
	snprintf(buffer, M_MqttMessageLen, "/pension/hamster/%d/fondle", HamsterId);

	if (strcmp(topicName, buffer) == 0)
	{
		hamsterFondle(atoi(payloadptr));
	}

	else
	{
		snprintf(buffer, M_MqttMessageLen, "/pension/hamster/%d/punish", HamsterId);

		if (strcmp(topicName, buffer) == 0)
		{
			hamsterPunish(atoi(payloadptr));
		}
	}
    
	MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

typedef enum MqqtPensionTopic 
{ 
	MPT_ERROR = -1,
	MPT_Livestock, 
	MPT_Wheels, 
	MPT_State, 
	MPT_Position, 
	MPT_Fondle, 
	MPT_Punish,
	MPT_RoomA,
	MPT_RoomB,
	MPT_RoomC,
	MPT_RoomD,
	MPT_COUNT
} MqttPensionTopic_t;

typedef struct MqttFormatString
{
	const char* s;
	bool needsHamsterId;
	int qos;
} MqttFormatString_t;

typedef int (*MqttIteratorFunction_t)(const char*, MqttPensionTopic_t, int);

#define M_HamsterIdAll (-43)

/**
 * @brief iterate all topics and call a function on them
 * 
 * @param func function to be called with topic names qos ...
 * @param hamsterId hamsterId to be used for the topics
 */
void mqtt_topicIterator(MqttIteratorFunction_t func, int hamsterId)
{
	static const MqttFormatString_t FormatStrings[] ={
												{"/pension/livestock", 0, M_Qos1},
												{"/pension/hamster/%d/wheels", 1, M_QosMax1},
												{"/pension/hamster/%d/state", 1, M_QosMin1},
												{"/pension/hamster/%d/position", 1, M_QosMin1},
												{"/pension/hamster/%d/fondle", 1, M_Qos1},
												{"/pension/hamster/%d/punish", 1, M_Qos1},
												{"/pension/room/A", 0, M_QosMax1},
												{"/pension/room/B", 0, M_QosMax1},
												{"/pension/room/C", 0, M_QosMax1},
												{"/pension/room/D", 0, M_QosMax1}
											};

	assert((sizeof(FormatStrings) / sizeof(FormatStrings[0])) == MPT_COUNT);
	assert(func);

	char buffer[M_MqttMessageLen];
	const MqttFormatString_t* s;
	int funcRet;

	for (int i = 0; i < MPT_COUNT; i++)
	{
		s = &FormatStrings[i];

		if (s->needsHamsterId)
		{
			if (hamsterId == M_HamsterIdAll)
			{
				char buffer2[M_MqttMessageLen];
				const int intIndex = 18;

				strcpy(buffer2, s->s);
				buffer2[intIndex] = 'c';

				snprintf(buffer, M_MqttMessageLen, buffer2, (char)(-M_HamsterIdAll)); 
			}

			else
			{
				snprintf(buffer, M_MqttMessageLen, s->s, hamsterId);
			}

			funcRet = func(buffer, i, s->qos);
		}

		else
		{
			funcRet = func(s->s, i, s->qos);
		}

		if (funcRet)
		{
			return;
		}
	}
}

MqttPensionTopic_t MatchedTopic = MPT_ERROR;
char* ReceivedTopic;
/**
 * @brief check if a message received is from a valid topic
 * 
 * @param topic topic the of message received
 * @param topicIndex index of the message received
 * @param qos  qos of the message received
 * @return int 1 on success ; 0 on error
 */
int mqtt_matchTopic(const char* topic, MqttPensionTopic_t topicIndex, int qos)
{
	if (strcmp(ReceivedTopic, topic) == 0)
	{
		MatchedTopic = topicIndex;
		return 1;
	}

	return 0;
}

/**
 * @brief check if the topic matches with the received topic
 * 
 * @param topic topic to be matched
 * @param topicLen topic length in bytes
 * @param hamsterId hamster id
 * @return MqttPensionTopic_t matched topic
 */
MqttPensionTopic_t mqtt_topicChecker(char* topic, int topicLen, int hamsterId)
{
	if (!(topic && hamsterId))
	{
		return MPT_ERROR;
	}

	ReceivedTopic = topic;
	mqtt_topicIterator(mqtt_matchTopic, hamsterId);
	
	MqttPensionTopic_t matchedTopicCopy = MatchedTopic;
	MatchedTopic = MPT_ERROR;
	ReceivedTopic = NULL;

	return matchedTopicCopy;
}

/**
 * @brief optional idea 7.1.6. This funtions is like mqtt_messageArrived but for all topics
 * 
 * @param context 
 * @param topicName 
 * @param topicLen 
 * @param message 
 * @return int 
 */
int mqtt_visualizer(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	//assert(context && topicName && topicLen && message && message->payloadlen && message->payload);

	//extract id from topic if it exists

	//find last backslash (first from the end) and check if its lead by a digit
	//simple solution
	const int numberIndex = 17;
	int hamster = atoi(&topicName[numberIndex]);

	if (hamster == 0)
	{
		hamster = HamsterId;
	}

	MqttPensionTopic_t ret = mqtt_topicChecker(topicName, topicLen, hamster);
	//char* payload = message->payload;
	//printf("hamsterId : %d | topic : '%s' | ret : %d\n", hamster, topicName, (int)ret);
	//printf("PAYLOAD : '%s'\n", payload);

	return ret != MPT_ERROR; 
}

/**
 * @brief check if the connection struct has valid pointers
 * 
 * @param connection a connection struct
 * @return true if all pointers are valid; else false
 */
bool mqtt_connectionStructTest(MQTT_Connection_t* connection)
{
	return connection && connection->address && connection->clientId && connection->clientIdString;
}

MQTT_Connection_t* SubscribeConnection;
/**
 * @brief function to subscribe to all topics
 * 
 * @param topic topic to subscribe
 * @param topicIndex index of topic
 * @param qos qos of topic
 * @return int always 0
 */
int mqtt_subscribeAll(const char* topic, MqttPensionTopic_t topicIndex, int qos)
{
	assert(topic && topicIndex != MPT_ERROR);
	MQTTClient_subscribe(SubscribeConnection, topic, qos);

	return 0;
}

/**
 * @brief subscribe to topic
 * 
 * @param connection connection
 * @return always true
 */
bool mqtt_subscribe(MQTT_Connection_t* connection)
{
	//char fondleBuffer[M_MqttMessageLen];
	//snprintf(fondleBuffer, M_MqttMessageLen, "/pension/hamster/%d/fondle", connection->clientId);
	//char punishBuffer[M_MqttMessageLen];
	//snprintf(punishBuffer, M_MqttMessageLen, "/pension/hamster/%d/punish", connection->clientId);
	//MQTTClient_subscribe(connection->client, fondleBuffer, M_Qos1);
	//MQTTClient_subscribe(connection->client, punishBuffer, M_Qos1);

	SubscribeConnection = connection->client;
	mqtt_topicIterator(mqtt_subscribeAll, M_HamsterIdAll);
	//SubscribeConnection = NULL;

	return true;
}

/**
 * @brief connect to mqqt
 * 
 * @param connection connections struct
 * @param ssl optional ssl options
 * @return true on success; exit on failure
 */
bool mqtt_connect(MQTT_Connection_t* connection, MQTTClient_SSLOptions* ssl)
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

	if (ssl)
	{
		conn_opts.ssl = ssl;
	}

	MQTTClient_setCallbacks(connection->client, NULL, NULL, mqtt_messageArrived, NULL);
	//MQTTClient_setCallbacks(connection->client, NULL, NULL, mqtt_visualizer, NULL);
	
	if ((rc = MQTTClient_connect(connection->client, &conn_opts)) != MQTTCLIENT_SUCCESS)
	{
		exit(EXIT_FAILURE);
	}

	mqtt_subscribe(connection);

	return true;
}

/**
 * @brief end mqtt connection
 * 
 * @param client client to be disconnected
 */
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

/**
 * @brief get room-string from room
 * 
 * @param room room for string needed
 * @return const char* room-string; on failure no return
 */
const char* hamsterRoomString(HamsterRoom_t room)
{
	static const char* Rooms[] = { "A", "B", "C", "D" };

	if (room >= (sizeof(Rooms) / sizeof(Rooms[0])))
	{
		assert(false && "wrong room");
	}

	return Rooms[room];
}

/**
 * @brief string from hamster state
 * 
 * @param state state
 * @return const char* hamster string; on failure false assertion
 */
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
/**
 * @brief check if the qos is valid
 * 
 * @param qos qos
 * @return true if qos is valid; else false
 */
bool isQosValid(int qos)
{
	return (qos == M_Qos1) || (qos == M_QosMin1) || (qos == M_QosMax1);
}

/**
 * @brief publish mqtt message
 * 
 * @param client client to publish the message
 * @param topic topic of the message
 * @param qos qos of the message
 * @param message message string itself
 * @return true on success; false on error
 */
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
	pubmsg.retained = false;

	if (MQTTClient_publishMessage(client, topic, &pubmsg, &DeliveryToken) != MQTTCLIENT_SUCCESS)
	{
		return false;
	}

	return MQTTClient_waitForCompletion(client, DeliveryToken, M_TimeoutMs) == MQTTCLIENT_SUCCESS;
}

/**
 * @brief register new hamster in /pension/livestock
 * 
 * @param client client to publish the message
 * @param id hamster id of the new hamster
 * @return true on success; false on error
 */
bool mqtt_publishLivestock(MQTTClient client, const char* id)
{
	return mqtt_publish(client, "/pension/livestock", M_Qos1, id);
}

/**
 * @brief publish hamster rounds 
 * 
 * @param client client to publish the message
 * @param hamsterId hamster id
 * @param roundCount hamster round count
 * @return true on success; false on error
 */
bool mqtt_publishRounds(MQTTClient client, int hamsterId, int roundCount)
{
	char buffer[M_MqttMessageLen];
	snprintf(buffer, M_MqttMessageLen, "/pension/hamster/%d/wheels", hamsterId);

	char roundBuffer[M_MaxIdLen];
	snprintf(roundBuffer, M_MaxIdLen, "%d", roundCount);

	return mqtt_publish(client, buffer, M_QosMax1, roundBuffer);
}

/**
 * @brief change hamster room and publish message
 * 
 * @param client client to publish the message
 * @param hamsterId hamster to change room
 * @param room new room
 * @return true on success; false on error
 */
bool hamsterSetRoom(MQTTClient client, int hamsterId, HamsterRoom_t room)
{
	char buffer[M_MqttMessageLen];
	snprintf(buffer, M_MqttMessageLen, "/pension/hamster/%d/position", hamsterId);

	return mqtt_publish(client, buffer, M_QosMin1, hamsterRoomString(room));
}

/**
 * @brief change hamster state and publish message
 * 
 * @param client client to publish the message
 * @param hamsterId hamster to change state
 * @param state new state
 * @return true on success; false on error
 */
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

/**
 * @brief punish hamster prototype above
 * 
 * @param count count of punishments
 */
void hamsterPunish(int count)
{
	static int punishmentCounter = 0;
	punishmentCounter += count;
	printf("PUNISH %d\n", punishmentCounter);
}

/**
 * @brief fondle hamster prototype above
 * 
 * @param count count of fondles
 */
void hamsterFondle(int count)
{
	static unsigned int fondleCounter = 0;
	fondleCounter += count;
	printf("FONDLE %d\n", fondleCounter);
}

/**
 * @brief function to make_hash, stolen from an old exercise
 * 
 * @param s1 string 1
 * @param s2 string 2
 * @return int hash value
 */
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

void hamster_randomAction(MQTTClient client, int hamster_id)
{
	assert(client);

	int r = rand() % 7;

	if (r <= 3)
	{
		hamsterSetRoom(client, hamster_id, r);
		printf("go to room : %s\n", hamsterRoomString(r));
	}

	else if (r <= 7)
	{
		hamsterSetState(client, hamster_id, r - 4);
		printf("set state : %s\n", hamsterStateString(r - 4));
	}
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
	//const char* connectionUnsafe = ;
	//const char* connectionSafe = ; //"hamsteriot.vs.cs.hs-rm.de:8883";

	int connectionStringIndex = 0;
	const char* connectionStrings[] = 	{  	
											"tcp://localhost:1883",
									 	 	"tcp://localhost:8883",
										 	"tcp://localhost:8884"
											//"hamsteriot.vs.cs.hs-rm.de:1883",
											//"hamsteriot.vs.cs.hs-rm.de:8883",
											//"hamsteriot.vs.cs.hs-rm.de:8884"
										};

	static MQTTClient_SSLOptions sslOptions = MQTTClient_SSLOptions_initializer;
	static MQTTClient_SSLOptions* ssl = NULL;

	//openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -sha256 -days 365
	if (doEncrypt)
	{
		sslOptions.trustStore = "../certs/mqtt_ca.crt";
		sslOptions.keyStore = "../certs/cert.pem";
		connectionStringIndex = 1;
		ssl = &sslOptions;
	}

	if (doAuthenticate)
	{
		sslOptions.privateKey = "../certs/key.pem";
		sslOptions.privateKeyPassword = "1234";
		connectionStringIndex = 2;
		//ssl = &sslOptions;
	}

	const char* connectionString = connectionStrings[connectionStringIndex];

	MQTT_Connection_t connection = { NULL, connectionString, hamster_id, hamsterIdString };
	//MQTT_Connection_t connection={NULL,"hamsteriot.vs.cs.hs-rm.de",hamster_id,hamsterIdString};

//mosquitto_pub -t "/Test" -h hamsteriot.vs.cs.hs-rm.de -m "test2"

	/*
	* connect to MQTT broker
	* 
	*/
	assert(mqtt_connect(&connection, ssl) && "connection failed");
	HamsterId = hamster_id;
	/*
	* Get admission time, start RUNNING in room A
	*/

	// time_t admissionTime = time(NULL); //TODO
	srand(time(NULL)); //initialize random
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
			case '*':
				hamster_randomAction(connection.client, hamster_id);
				break;
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
