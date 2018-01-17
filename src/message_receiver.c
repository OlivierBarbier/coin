// ------------------------------------------------------------------
// tcp-2-stdout.c
// Fichier d'exemple du livre "Developpement Systeme sous Linux"
// (C) 2000-2016 - Christophe BLAESS -Christophe.Blaess@Logilin.fr
// http://www.logilin.fr
// ------------------------------------------------------------------

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/socket.h>

#define LG_BUFFER  1024

struct peer {
    char hostname [NI_MAXHOST];
    char servname [NI_MAXSERV];
    struct cnx * next;
} peer;

int lecture_arguments (int argc, char * argv[], int server, struct addrinfo **result);

int main (int argc, char * argv[])
{
    int    sock;
    char   buffer[LG_BUFFER];
    int    nb_lus;
    struct addrinfo *results;
    
    if (lecture_arguments(argc, argv, 0, &results) < 0)
        exit(EXIT_FAILURE);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    if (connect(sock, results->ai_addr, results->ai_addrlen) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(results);

    setvbuf(stdout, NULL, _IONBF, 0);
    while (1) {
        if ((nb_lus = read(sock, &peer, sizeof (struct peer))) == 0)
            break;
        if (nb_lus < 0) {
            perror("read");
            break;
        }
        write(STDOUT_FILENO, peer.hostname, strlen (peer.hostname));
        write(STDOUT_FILENO, ":", strlen (":"));
        write(STDOUT_FILENO, peer.servname, strlen (peer.servname));
        write(STDOUT_FILENO, "\n", strlen ("\n"));
    }
    return EXIT_SUCCESS;
}


int lecture_arguments (int argc, char * argv[], int server, struct addrinfo **results)
{
    char * liste_options = "a:p:h";
    int    option;
    int    err; 
    char * hote  = "localhost";
    char * port = "2000";

    struct addrinfo  hints;

    while ((option = getopt(argc, argv, liste_options)) != -1) {
        switch (option) {
            case 'a' :
                hote  = optarg;
                break;
            case 'p' :
                port = optarg;
                break;
            case 'h' :
                fprintf(stderr, "Syntaxe : %s [-a adresse] [-p port] \n",
                        argv[0]);
                return -1;
            default : 
                break;
        }
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (server)
        hints.ai_flags = AI_PASSIVE;
    if ((err = getaddrinfo(hote, port, &hints, results)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        return -1;
    }
    return 0;
}
