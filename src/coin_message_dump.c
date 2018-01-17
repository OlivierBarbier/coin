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
#include "./binn.c"

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

void raw (struct message m) {
    binn *obj, *list;
    int i;

    obj = binn_object();

    binn_object_set_str (obj, "from", m.t.from);
    binn_object_set_str (obj, "to", m.t.to);
    binn_object_set_uint64 (obj, "amount", m.t.amount);

    list = binn_list();
    for (i=0; i<64; i++) {
        binn_list_add_uint8 (list, m.t.public_from[i]);
    }
    binn_object_set_list (obj, "public_from", list);
    binn_free (list);

    list = binn_list();
    for (i=0; i<64; i++) {
        binn_list_add_uint8 (list, m.t.public_to[i]);
    }
    binn_object_set_list (obj, "public_to", list);
    binn_free (list);

    binn_object_set_uint64 (obj, "nonce", m.t.nonce);

    list = binn_list();
    for (i=0; i<32; i++) {
        binn_list_add_uint8 (list, m.hash[i]);
    }
    binn_object_set_list (obj, "hash", list);
    binn_free (list);

    list = binn_list();
    for (i=0; i<64; i++) {
        binn_list_add_uint8 (list, m.sig[i]);
    }
    binn_object_set_list (obj, "sig", list);
    binn_free (list);

    list = binn_list();
    for (i=0; i<32; i++) {
        binn_list_add_uint8 (list, m.prev_hash[i]);
    }
    binn_object_set_list (obj, "prev_hash", list);
    binn_free (list);

    // send over the network or save to a file...
    write (STDOUT_FILENO, binn_ptr(obj), binn_size(obj));

    // release the buffer
    binn_free (obj);
}

void dump (int message_fd)
{
    int i, count;
    uint8_t note;
    char *list;
    int bytes;
    char buf[622];
    /* raw (m); */

    bytes = read (message_fd, buf, /*sizeof (struct message*/622);
    if (bytes == -1) {
        perror ("read");
        exit (EXIT_FAILURE);
    }

    printf ("from: %s\n", binn_object_str (buf, "from"));
    printf ("to: %s\n", binn_object_str (buf, "to"));
    printf ("amount: %llu\n", binn_object_uint64 (buf, "amount"));

    printf ("pubkey: ");
    list = binn_object_list (buf, "public_from");
    count = binn_count(list);
    for(i=1; i<=count; i++) {
      note = binn_list_uint8(list, i);
      printf ("%02x:", note&0xFF);
    }
    printf ("\n");

    printf ("pubkey: ");
    list = binn_object_list (buf, "public_to");
    count = binn_count(list);
    for(i=1; i<=count; i++) {
      note = binn_list_uint8(list, i);
      printf ("%02x:", note&0xFF);
    }
    printf ("\n");  

    printf ("nonce: %llu\n", binn_object_uint64 (buf, "nonce"));

    printf ("hash: ");
    list = binn_object_list (buf, "hash");
    count = binn_count(list);
    for(i=1; i<=count; i++) {
      note = binn_list_uint8(list, i);
      printf ("%02x:", note&0xFF);
    }
    printf ("\n"); 
    
    printf ("sig: ");
    list = binn_object_list (buf, "sig");
    count = binn_count(list);
    for(i=1; i<=count; i++) {
      note = binn_list_uint8(list, i);
      printf ("%02x:", note&0xFF);
    }
    printf ("\n"); 

    printf ("prev_hash: ");
    list = binn_object_list (buf, "prev_hash");
    count = binn_count(list);
    for(i=1; i<=count; i++) {
      note = binn_list_uint8(list, i);
      printf ("%02x:", note&0xFF);
    }
    printf ("\n"); 
}

int main (int argc, char * argv[])
{
    int message_fd;
    struct message m;
    int bytes;

    if (argc < 2) {
        fprintf (stderr, "Usage: %s <messsage_filename>", argv[0]);
        exit (EXIT_FAILURE);
    }

    message_fd = open (argv[1], O_RDONLY);
    if (message_fd == -1) {
        perror ("open");
        exit (EXIT_FAILURE);
    }
/*
    bytes = read (message_fd, &m, sizeof (struct message));
    if (bytes == -1) {
        perror ("read");
        exit (EXIT_FAILURE);
    }
raw (m);
*/
    dump (message_fd);



    return 0;
}
