#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h> //für getaddrinfo
#include "performConnection.h"
#define GAMEKINDNAME "Checkers"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
int main(int argc, char * argv[])
{

    //test
    int l;
    struct hostent *he;
    struct in_addr **addr_list;
    // get the host info
    if ((he = gethostbyname(HOSTNAME)) == NULL) {
        herror("gethostbyname");
        return 2;
    }

    // print information about this host:
    printf("Official name is: %s\n", he->h_name);
    printf("    IP addresses: ");
    addr_list = (struct in_addr **)he->h_addr_list;
    for(l = 0; addr_list[l] != NULL; l++) {
        printf("%s", inet_ntoa(*addr_list[l]));
    }
    printf("\n");


    //Game-id und Spielernummer
    char *g;
    char *p;

    //Schleife für Kommandozeilenparameter
    int i = 1;
    while (i < argc) {
        if (strcmp("-g", argv[i])==0) {
            g=argv[i+1];
            char spanset[] = " ";
            if (strcspn(argv[i+1], spanset) != 13) {
                printf("Fehler!\n");
                printf("Die Game-ID muss 13 stellig sein!\n");
                return -1;
            }
        }
        if (strcmp("-p", argv[i])==0) {
            p=argv[i+1];
            char spanset[] = " ";
            if (strcspn(argv[i+1], spanset) != 1) {
                printf("Fehler!\n");
                printf("Die Spielernummer muss 1 oder 2 sein!\n");
                return -1;
            }
            if ((strcmp("1", argv[i+1])!=0) && (strcmp("2", argv[i+1])!=0)) {
                printf("Fehler!\n");
                printf("Die Spielernummer muss 1 oder 2 sein!\n");
                return -1;
            }
        }
        i++;
    }

    //Socketvariablen
    struct sockaddr_in sa;
    int res;
    int SocketFD;

    //Socket
    SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (SocketFD == -1) {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }


    //setzt sa auf 0
    memset(&sa, 0, sizeof sa);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(PORTNUMBER);
    res = inet_pton(AF_INET, inet_ntoa(*addr_list[0]), &sa.sin_addr);                   //konvertiert die ip und speichert sie in &sa.sin_addr

    //testet die connection
    if (connect(SocketFD, (struct sockaddr *)&sa, sizeof sa) == -1) {
        perror("connect failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    /* perform read write operations ... */
    performConnection(SocketFD);

    shutdown(SocketFD, SHUT_RDWR);

    close(SocketFD);
    return EXIT_SUCCESS;
}
