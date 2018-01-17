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
    struct message * p;

    for (p=&ledger->m[0]; p<&ledger->m[LEDGER_SIZE]; p++)
    {
        printf ("%s -> %s (%lu)\n",p->t.from,p->t.to, p->t.amount);
    }
}

int main (int argc, char * argv[])
{
    int ledger_fd;
    struct ledger ledger;
    
    if (argc < 2) {
        fprintf (stderr, "Usage: %s <ledger_filename>", argv[0]);
        exit (EXIT_FAILURE);
    }

    ledger_fd = open (argv[1], O_CREAT|O_TRUNC|O_WRONLY, 0644);
    memset (&ledger, 0, sizeof (struct ledger));

    if (write (ledger_fd, &ledger, sizeof (struct ledger)) == -1) {
        perror ("write");
        exit (EXIT_FAILURE);
    }
    printf ("Wrote %s\n", argv[1]);
    return 0;
}
