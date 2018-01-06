#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>

#include "../include/uECC.h"

#include "../include/ledger.h"

struct transfer * transfer_create (char * from, char * to, unsigned long amount, uint8_t * public_from, uint8_t * public_to)
{
    struct transfer *t = (struct transfer *) malloc (sizeof (struct transfer));
    strcpy (t->from, from);
    strcpy (t->to, to);
    t->amount = amount;
    memcpy (t->public_from, public_from, 64 * sizeof (uint8_t));
    memcpy (t->public_to, public_to, 64 * sizeof (uint8_t));
    t->nonce = 0;
    return t;
}

int main (int argc, char * argv[])
{
    char * from, *to, pubkey_from[64], pubkey_to[64];
    int pubkey_from_fd, pubkey_to_fd, transfer_fd;
    struct transfer * t;
    
    if (argc < 7) {
        fprintf (stderr, "Usage: %s <from> <to> <pubkey_from_filename> <pubkey_to_filename> <transfer_filename> <amount>", argv[0]);
        exit (EXIT_FAILURE);
    }

    from = argv[1];
    to = argv[2];
    pubkey_from_fd = open (argv[3], O_RDONLY);
    pubkey_to_fd = open (argv[4], O_RDONLY);
    
    read (pubkey_from_fd, pubkey_from, 64);
    read (pubkey_to_fd, pubkey_to, 64);

    t = transfer_create (from, to, atoi(argv[6]), (uint8_t *)pubkey_from, (uint8_t *)pubkey_to);

    transfer_fd = open (argv[5], O_CREAT|O_TRUNC|O_WRONLY, 0644);
    printf ("%s -> %s (%d)\n", from, to, atoi(argv[6]));
    if (write (transfer_fd, t, sizeof (struct transfer)) == -1) {
        perror ("write");
        exit (EXIT_FAILURE);
    }
    printf ("Wrote %s\n", argv[5]);
    return 0;
}
