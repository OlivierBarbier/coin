
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/socket.h>

#define LG_BUFFER   1024

int lecture_arguments (int argc, char * argv[], int server, struct addrinfo **result);

int main (int argc, char * argv[])
{
    int   sock;
    char  buffer[LG_BUFFER];
    int   nb_lus;
    struct addrinfo *results;
    int message_fd;

    if (lecture_arguments(argc, argv, 0, &results) < 0)
        exit(EXIT_FAILURE);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    if (bind(sock, results->ai_addr, results->ai_addrlen) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(results);

    setvbuf(stdout, NULL, _IONBF, 0);
    while (1) {
        if ((nb_lus = recv(sock, buffer, LG_BUFFER, 0)) == 0)
            break;
        if (nb_lus < 0) {
            perror("read");
            break;
        }
        message_fd = open ("message.bin", O_CREAT|O_TRUNC|O_WRONLY, 0644);
        if (write (message_fd, buffer, nb_lus) == -1) {
            perror ("write");
            exit (EXIT_FAILURE);
        }
        printf ("Wrote %s\n", "message.bin");
        
        if (fork () == 0) {
            if (execl (
                "./bin/coin_message_consume", 
                "coin_message_consume", 
                "ledger.bin", 
                "message.bin",
                (char *) NULL) == -1) {
                perror ("excl");
                exit (EXIT_FAILURE);
            }
            exit (EXIT_SUCCESS);
        } else {
            wait (NULL);
        }

    }
    return EXIT_SUCCESS;
}


int lecture_arguments (int argc, char * argv[], int server, struct addrinfo **results)
{
    char * liste_options = "a:p:h";
    int    option;
    int    err; 
    char * hote  = "0.0.0.0";
    char * port = "7777";

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
    hints.ai_socktype = SOCK_DGRAM;
    if (server)
        hints.ai_flags = AI_PASSIVE;
    if ((err = getaddrinfo(hote, port, &hints, results)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        return -1;
    }
    return 0;
}