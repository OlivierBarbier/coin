#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>

#include "../include/uECC.h"

#include "../include/ledger.h"

unsigned long balance (struct ledger * ledger, char * account)
{
    struct message * p;

    unsigned long credit=0, debit=0;

    for (p=&ledger->m[0]; p<&ledger->m[10]; p++)
    {
        if (strcmp (p->t.to, account) == 0)
        {
            /* transfer is a credit */
            credit = credit + p->t.amount;
        }
        if (strcmp (p->t.from, account) == 0)
        {
            /* transfer is a debit */
            debit = debit + p->t.amount;
        }
    }

    return credit - debit;
}


void ledger_dump_balance (struct ledger * ledger)
{
    struct message * p;

    for (p=&ledger->m[0]; p<&ledger->m[10]; p++)
    {
        printf ("%s (%lu)\n", p->t.to, balance (ledger, p->t.to));
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

    ledger_dump_balance (&ledger);

    close (ledger_fd);

    return 0;
}
