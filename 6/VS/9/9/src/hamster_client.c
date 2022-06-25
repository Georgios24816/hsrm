#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hamsterrest.h"

#include "curl/curl.h"

#define M_BufferLen 1023
#define M_ResponseBufferLen 16383

static const char* BaseUrl = "http://hamsteriot.vs.cs.hs-rm.de:8080/HamsterREST/";
//static CURL* CurlConnection;
static char Buffer[M_BufferLen];
static char ResponseBuffer[M_ResponseBufferLen];

size_t curlWriteFunction(char *ptr, size_t size, size_t nmemb, char* data)
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

void hrest_get(char* hostname)
{
	assert(hostname);

	CURL* CurlConnection = curl_easy_init();
	assert(CurlConnection);

	assert(curl_easy_setopt(CurlConnection, CURLOPT_URL, hostname) == CURLE_OK);
	assert(curl_easy_setopt(CurlConnection, CURLOPT_FOLLOWLOCATION, 1L) == CURLE_OK);
	assert(curl_easy_setopt(CurlConnection, CURLOPT_HTTPGET, 1L) == CURLE_OK);
	assert(curl_easy_setopt(CurlConnection, CURLOPT_WRITEFUNCTION, curlWriteFunction) == CURLE_OK);
	assert(curl_easy_setopt(CurlConnection, CURLOPT_WRITEDATA, ResponseBuffer) == CURLE_OK);

	assert(curl_easy_perform(CurlConnection) == 200);
	curl_easy_cleanup(CurlConnection);
}

void hrest_getAndPrint(char* hostname)
{
	hrest_get(hostname);
	printf("\n'%s'\n%d\n", ResponseBuffer, (int)strlen(ResponseBuffer));
}

void hrest_post(char* hostname, char* message, size_t len)
{
	assert(hostname && message && len);

}

int hrest_delete(char* hostname)
{
	assert(hostname);
	
	CURL* CurlConnection = curl_easy_init();
	assert(CurlConnection);

	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "DELETE");
	assert(curl_easy_setopt(CurlConnection, CURLOPT_URL, hostname) == CURLE_OK);
	assert(curl_easy_setopt(CurlConnection, CURLOPT_FOLLOWLOCATION, 1L) == CURLE_OK);
	assert(curl_easy_setopt(CurlConnection, CURLOPT_WRITEFUNCTION, curlWriteFunction) == CURLE_OK);
	assert(curl_easy_setopt(CurlConnection, CURLOPT_WRITEDATA, ResponseBuffer) == CURLE_OK);

	assert(curl_easy_perform(CurlConnection) == 200);
	curl_easy_cleanup(CurlConnection);
}

void hrest_terminate(void) 
{
	//curl_easy_cleanup(CurlConnection);
}

int hrest_helper_new(const char* owner, const char* hamster_name, uint16_t treats, int* hamster_id)
{
	snprintf(Buffer, M_BufferLen, "%srest/owners?name=%s%%20%s", BaseUrl, owner, hamster_name);

	return HREST_RET_NOT_IMPLEMENTED;
}

int hrest_helper_collect(const char* owner, int16_t* price) 
{

	return HREST_RET_NOT_IMPLEMENTED;
}

int hrest_helper_print_all_hamsters(void)
{
	snprintf(Buffer, M_BufferLen, "%s%s", BaseUrl, "rest/hamsters");
	hrest_getAndPrint(Buffer);

	return 0;
}

int hrest_helper_print_owners_hamsters(const char* owner)
{
	assert(owner);
	snprintf(Buffer, M_BufferLen, "%srest/owners/%s", BaseUrl, owner);
	hrest_getAndPrint(Buffer);

	return 0;
}

int hrest_helper_print_hamster(int id, const char* owner, const char* hamster_name, uint16_t price, uint16_t treats)
{
	printf("%-10d %-32s%-32s%-12d %-12d\n", id, owner, hamster_name, price, treats);

	return HREST_RET_OK;
}

int hrest_helper_howsdoing(int id, struct hmstr_state* state)
{
	
	return HREST_RET_NOT_IMPLEMENTED;
}

int hrest_helper_givetreats(int id, uint16_t treats, uint16_t* treats_left )
{

	return HREST_RET_NOT_IMPLEMENTED;
}

int hrest_helper_queryHamster(char* owner, char* hamster_name, int* hamster_id)
{

	return HREST_RET_NOT_IMPLEMENTED;
}
