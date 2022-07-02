#include "hamster.h"
#include "../libsrc/hamsterdef.h"

/**
 * @brief rpc server call for hmstr_new
 */
hmstr_rpc_new_resp* hmstr_new_1_svc(hmstr_rpc_new_req* argp, struct svc_req* req)
{
	static hmstr_rpc_new_resp ret;
	ret.uid = hmstr_new(argp->owner_name, argp->hamster_name, argp->treats);
	return &ret;
}

/**
 * @brief rpc server call for hmstr_lookup
 */
hmstr_rpc_lookup_resp* hmstr_lookup_1_svc(hmstr_rpc_lookup_req* argp, struct svc_req* req)
{
	static  hmstr_rpc_lookup_resp ret;
	ret.uid = hmstr_lookup(argp->hamster_name, argp->hamster_name);

	return &ret;
}

/**
 * @brief rpc server call for hmstr_directory
 */
hmstr_rpc_directory_resp* hmstr_directory_1_svc(hmstr_rpc_directory_req* argp, struct svc_req* req)
{
	static hmstr_rpc_directory_resp ret;
	ret.fdptr = argp->fdptr;
	ret.uid = hmstr_directory(&(ret.fdptr), argp->owner_name, argp->hamster_name);

	return &ret;
}

/**
 * @brief rpc server call for hmstr_howsdoing
 */
hmstr_rpc_howsdoing_resp* hmstr_howsdoing_1_svc(hmstr_rpc_howsdoing_req* argp, struct svc_req* req)
{
	static hmstr_rpc_howsdoing_resp ret;
	struct hmstr_state state;

	ret.zero = hmstr_howsdoing(argp->ID, &state);
	ret.cost = state.cost;
	ret.rounds = state.rounds;
	ret.treats_left = state.treats_left;

	return &ret;
}

/**
 * @brief rpc server call for hmstr_readentry
 */
hmstr_rpc_readentry_resp* hmstr_readentry_1_svc(hmstr_rpc_readentry_req* argp, struct svc_req* req)
{
	static hmstr_rpc_readentry_resp ret;
	ret.treats = hmstr_readentry(argp->ID, ret.owner_name, ret.hamster_name, &ret.price);

	return &ret;
}

/**
 * @brief rpc server call for hmstr_givetreats
 */
hmstr_rpc_givetreats_resp* hmstr_givetreats_1_svc(	hmstr_rpc_givetreats_req* argp, 
													struct svc_req* req)
{
	static hmstr_rpc_givetreats_resp ret;
	ret.treats = hmstr_givetreats(argp->uid, argp->treats);

	return &ret;
}

/**
 * @brief rpc server call for hmstr_collect
 */
hmstr_rpc_collect_resp* hmstr_collect_1_svc(hmstr_rpc_collect_req* argp, struct svc_req* req)
{
	static hmstr_rpc_collect_resp ret;
	ret.price = hmstr_collect(argp->owner_name);

	return &ret;
}