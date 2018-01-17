#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/socket.h>
#include <pthread.h>

int  serveur_tcp (const char * service_name);
int  quitter_le_serveur (void);
void * traite_connexion (void *);
 
struct cnx {
    char hostname [NI_MAXHOST];
    char servname [NI_MAXSERV];
    struct cnx * next;
} dummy, *p;

int serveur_tcp (const char * nom_service)
{
    int err;
    int sock_serveur;
    int sock_connectee;
    struct addrinfo  hints;
    struct addrinfo *results;

    struct sockaddr_in * addr_in;
    socklen_t length = 0;
    char hostname [NI_MAXHOST];
    char servname [NI_MAXSERV];

    struct cnx * client;
    int i = 0;
    dummy.next = NULL;

    // Creer la socket serveur et lui attribuer un numero de service.
    if ((sock_serveur = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("socket");
        return -1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((err = getaddrinfo(NULL, nom_service,  &hints, &results)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        close(sock_serveur);
        return -1;
    }

    err = bind(sock_serveur, results->ai_addr, results->ai_addrlen);
    freeaddrinfo(results);
    if (err < 0) {
        perror("bind");
        close(sock_serveur);
        return -1;
    }

    // Afficher notre adresse locale.
    fprintf(stdout, "Mon adresse >> ");
    length = sizeof(struct sockaddr_in);
    if (getsockname(sock_serveur, (struct sockaddr *) &addr_in, &length) < 0) {
        perror ("getsockname");
        return -1;
    }
    if (getnameinfo((struct sockaddr *) &addr_in, length,
                    hostname, NI_MAXHOST,
                    servname, NI_MAXSERV,
                    NI_NUMERICHOST | NI_NUMERICSERV) == 0) 
        fprintf (stdout, "IP = %s, Port = %s \n", hostname, servname);

    listen(sock_serveur, 5);

    while (! quitter_le_serveur()) {
        sock_connectee = accept(sock_serveur, NULL, NULL);
        if (sock_connectee < 0) {
            perror("accept");
            return -1;
        }
        
        pthread_t thr;
        if (pthread_create(&thr, NULL, traite_connexion, (void *)&sock_connectee) != 0) {
            fprintf(stderr, "Erreur dans pthread_create\n");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}

int quitter_le_serveur (void)
{
    return 0;
}

void * traite_connexion (void * arg)
{
    struct sockaddr * sockaddr;
    socklen_t length = 0;
    char hostname [NI_MAXHOST];
    char servname [NI_MAXSERV];
    char buffer [256];
    int sock = *(int *)arg;

    // Afficher l'adresse du correspondant
    getpeername(sock, NULL, &length);
    if (length == 0)
        return (void *) 0;
    sockaddr = malloc(length);
    if (getpeername(sock, sockaddr, & length) < 0) {
        perror ("getpeername");
        free(sockaddr);
        return (void *) 0;
    }
    if (getnameinfo(sockaddr, length,
                    hostname, NI_MAXHOST,
                    servname, NI_MAXSERV,
                    NI_NUMERICHOST | NI_NUMERICSERV) == 0) {
        snprintf (buffer, 256, "IP = %s, Port = %s \n", hostname, servname);
        fprintf(stdout, "Connexion distante %s", buffer);
        for (p = &dummy; p->next != NULL; p = p->next) {
            write(sock, p->next, sizeof(struct cnx));
        }
        p->next = (struct cnx *) malloc (sizeof (struct cnx));
        strncpy(p->next->hostname, hostname, strlen (hostname)+1);
        strncpy(p->next->servname, servname, strlen (hostname)+1);
    }
    free(sockaddr);
    close(sock);
    return (void *) 0;
}

int main (int argc, char * argv[])
{
    return serveur_tcp ("1234");
}