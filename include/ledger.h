struct transfer {
    char from[256];
    char to[256];
    unsigned long amount;
    uint8_t public_from[64];
    uint8_t public_to[64];
    unsigned long nonce;
};

struct message {
    struct transfer t;
    uint8_t hash[32];
    uint8_t sig[64];
};

struct ledger {
    struct message m[10];
    unsigned short _tail;
} ledger;

struct keypair {
    uint8_t private[32];
    uint8_t public[64];
};

struct signature_t {
    char message[1];
    char crypted_message[1];
    char public_key[32];
};