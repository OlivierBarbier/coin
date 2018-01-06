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

int verify (uint8_t * public, uint8_t * hash, uint8_t * sig) {
    return uECC_verify(public, hash, sizeof(hash), sig, uECC_secp256k1());
}

int message_verify (struct message * m) {
    return verify (m->t.public_from, m->hash, m->sig);
}

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

int ledger_add_transfer (struct ledger * ledger, struct message * m)
{
    memcpy(&(ledger->m[ledger->_tail++]), m, sizeof (struct message));
 
 printf ("@%s -> %s (%lu)\n", ledger->m[ledger->_tail-1].t.from, ledger->m[ledger->_tail-1].t.to, ledger->m[ledger->_tail-1].t.amount);   
 
    return m->t.amount;
}

uint8_t * sha256 (struct transfer * t)
{
    sha256_context ctx;
    uint8_t * hash = (uint8_t *) malloc (32 * sizeof (uint8_t));
    sha256_init (&ctx);
    sha256_hash(&ctx, (uint8_t *)t, (uint32_t) sizeof (struct transfer));
    sha256_done(&ctx, hash);
    return hash;
}

int check_proof_of_work (struct message * m)
{
    uint8_t * h;
    h = sha256 (&m->t); 
    if (h[0] ==  0 /*&& h[1] == 0 && h[2] == 0*/) {
        printf ("proof-of-work: ok\n");
        return 1;
    } else {
        printf ("proof-of-work: ko\n");
        return 0;
    }
}

int ledger_consume_message (struct ledger * ledger, struct message * m)
{   
    unsigned long total = 0;

    if ( ! message_verify (m)) {
        printf ("<%s>", m->t.from);
        return -1;
    }

    if (strcmp ("BANK", m->t.from) == 0) {
        if ( ! check_proof_of_work (m)) {
            printf ("%lu not a valid proof-of-work\n", m->t.nonce);
            return -1;        
        }
    }

    if (strcmp ("BANK", m->t.from) != 0 && (total=balance (ledger, m->t.from) < m->t.amount)) {
        printf ("%s has not enought money %lu\n", m->t.from, total);
        return -1;
    }

    ledger_add_transfer (ledger, m);

    return 0;
}

int main (int argc, char * argv[])
{
    int ledger_fd, message_fd;
    struct message m;
    int bytes;

    if (argc < 3) {
        fprintf (stderr, "Usage: %s <ledger_filename> <messsage_filename>", argv[0]);
        exit (EXIT_FAILURE);
    }

    ledger_fd = open (argv[1], O_RDWR);
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

    if (ledger_consume_message (&ledger, &m) == -1) {
        printf ("Message not consumable\n");
    } else {
        lseek (ledger_fd, 0, SEEK_SET);
        if (write (ledger_fd, &ledger, sizeof (struct ledger)) == -1) {
            perror ("write");
            exit (EXIT_FAILURE);
        }
        printf ("Wrote %s\n", argv[1]);
    }
    return 0;
}
