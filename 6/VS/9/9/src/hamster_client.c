#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "hamsterrest.h"

#include "curl/curl.h"

#define M_BufferLen 1023
#define M_ResponseBufferLen 16383

static const char *BaseUrl = "http://hamsteriot.vs.cs.hs-rm.de:8080/HamsterREST/";
// static CURL* CurlConnection;
static char Buffer[M_BufferLen];
static char ResponseBuffer[M_ResponseBufferLen];

size_t curlWriteFunction(char *ptr, size_t size, size_t nmemb, char *data)
{
	for (size_t i = 0; i < (size * nmemb); i++)
	{
		data[i] = ptr[i];
	}

	return size * nmemb;
}

void hrest_init(char *hostname)
{
}

void hrest_helper(const char *hostname, const char *request)
{
	assert(hostname && request);

	CURL *CurlConnection = curl_easy_init();
	assert(CurlConnection);

	assert(curl_easy_setopt(CurlConnection, CURLOPT_CUSTOMREQUEST, request) == CURLE_OK);
	assert(curl_easy_setopt(CurlConnection, CURLOPT_URL, hostname) == CURLE_OK);
	assert(curl_easy_setopt(CurlConnection, CURLOPT_FOLLOWLOCATION, 1L) == CURLE_OK);
	assert(curl_easy_setopt(CurlConnection, CURLOPT_WRITEFUNCTION, curlWriteFunction) == CURLE_OK);
	assert(curl_easy_setopt(CurlConnection, CURLOPT_WRITEDATA, ResponseBuffer) == CURLE_OK);

	assert(curl_easy_perform(CurlConnection) == CURLE_OK);
	curl_easy_cleanup(CurlConnection);

	printf("\n'%s'\n%d\n", ResponseBuffer, (int)strlen(ResponseBuffer));
}

void hrest_get(char *hostname)
{
	hrest_helper(hostname, "GET");
}

void hrest_getAndPrint(char *hostname)
{
	hrest_get(hostname);
}

void hrest_post(char *hostname)
{
	hrest_helper(hostname, "POST");
}

int hrest_delete(char *hostname)
{
	assert(hostname);

	CURL *CurlConnection = curl_easy_init();
	assert(CurlConnection);

	assert(curl_easy_setopt(CurlConnection, CURLOPT_CUSTOMREQUEST, "DELETE") == CURLE_OK);
	assert(curl_easy_setopt(CurlConnection, CURLOPT_URL, hostname) == CURLE_OK);
	assert(curl_easy_setopt(CurlConnection, CURLOPT_FOLLOWLOCATION, 1L) == CURLE_OK);
	assert(curl_easy_setopt(CurlConnection, CURLOPT_WRITEFUNCTION, curlWriteFunction) == CURLE_OK);
	assert(curl_easy_setopt(CurlConnection, CURLOPT_WRITEDATA, ResponseBuffer) == CURLE_OK);

	assert(curl_easy_perform(CurlConnection) == CURLE_OK);
	curl_easy_cleanup(CurlConnection);

	return 0;
}

void hrest_terminate(void)
{
	// curl_easy_cleanup(CurlConnection);
}

const char *getOwnerById(int id)
{
	static char name[32];
	snprintf(Buffer, M_BufferLen, "%srest/owners/%d", BaseUrl, id);
	hrest_get(Buffer);

	sscanf(ResponseBuffer, "name:%s", name);

	return name;
}

bool checkOwnerId(int id, const char *owner)
{
	const char *res = getOwnerById(id);

	return (strcmp(owner, res) == 0);
}

int getOwnerId(const char *owner)
{
	snprintf(Buffer, M_BufferLen, "%srest/owners", BaseUrl);
	hrest_get(Buffer);

	char *ch;
	ch = strtok(ResponseBuffer, " ");
	while (ch != NULL)
	{
		int id = atoi(ch);

		if (checkOwnerId(id, owner))
		{
			return id;
		}

		ch = strtok(NULL, "\n");
	}

	return -1;
}

int hrest_helper_new(const char *owner, const char *hamster_name, uint16_t treats, int *hamster_id)
{
	// snprintf(Buffer, M_BufferLen, "%srest/owners?name=%s%%20%s", BaseUrl, owner, hamster_name);
	int id = getOwnerId(owner);
	assert(id > 0);

	const char *formatString = "%srest/owners/%d/hamsters?name=%s&treats=%d";
	snprintf(Buffer, M_BufferLen, formatString, BaseUrl, id, hamster_name, treats);

	hrest_post(Buffer);

	return 0;
}

int hrest_helper_collect(const char *owner, int16_t *price)
{
	int id = getOwnerId(owner);
	assert(id > 0);

	snprintf(Buffer, M_BufferLen, "%srest/owners/%d/hamsters/%d", BaseUrl, id);
	hrest_delete(Buffer);
	return 0;
}

int hrest_helper_print_all_hamsters(void)
{
	snprintf(Buffer, M_BufferLen, "%s%s", BaseUrl, "rest/hamsters");
	hrest_get(Buffer);
	return 0;
}

int hrest_helper_print_owners_hamsters(const char *owner)
{
	assert(owner);
	snprintf(Buffer, M_BufferLen, "%srest/owners/%s", BaseUrl, owner);
	hrest_get(Buffer);
	return 0;
}

int hrest_helper_print_hamster(int id, const char *owner, const char *hamster_name, uint16_t price, uint16_t treats)
{
	printf("%-10d %-32s%-32s%-12d %-12d\n", id, owner, hamster_name, price, treats);

	return HREST_RET_OK;
}

int hrest_helper_howsdoing(int id, struct hmstr_state *state)
{
	return HREST_RET_NOT_IMPLEMENTED;
}

int hrest_helper_givetreats(int id, uint16_t treats, uint16_t *treats_left)
{

	return HREST_RET_NOT_IMPLEMENTED;
}

int hrest_helper_queryHamster(char *owner, char *hamster_name, int *hamster_id)
{

	return HREST_RET_NOT_IMPLEMENTED;
}
