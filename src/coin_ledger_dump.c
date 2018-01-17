#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>

#include "../include/uECC.h"

#include "../include/ledger.h"

void ledger_dump (struct ledger * ledger)
{
    int i;
    struct message * p;

    for (p=&ledger->m[0]; p<&ledger->m[10]; p++)
    {
        printf ("%s -> %s (%lu) ", p->t.from, p->t.to, p->t.amount);
        for (i=0; i<31; i++)
        printf ("%02x", p->hash[i]);

        printf (" ");

        for (i=0; i<31; i++)
        printf ("%02x", p->prev_hash[i]);
        printf ("\n");
    }
}

int main (int argc, char * argv[])
{
    int ledger_fd;
    struct ledger ledger;
    int bytes; 

    if (argc < 2) {
        fprintf (stderr, "Usage: %s <ledger_filename>", argv[0]);
        exit (EXIT_FAILURE);
    }

    ledger_fd = open (argv[1], O_RDONLY);

    bytes = read (ledger_fd, &ledger, sizeof (struct ledger));
    if (bytes == -1) {
        perror ("read");
        exit (EXIT_FAILURE);
    }

    ledger_dump (&ledger);

    close (ledger_fd);

    return 0;
}
