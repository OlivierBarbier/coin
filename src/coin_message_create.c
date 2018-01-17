#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>

#include "../include/sha256.h"
#include "../include/uECC.h"
#include "../include/ledger.h"

uint8_t * sha256 (struct transfer * t)
{
    sha256_context ctx;
    uint8_t * hash = (uint8_t *) malloc (32 * sizeof (uint8_t));
    sha256_init (&ctx);
    sha256_hash(&ctx, (uint8_t *)t, (uint32_t) sizeof (struct transfer));
    sha256_done(&ctx, hash);
    return hash;
}

uint8_t * sign (uint8_t * private, uint8_t * hash) {

    uint8_t * sig = (uint8_t *) malloc (64 * sizeof (uint8_t));

    if ( ! uECC_sign (private, hash, sizeof(hash), sig, uECC_secp256k1())) {
        printf("uECC_sign() failed\n");
        return (uint8_t *) 0;
    }

    return sig;
}

struct message * message_create (struct transfer * t, uint8_t * private) {
    struct message * m = (struct message *) malloc (sizeof (struct message));
    memcpy (&(m->t), t, sizeof (struct transfer));
    memcpy (m->hash, sha256 (t), 32 * sizeof (uint8_t));
    memcpy (m->sig, sign (private, m->hash), 64 * sizeof (uint8_t));   
    memcpy (m->prev_hash, m->hash, 32 * sizeof (uint8_t));  
    return m;   
}

int main (int argc, char * argv[])
{
    int keypair_fd, transfer_fd, message_fd;
    struct transfer t;
    struct keypair kp;
    struct message * m;
    int bytes;

    if (argc < 4) {
        fprintf (stderr, "Usage: %s <transfer_filename> <keypair_filename> <messsage_filename>", argv[0]);
        exit (EXIT_FAILURE);
    }

    transfer_fd = open (argv[1], O_RDONLY);
    if (transfer_fd == -1) {
        perror ("open");
        exit (EXIT_FAILURE);
    }
    keypair_fd = open (argv[2], O_RDONLY);
    if (keypair_fd == -1) {
        perror ("open");
        exit (EXIT_FAILURE);
    }   
    bytes = read (transfer_fd, &t, sizeof (struct transfer));
    if (bytes == -1) {
        perror ("read");
        exit (EXIT_FAILURE);
    }
    bytes = read (keypair_fd, &kp, sizeof (struct keypair));
    if (bytes == -1) {
        perror ("read");
        exit (EXIT_FAILURE);
    }

    m = message_create (&t, kp.private);

    message_fd = open (argv[3], O_CREAT|O_TRUNC|O_WRONLY, 0644);
    if (write (message_fd, m, sizeof (struct message)) == -1) {
        perror ("write");
        exit (EXIT_FAILURE);
    }
    printf ("Wrote %s\n", argv[3]);
    return 0;
}

