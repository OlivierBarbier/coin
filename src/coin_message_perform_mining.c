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

unsigned long  perform_mining (struct message * m) {
    unsigned long i;
    uint8_t * h;
    
    i = 0;
    do {
        m->t.nonce = i++;
        h = sha256 (&m->t); 
        if (h[0] ==  0 /*&& h[1] == 0 && h[2] == 0*/) {
            printf ("nonce=%lu\n", i-1);
            break;
        }
        if (i%50000==0) printf ("<%lu>\n", i);
    } while (1);

    return i;
}

int main (int argc, char * argv[])
{
    int ledger_fd, message_fd, mined_message_fd;
    struct message m;
    int bytes;

    if (argc < 4) {
        fprintf (stderr, "Usage: %s <ledger_filename> <messsage_filename> <mined_messsage_filename>", argv[0]);
        exit (EXIT_FAILURE);
    }

    ledger_fd = open (argv[1], O_RDONLY);
    if (ledger_fd == -1) {
        perror ("open");
        exit (EXIT_FAILURE);
    }
    bytes = read (ledger_fd, &ledger, sizeof (struct ledger));
    if (bytes == -1) {
        perror ("read");
        exit (EXIT_FAILURE);
    }

    message_fd = open (argv[2], O_RDONLY);
    if (message_fd == -1) {
        perror ("open");
        exit (EXIT_FAILURE);
    }
    bytes = read (message_fd, &m, sizeof (struct message));
    if (bytes == -1) {
        perror ("read");
        exit (EXIT_FAILURE);
    }

    perform_mining (&m);

    mined_message_fd = open (argv[3], O_CREAT|O_TRUNC|O_WRONLY, 0644);
    if (write (mined_message_fd, &m, sizeof (struct message)) == -1) {
        perror ("write");
        exit (EXIT_FAILURE);
    }
    printf ("Wrote %s\n", argv[3]);

    return 0;
}
