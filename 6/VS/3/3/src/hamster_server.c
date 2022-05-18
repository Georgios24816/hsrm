/**
 * @file	hamster_server.c
 * @brief	Remote Hamsterverwaltungsprogramm
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <byteswap.h>

#include "../include/hamsterlib.h"

#include "hamsterprotocol.h"
#include "hamsterInterface.h"

#include <assert.h>

#define _MESSAGELEN 65536

static char ResponseBuffer[_MESSAGELEN - HAMSTER_RPC_PROT_HEADER_SIZE] = { 0 };

/**
 * @brief Kurze Hilfe ausgeben
 */
static void rtfm(char* argv[])
{
	printf("Usage: %s {<Option>} <param1> \n", argv[0]);
	printf("Function: Hamster management server\n");
	printf("Optionen:\n");
	printf("     -p {<port>}                    - port to run the server\n");
	printf("     -h {<IP address>}              - IP address to run the server on (default: 127.0.0.1\n");
}

static char MessageBuffer[_MESSAGELEN] = { 0 };


uint16_t encode_uint16(uint16_t val)
{
	uint16_t val2 = ((val >> 8) | (val << 8));
	return val2;
}

uint32_t encode_uint32(uint32_t val)
{
	return __builtin_bswap32(val);
}

uint64_t encode_uint64(uint64_t val)
{
	return __builtin_bswap64(val);
}

bool sendRPCResponse(int socket, bool error, uint16_t messageID, const void* payload, uint16_t payloadLen, uint16_t callID)
{
    //if (!payload || (payloadLen + HAMSTER_RPC_PROT_HEADER_SIZE) > _MESSAGELEN) 
    //{
    //    return false; //payload not specified or packet too large
    //}

    static uint8_t ResponseBuffer[_MESSAGELEN - HAMSTER_RPC_PROT_HEADER_SIZE] = { 0 };

    ResponseBuffer[HAMSTER_RPC_PROT_HEADER_VERSION_OFFSET] = HAMSTER_RPC_PROT_VERSION;
    ResponseBuffer[HAMSTER_RPC_PROT_HEADER_FLAGS_OFFSET] = 1 + ((!error) ? 2 : 0);
    ((uint16_t*)&ResponseBuffer[HAMSTER_RPC_PROT_HEADER_MSGID_OFFSET])[0] = encode_uint16(messageID);
    ((uint16_t*)&ResponseBuffer[HAMSTER_RPC_PROT_HEADER_PAYLOADLENGTH_OFFSET])[0] = encode_uint16(payloadLen);
    ((uint16_t*)&ResponseBuffer[HAMSTER_RPC_PROT_HEADER_CALLID_OFFSET])[0] = encode_uint16(callID);

    memcpy(&ResponseBuffer[HAMSTER_RPC_PROT_HEADER_SIZE], payload, payloadLen);

    printf("sending back (payloadLen : %d)\n", (int)payloadLen);

    for (int i = 0; i < payloadLen + HAMSTER_RPC_PROT_HEADER_SIZE; i++)
    {
        printf("%d : %d\n", i, (int)ResponseBuffer[i]);
    }

    if (send(socket, ResponseBuffer, payloadLen + HAMSTER_RPC_PROT_HEADER_SIZE, 0) >= 0)
    {
        return true;
    }

    return false;
}

int32_t functionDecoder(char* message, size_t maxSize, uint16_t funcID, uint16_t* payloadLen)
{
    int32_t result = -1;


    if (!payloadLen)
    {
        return result;
    }
    //const uint16_t payloadLenCopy = *payloadLen;

    *payloadLen = 4; //return code is always included

    memset(ResponseBuffer, 0, _MESSAGELEN - HAMSTER_RPC_PROT_HEADER_SIZE);
    size_t index = HAMSTER_RPC_PROT_HEADER_SIZE;

    if (funcID == HAMSTER_RPC_FUNCID_NEW) //DONE TESTED
    {
        const char* owner = &message[index];
        index = 40;

        const char* name = &message[index];
        index = 73;


        uint16_t treats = (*((uint16_t*)(&message[index])));

        printf("%s | %s | %d\n", owner, name, (int)treats);

        result = hmstr_new(owner, name, treats);

		printf("NEW result : %d\n", (int)result);
    }
 
    else if (funcID == HAMSTER_RPC_FUNCID_COLLECT) //DONE TESTED
    {
        const char* owner = &message[index];

        result = hmstr_collect(owner);
    }

    else if (funcID == HAMSTER_RPC_FUNCID_DIRECTORY) //DONE TESTED
    {
        //int32_t file = (int32_t)encode_uint32(*((uint32_t*)&message[index]));
        //bool started = false;
        static int32_t fileCopy = 0;
        static int32_t entryCount = 0;
        int32_t file = (*((int32_t*)&message[index]));
        index += 4;
        char* owner = &message[index];
        char* hamster = &message[index + HMSTR_MAX_NAME + 1];

        char ownerBuffer[HMSTR_MAX_NAME + 1] = { 0 };
        char hamsterBuffer[HMSTR_MAX_NAME + 1] = { 0 };

        int16_t price = 0;

        // result = hmstr_directory(&file, owner, hamster);
        if (entryCount == 0)
        {
            fileCopy = file;
        }

        while (result != HMSTR_ERR_NOTFOUND)
	    {
	    	result = hmstr_directory(&fileCopy, owner, hamster);

            if (errorCheck(result)) //result == HMSTR_ERR_NOTFOUND)
            {
                //started = false;
                entryCount = 0;
                //if (index == 0)
                goto functionDecoderEnd;
            }

            //else
            //{
            //    errorCheck(result);
            //}

            //index++;
            entryCount++;

            //if (result >= 0)
            //{
                //started = true;
                ((uint32_t*)&ResponseBuffer[*payloadLen])[0] = encode_uint32((uint32_t)result);
                *payloadLen += 4;

	    	    result = hmstr_readentry(result, ownerBuffer, hamsterBuffer, &price);

                memcpy(&ResponseBuffer[*payloadLen], ownerBuffer, HMSTR_MAX_NAME + 1);
                *payloadLen += HMSTR_MAX_NAME + 1;

                memcpy(&ResponseBuffer[*payloadLen], hamsterBuffer, HMSTR_MAX_NAME + 1);
                *payloadLen += HMSTR_MAX_NAME + 1;

                ((int16_t*)&ResponseBuffer[*payloadLen])[0] = (int16_t)encode_uint16((uint16_t)price);
                *payloadLen += 2;
                
                ((uint32_t*)&ResponseBuffer[*payloadLen])[0] = encode_uint32((uint32_t)result);
                *payloadLen += 4;
            //}

	    }


        /*
        if (result >= 0)
        {
            result = hmstr_readentry(result, owner, hamster, &price);
        }

        


        ((uint32_t*)&ResponseBuffer[4])[0] = encode_uint32((uint32_t)file);
        memcpy(&ResponseBuffer[8], owner, HMSTR_MAX_NAME + 1);
        memcpy(&ResponseBuffer[8 + HMSTR_MAX_NAME + 1], hamster, HMSTR_MAX_NAME + 1);
        ((int16_t*)&ResponseBuffer[4])[0] = (int16_t)encode_uint16((uint16_t)price);
        *payloadLen += 6 + (2 * (HMSTR_MAX_NAME + 1));
        */
    }

    else if (funcID == HAMSTER_RPC_FUNCID_GIVETREATS) //DONE TESTED
    {
        int32_t ID = (int32_t)encode_uint32(*((uint32_t*)&message[index]));
        index = 12;

        uint16_t treats = encode_uint16(*((uint16_t*)&message[index]));
        result = hmstr_givetreats(ID, treats);
    }

    else if (funcID == HAMSTER_RPC_FUNCID_HOWSDOING) //DONE
    {
		printf("HOWSDOING\n");
        int32_t ID = (int32_t)encode_uint32(*((uint32_t*)(&message[index])));

        struct hmstr_state state;

        result = hmstr_howsdoing(ID, &state);

        ((uint16_t*)&ResponseBuffer[4])[0] = encode_uint16(state.treats_left);
        ((uint32_t*)&ResponseBuffer[6])[0] = encode_uint32(state.rounds);
        ((int16_t*)&ResponseBuffer[10])[0] = (int16_t)encode_uint16((uint16_t)state.cost);

        *payloadLen += 8;
    }

    else if (funcID == HAMSTER_RPC_FUNCID_LOOKUP) //DONE TESTED
    {
        printf("LOOKUP\n");
        const char* owner = &message[index];
        index = 40;

        const char* name = &message[index];

        result = hmstr_lookup(owner, name);

        printf("owner : %s | name : %s\n", owner, name);
    }

    else if (funcID == HAMSTER_RPC_FUNCID_READENTRY) 
    {
        printf("READENTRY\n");
        int32_t ID = (int32_t)encode_uint32(*((uint32_t*)&message[index]));

        char owner[HMSTR_MAX_NAME + 1] = { 0 };
        char name[HMSTR_MAX_NAME + 1] = { 0 };
        int16_t price;


        result = hmstr_readentry(ID, owner, name, &price);

        memcpy(&ResponseBuffer[4], owner, HMSTR_MAX_NAME + 1);
        memcpy(&ResponseBuffer[4 + HMSTR_MAX_NAME + 1], name, HMSTR_MAX_NAME + 1);
        ((uint16_t*)&ResponseBuffer[4 + (2 * (HMSTR_MAX_NAME + 1))])[0] = (encode_uint16((uint16_t)price));

        *payloadLen += 2 + (2 * (HMSTR_MAX_NAME + 1));
    }

    functionDecoderEnd:

    ((uint32_t*)ResponseBuffer)[0] = encode_uint32((uint32_t)result);
    
    return (int32_t)encode_uint32((uint32_t)result);
}

size_t readRPCMessage(int socket)
{
    //read 8 byte header
    ssize_t valread = read(socket, MessageBuffer, HAMSTER_RPC_PROT_HEADER_SIZE);

    if (valread != HAMSTER_RPC_PROT_HEADER_SIZE)
    {
		return 0;
        //perror("invalid header");
        //exit(EXIT_FAILURE);
    }

    uint16_t payloadSize = encode_uint16(*((uint16_t*)(&MessageBuffer[HAMSTER_RPC_PROT_HEADER_PAYLOADLENGTH_OFFSET])));

    valread = read(socket, &MessageBuffer[HAMSTER_RPC_PROT_HEADER_SIZE], payloadSize);

    if (valread != payloadSize)
    {
        perror("invalid payload size");
        exit(EXIT_FAILURE);
    }

    /////
    for (int i = 0; i < HAMSTER_RPC_PROT_HEADER_SIZE + payloadSize; i++)
    {
        printf("%d : %d\n", i, (int)MessageBuffer[i]);
    }
    ////

    return HAMSTER_RPC_PROT_HEADER_SIZE + payloadSize;
}

bool handleRequest(int socket)
{
    memset(MessageBuffer, 0, _MESSAGELEN);
    if (readRPCMessage(socket) < HAMSTER_RPC_PROT_HEADER_SIZE)
    {
        return false;
    }

    uint16_t messageID = encode_uint16(*((uint16_t*)(&MessageBuffer[HAMSTER_RPC_PROT_HEADER_MSGID_OFFSET])));
    uint16_t payloadLen;
    uint16_t callID = encode_uint16(*((uint16_t*)&MessageBuffer[HAMSTER_RPC_PROT_HEADER_CALLID_OFFSET]));

    int32_t ret = functionDecoder(MessageBuffer, _MESSAGELEN, callID, &payloadLen);

    if (ret < 0)
    {
        payloadLen = 4;
    }
    
    if (!sendRPCResponse(socket, !errorCheck(ret), messageID, ResponseBuffer, payloadLen, callID))
    {
        return false;
    }

    printf("ret : %d; callID : %d; payloadLen : %d\n", (int)ret, (int)callID, (int)payloadLen);
    return true;
}

void exitAndCloseSocket(int socket, const char* errorMsg)
{
	int trueValue = 1;
	setsockopt(socket,SOL_SOCKET,SO_REUSEADDR,&trueValue,sizeof(int));
	close(socket);

	perror(errorMsg);
	exit(EXIT_FAILURE);
}

bool isValidIpAddress(char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result != 0;
}

/**
 * @brief Main program
 */
int main(int argc, char* argv[])
{
	if (argc == 3 || argc == 5)
	{
		if (strcmp(argv[1], "-p") != 0 || !isNumber(argv[2]))
		{
			rtfm(argv);
			exit(EXIT_FAILURE);
		}

		if (argc == 5 && (strcmp(argv[3], "-h") != 0 || !isValidIpAddress(argv[4])))
		{
			rtfm(argv);
			exit(EXIT_FAILURE);
		}
	}

	else
	{
        rtfm(argv);
        exit(EXIT_FAILURE);
	}


    uint16_t port = atoi(argv[2]);
	int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
       
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
	printf("TEST1\n");
       
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        exitAndCloseSocket(server_fd, "setsockopt");
    }
	printf("TEST2\n");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");//inet_addr("192.168.1.100");
    address.sin_port = htons(port);
       
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        exitAndCloseSocket(server_fd, "bind");
    }
	printf("TEST3\n");

    if (listen(server_fd, 3) < 0)
    {
        exitAndCloseSocket(server_fd, "listen");
    }
	printf("TEST4\n");

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
    {
        exitAndCloseSocket(server_fd, "accept");
    }
	printf("TEST5\n");

    while (1) //wait for connections
    {
        if (!handleRequest(new_socket))
        {
			printf("handleRequest Failed\n");
            close(new_socket);

            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
            {
				printf("accept failed\n");
                perror("accept");
                exit(EXIT_FAILURE);
            }
        }

		else
		{
			printf("handleRequest Success\n");
		}   
    }

	return 0;
}
