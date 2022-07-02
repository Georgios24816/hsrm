struct hmstr_rpc_new_req {
    char owner_name[32];
    char hamster_name[32];
    uint16_t treats;
};

struct hmstr_rpc_new_resp {
    int32_t uid;    
};

struct hmstr_rpc_lookup_req {
    char owner_name[32];
    char hamster_name[32];
};

struct hmstr_rpc_lookup_resp {
    int32_t uid;
};

struct hmstr_rpc_directory_req {
    int32_t fdptr;
	char* owner_name;
    char* hamster_name;
};

struct hmstr_rpc_directory_resp {
    int32_t uid;
    int32_t fdptr;
};

struct hmstr_rpc_howsdoing_req {
    int32_t ID;
    uint16_t treats_left;
	uint32_t rounds;
	int16_t cost;
};

struct hmstr_rpc_howsdoing_resp {
    int32_t zero;
    uint16_t treats_left;
	uint32_t rounds;
	int16_t cost;
};

struct hmstr_rpc_readentry_req {
    int32_t ID;
    char owner_name[32];
    char hamster_name[32];
    int16_t price;
};

struct hmstr_rpc_readentry_resp {
    int32_t treats;
    char owner_name[32];
    char hamster_name[32];
    int16_t price;
};

struct hmstr_rpc_givetreats_req {
    int32_t uid;
    uint16_t treats;
};

struct hmstr_rpc_givetreats_resp {
    int32_t treats;
};

struct hmstr_rpc_collect_req {
    char owner_name[32];
};

struct hmstr_rpc_collect_resp {
    int32_t price;
};

program HMSTR_PROG {
            version HMSTR_VERS {
                    hmstr_rpc_new_resp HMSTR_NEW(hmstr_rpc_new_req) = 2;
                    hmstr_rpc_lookup_resp HMSTR_LOOKUP(hmstr_rpc_lookup_req) = 3;
                    hmstr_rpc_directory_resp HMSTR_DIRECTORY(hmstr_rpc_directory_req) = 4;
                    hmstr_rpc_howsdoing_resp HMSTR_HOWSDOING(hmstr_rpc_howsdoing_req) = 5;
                    hmstr_rpc_readentry_resp HMSTR_READENTRY(hmstr_rpc_readentry_req) = 6;
                    hmstr_rpc_givetreats_resp HMSTR_GIVETREATS(hmstr_rpc_givetreats_req) = 7;
                    hmstr_rpc_collect_resp HMSTR_COLLECT(hmstr_rpc_collect_req) = 8;
            } = 1;
} = 0x23451111;