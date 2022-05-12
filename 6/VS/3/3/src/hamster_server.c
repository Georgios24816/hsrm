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

#define _MESSAGELEN 150

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


uint8_t encode_uint8(const char* buffer, size_t index)
{
    if (!buffer)
    {
        return 0;
    }

    return (uint8_t)(buffer[index]);
}

uint16_t encode_uint16(const char* buffer, size_t index)
{
    if (!buffer)
    {
        return 0;
    }

    return (uint16_t)(buffer[index] | buffer[index + 1]);
}

uint32_t encode_uint32(const char* buffer, size_t index)
{
    if (!buffer)
    {
        return 0;
    }

    return (uint32_t)(buffer[index] | buffer[index + 1] | buffer[index + 2] | buffer[index + 3]);
}

int8_t encode_int8(const char* buffer, size_t index)
{
    return (int8_t)(encode_uint8(buffer, index));
}

int16_t encode_int16(const char* buffer, size_t index)
{
    return (int16_t)(encode_uint16(buffer, index));
}

int32_t encode_int32(const char* buffer, size_t index)
{
    return (int32_t)(encode_uint32(buffer, index));
}

bool sendRPCResponse(int socket, bool error, uint16_t messageID, const void* payload, uint16_t payloadLen, uint16_t callID)
{
    if (!payload || (payloadLen + HAMSTER_RPC_PROT_HEADER_SIZE) > _MESSAGELEN) 
    {
        return false; //payload not specified or packet too large
    }

    static uint8_t ResponseBuffer[_MESSAGELEN - HAMSTER_RPC_PROT_HEADER_SIZE] = { 0 };

    ResponseBuffer[HAMSTER_RPC_PROT_HEADER_VERSION_OFFSET] = HAMSTER_RPC_PROT_VERSION;
    ResponseBuffer[HAMSTER_RPC_PROT_HEADER_FLAGS_OFFSET] = HAMSTER_RPC_MASK_FLAG_RESPONSE | (error << HAMSTER_RPC_MASK_FLAG_ERROR);
    ((uint16_t*)&ResponseBuffer[HAMSTER_RPC_PROT_HEADER_MSGID_OFFSET])[0] = htons(messageID);
    ((uint16_t*)&ResponseBuffer[HAMSTER_RPC_PROT_HEADER_PAYLOADLENGTH_OFFSET])[0] = htons(payloadLen);
    ((uint16_t*)&ResponseBuffer[HAMSTER_RPC_PROT_HEADER_CALLID_OFFSET])[0] = htons(callID);

    memcpy(&ResponseBuffer[HAMSTER_RPC_PROT_HEADER_SIZE], payload, payloadLen);

    printf("sending back\n");

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

    *payloadLen = 4; //return code is always included

    memset(ResponseBuffer, 0, _MESSAGELEN - HAMSTER_RPC_PROT_HEADER_SIZE);
    size_t index = HAMSTER_RPC_PROT_HEADER_SIZE;

    if (funcID == HAMSTER_RPC_FUNCID_NEW) //DONE TESTED
    {
        const char* owner = &message[index];
        index = 40;

        const char* name = &message[index];
        index = 73;

        uint16_t treats = encode_uint16(&message[index], 0);

        printf("%s | %s | %d\n", owner, name, (int)treats);

        result = hmstr_new(owner, name, treats);
    }
 
    else if (funcID == HAMSTER_RPC_FUNCID_COLLECT) //DONE TESTED
    {
        const char* owner = &message[index];

        result = hmstr_collect(owner);
    }

    else if (funcID == HAMSTER_RPC_FUNCID_DIRECTORY) //DONE TESTED
    {
        int32_t file = (int32_t)ntohl(*((uint32_t*)&message[index]));
        result = hmstr_directory(&file, NULL, NULL);

        ((uint32_t*)ResponseBuffer)[1] = htonl((uint32_t)file);
        *payloadLen += 4;
    }

    else if (funcID == HAMSTER_RPC_FUNCID_GIVETREATS) //DONE TESTED
    {
        int32_t ID = (int32_t)ntohl(*((uint32_t*)&message[index]));
        index = 12;

        uint16_t treats = ntohs(*((uint16_t*)&message[index]));
        result = hmstr_givetreats(ID, treats);
    }

    else if (funcID == HAMSTER_RPC_FUNCID_HOWSDOING) //DONE
    {
        printf("HOWSDOING\n");
        int32_t ID =  encode_uint32(&message[index], 0);

        struct hmstr_state state;

        result = hmstr_howsdoing(ID, &state);
        ((uint16_t*)&ResponseBuffer[4])[0] = htons(state.treats_left);
        ((uint32_t*)&ResponseBuffer[6])[0] = htonl(state.rounds);
        ((int16_t*)&ResponseBuffer[10])[0] = (int16_t)htons((uint16_t)state.cost);

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
        int32_t ID = (int32_t)ntohl(*((uint32_t*)&message[index]));

        char owner[HMSTR_MAX_NAME] = { 0 };
        char name[HMSTR_MAX_NAME] = { 0 };
        int16_t price;

        result = hmstr_readentry(ID, owner, name, &price);
        memcpy(&ResponseBuffer[4], owner, HMSTR_MAX_NAME);
        memcpy(&ResponseBuffer[4 + HMSTR_MAX_NAME], name, HMSTR_MAX_NAME);
        ((uint16_t*)&ResponseBuffer[4 + (2 * HMSTR_MAX_NAME)])[0] = (uint16_t)(htons((uint16_t)price));

        *payloadLen += (2 * HMSTR_MAX_NAME);
    }

    ((uint32_t*)ResponseBuffer)[0] = htonl(result);
    
    return result;
}

size_t readRPCMessage(int socket)
{
    //read 8 byte header
    ssize_t valread = read(socket, MessageBuffer, HAMSTER_RPC_PROT_HEADER_SIZE);

    if (valread != HAMSTER_RPC_PROT_HEADER_SIZE)
    {
        perror("invalid header");
        exit(EXIT_FAILURE);
    }

    uint16_t payloadSize = encode_uint16(MessageBuffer, HAMSTER_RPC_PROT_HEADER_PAYLOADLENGTH_OFFSET);

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
    if (readRPCMessage(socket) <= HAMSTER_RPC_PROT_HEADER_SIZE)
    {
        return false;
    }

    uint16_t messageID = encode_uint16(MessageBuffer, HAMSTER_RPC_PROT_HEADER_MSGID_OFFSET);
    uint16_t payloadLen;
    uint16_t callID = encode_uint16(MessageBuffer, HAMSTER_RPC_PROT_HEADER_CALLID_OFFSET);

    int32_t ret = functionDecoder(MessageBuffer, _MESSAGELEN, callID, &payloadLen);

    sendRPCResponse(socket, errorCheck(ret), messageID, ResponseBuffer, payloadLen, callID);

    printf("ret : %d; callID : %d; payloadLen : %d\n", (int)ret, (int)callID, (int)payloadLen);
    return true;
}

/**
 * @brief Main program
 */
int main(int argc, char* argv[])
{
    if (argc < 3 || (strcmp(argv[1], "-p")  != 0) || !isNumber(argv[2]))
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
       
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");//inet_addr("192.168.1.100");
    address.sin_port = htons(port);
       
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while (1) //wait for connections
    {
        if (!handleRequest(new_socket))
        {
            close(new_socket);

            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
        }   
    }

	return 0;
}
