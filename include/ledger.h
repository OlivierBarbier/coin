#define LEDGER_SIZE 10

struct transfer {
    char from[5];
    char to[5];
    unsigned short amount;
    uint8_t public_from[64];
    uint8_t public_to[64];
    unsigned short nonce;
};

struct message {
    struct transfer t;
    uint8_t hash[32];
    uint8_t sig[64];
    uint8_t prev_hash[32];
};

struct ledger {
    struct message m[LEDGER_SIZE];
    unsigned short _tail;
} ledger;

struct keypair {
    uint8_t private[32];
    uint8_t public[64];
};
