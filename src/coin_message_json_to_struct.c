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

void toJson (struct message m) {
    printf ("{\"t\": {\"from\": \"%s\", \"to\": \"%s\", \"amount\": %lu", m.t.from, m.t.to, m.t.amount);
    printf (",\"public_from\": \"");
    for (int i=0; i<64;i++) {
        printf ("%02x",  m.t.public_from[i]&0xFF);
    }
    printf ("\",\"public_to\": \"");
    for (int i=0; i<64;i++) {
        printf ("%02x",  m.t.public_to[i]&0xFF);
    }
    printf ("\",\"nonce\": %lu}", m.t.nonce);
    printf (", \"hash\": \"");
    for (int i=0; i<32;i++) {
        printf ("%02x",  m.hash[i]&0xFF);
    }
    printf ("\", \"sig\": \"");
    for (int i=0; i<64;i++) {
        printf ("%02x",  m.sig[i]&0xFF);
    }
    printf ("\", \"prev_hash\": \"");
    for (int i=0; i<32;i++) {
        printf ("%02x",  m.prev_hash[i]&0xFF);
    }
    printf ("\"}");    
}

void toCsv (struct message m) {
    /* from */
    printf("%s", m.t.from);
    /* to */
    printf(";%s", m.t.to);
    /* amount */
    printf(";%lu", m.t.amount);
    /* public_from */
    for (int i=0; i<64;i++) {
        printf (";%02x",  m.t.public_from[i]&0xFF);
    }
    /* public_to */
    for (int i=0; i<64;i++) {
        printf (";%02x",  m.t.public_to[i]&0xFF);
    }
    /* nonce */
    printf(";%lu", m.t.nonce);
    /* hash */
    for (int i=0; i<32;i++) {
        printf (";%02x",  m.hash[i]&0xFF);
    }
    /* sig */
    for (int i=0; i<64;i++) {
        printf (";%02x",  m.sig[i]&0xFF);
    }
    /* prev_hash */
    for (int i=0; i<32;i++) {
        printf (";%02x",  m.prev_hash[i]&0xFF);
    }  
}

int main (int argc, char * argv[])
{
    int message_fd;
    char * string;
    int bytes;

    string = malloc (1024);
    memset (string, '\0', 1024);

    if (argc < 2) {
        fprintf (stderr, "Usage: %s <messsage_filename>", argv[0]);
        exit (EXIT_FAILURE);
    }

    message_fd = open (argv[1], O_RDONLY);
    if (message_fd == -1) {
        perror ("open");
        exit (EXIT_FAILURE);
    }


    bytes = read (message_fd, string, sizeof (struct message));
    if (bytes == -1) {
        perror ("read");
        exit (EXIT_FAILURE);
    }
        
    char *token;
    struct message m;

    token = strtok(string, ";");
    strcpy(m.t.from, token);

    token = strtok(NULL, ";");
    strcpy(m.t.to, token);

    token = strtok(NULL, ";");
    m.t.amount = (unsigned long)atoi(token);
uint8_t n;
    for (int i=0; i<64;i++) {
        token = strtok(NULL, ";");
sscanf ( "%02x", token, &n);
printf ("%x",  n);
break;
    }



/*
    token = strtok(NULL, ";");
    m.t.nonce = (unsigned long)atoi(token);

    for (int i=0; i<32;i++) {
        token = strtok(NULL, ";");
        m.hash[i] = ((uint8_t) token[0])&0xFF;
    }

    for (int i=0; i<64;i++) {
        token = strtok(NULL, ";");
        m.sig[i] = ((uint8_t) token[0])&0xFF;
    }

    for (int i=0; i<32;i++) {
        token = strtok(NULL, ";");
        m.prev_hash[i] = ((uint8_t) token[0])&0xFF;
    }
*/

    return 0;
}
