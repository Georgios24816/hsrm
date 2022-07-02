#include "hamster.h"
#include "string.h"
#include <stdio.h>

//global/static Client variable
CLIENT*  Client;

/**
 * @brief create RPC-Client
 * 
 * @param hostname hostname for client
 */
void hmstr_rpc_init(char* hostname)
{
	Client = clnt_create(hostname, HMSTR_PROG, HMSTR_VERS, "udp");

	if (!Client)
	{
		clnt_pcreateerror(hostname);
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief close client
 * 
 */
void hmstr_rpc_terminate(void)
{
	clnt_destroy(Client);
}

/**
 * @brief create new hamster
 * 
 * @param owner  owner name
 * @param hamster hamster name
 * @param treats treat count
 * @return int32_t success : hamster id ; error : error code
 */
int32_t hmstr_new(const char* owner, const char* hamster, uint16_t treats) 
{
	hmstr_rpc_new_req req;
	hmstr_rpc_new_resp* resp;

	if (owner && hamster)
	{
		memcpy(req.owner_name, owner, 31);
		memcpy(req.hamster_name, hamster, 31);
	}

	req.treats = treats;
	resp = hmstr_new_1(&req, Client);
	return resp->uid;
}

/**
 * @brief lookup hamster
 * 
 * @param owner_name owner name
 * @param hamster_name hamster name
 * @return int32_t success : hamster id ; error : error code
 */
int32_t hmstr_lookup(const char* owner_name, const char* hamster_name)
{
	hmstr_rpc_lookup_req req;
	hmstr_rpc_lookup_resp* resp;

	if (owner_name && hamster_name)
	{
		memcpy(req.owner_name, owner_name, 31);
		memcpy(req.hamster_name, hamster_name, 31);
	}

	resp = hmstr_lookup_1(&req, Client);
	return resp->uid;
}

/**
 * @brief search database one entry at a time
 * 
 * @param fdptr file pointer ; -1 on first call
 * @param owner_name owner name
 * @param hamster_name hamster name
 * @return int32_t success : hamster id ; error : error code
 */
int32_t hmstr_directory(
	int32_t* fdptr,	/**< pointer where to store the open file handle across calls */
	const char* owner_name,		/**< name of hamster's owner */
	const char* hamster_name	/**< name of hamster         */
	)
{
	hmstr_rpc_directory_req req;
	hmstr_rpc_directory_resp* resp;

	//printf("START\nhmstr_directory : %d TEST\n ", *fdptr);

	req.owner_name = (char*)owner_name;
	req.hamster_name = (char*)hamster_name;
	req.fdptr = *fdptr;
	resp = hmstr_directory_1(&req, Client);

	*fdptr = req.fdptr;
	//printf("ownerName : '%s'\n", req.owner_name);
	//printf("hamsterName : '%s'\n", req.hamster_name);
	printf("END\nhmstr_directory : %d | %d\n", (int)resp->fdptr, (int)resp->uid);
	return resp->uid;
}

struct hmstr_state
{
	uint16_t treats_left;	/**< How many treats are left in stock? */
	uint32_t rounds;		/**< Number of hamster wheel revolutions */
	int16_t cost;			/**< expenses accumulated so far */
};

/**
 * @brief get hamster state
 * 
 * @param ID hamster id
 * @param state state output
 * @return int32_t success : 0 ; error : error code
 */
int32_t hmstr_howsdoing(
	int32_t ID,
	struct hmstr_state* state
	)
{
	if (!state)
	{
		exit(EXIT_FAILURE);
	}

	hmstr_rpc_howsdoing_req req;
	hmstr_rpc_howsdoing_resp* resp;

	req.ID = ID;
	req.cost = state->cost;
	req.rounds = state->rounds;
	req.treats_left = state->treats_left;
	resp = hmstr_howsdoing_1(&req, Client);

	if (!resp)
	{
		exit(EXIT_FAILURE);
	}

	state->cost = resp->cost;
	state->rounds = resp->rounds;
	state->treats_left = resp->treats_left;
	return resp->zero;
}

/**
 * @brief get hamster info using id
 * 
 * @param ID hamster id
 * @param owner owner name output
 * @param name hamster name output
 * @param price cost output
 * @return int32_t success : hamster treats left ; error : error code
 */
int32_t hmstr_readentry(
	int32_t ID,		/**< Hamster's unique ID             */
	char* owner,    /**< Where to store name of Owner    */
	char* name,   	/**< Where to store name of Hamster  */
	int16_t*  price	/**< Where to store cost (up to now) */
	)
{
	hmstr_rpc_readentry_req req;
	hmstr_rpc_readentry_resp* resp;

	memcpy(req.owner_name, owner, 31);
	memcpy(req.hamster_name, name, 31);

	req.ID = ID;
	req.price = *price;
	resp = hmstr_readentry_1(&req, Client);
	return resp->treats;
}

/**
 * @brief give treats to hamster
 * 
 * @param ID hamster id
 * @param treats treats to feed to hamster
 * @return int32_t success : treats left ; error : error code
 */
int32_t hmstr_givetreats(
	int32_t ID,	/**< Hamster's unique ID */
	uint16_t treats	/**< How many treats to feed */
	)
{
	hmstr_rpc_givetreats_req req;
	hmstr_rpc_givetreats_resp* resp;

	req.uid = ID;
	req.treats = treats;
	resp = hmstr_givetreats_1(&req, Client);
	return resp->treats;
}

/**
 * @brief collect hamster from cage
 * 
 * @param owner_name owner name
 * @return int32_t success : price ; error : error code
 */
int32_t hmstr_collect(
	const char *owner_name		/**< name of hamster owner */
	)
{
	hmstr_rpc_collect_req req;
	hmstr_rpc_collect_resp* resp;

	memcpy(req.owner_name, owner_name, 31);
	printf("DEBUG: hmstr_collect: owner_name : '%s'\n", req.owner_name);
	resp = hmstr_collect_1(&req, Client);
	return resp->price;
}
