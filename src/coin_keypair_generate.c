#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "../include/uECC.h"

#include "../include/ledger.h"

struct keypair * genkp () {
    struct keypair * kp = (struct keypair *) malloc (sizeof (struct keypair));

    if ( ! uECC_make_key(kp->public, kp->private, uECC_secp256k1())) {
        return (struct keypair *) 0;
    }

    return kp;
}

/* gcc src/coin_generate_keypair.c obj/uECC.o -o bin/coin_generate_keypair */
int main (int argc, char * argv[])
{
    int keypair_fd, pubkey_fd;
    ssize_t bytes;
    struct keypair * kp;
    if (argc < 3) {
        fprintf (stderr, "Usage: %s <keypair_filename> <pubkey_filename>", argv[0]);
        exit (EXIT_FAILURE);
    }
    kp = genkp ();
    keypair_fd = open (argv[1], O_CREAT|O_TRUNC|O_WRONLY, 0644);
    if (keypair_fd == -1)
    {
        perror ("open");
        exit (EXIT_FAILURE);
    }
    bytes = write (keypair_fd, kp, sizeof (struct keypair));
    if (bytes == -1) {
        perror ("write");
        exit (EXIT_FAILURE);
    }
    printf ("Wrote %s\n", argv[1]);
    close (keypair_fd);
    pubkey_fd = open (argv[2], O_CREAT|O_TRUNC|O_WRONLY, 0644);
    if (pubkey_fd == -1)
    {
        perror ("open");
        exit (EXIT_FAILURE);
    }
    bytes = write (pubkey_fd, kp->public, 64 * sizeof (uint8_t));
    if (bytes == -1) {
        perror ("write");
        exit (EXIT_FAILURE);
    }
    printf ("Wrote %s\n", argv[2]);
    close (pubkey_fd);
    return 0;
}
