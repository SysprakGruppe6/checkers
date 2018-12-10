#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "performConnection.h"
#define GAMEKINDNAME "Checkers"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
int main(int argc, char * argv[])
{
    //Test
    //Game-id und Spielernummer
    char *g;        //Variable für die Game-ID
    char *p;        //Variable für die Spielernummer

    //Schleife für Kommandozeilenparameter
    if (argc<5) {                                               //prüft ob zu wenige Parameter angegeben wurden
        printf("Fehler!\n");
        printf("Kommandozeilenparameter bitte in folgender Form angeben:\n");
        printf("./sysprak-client -g <GAME-ID> -p <{1,2}>\n");
        return -1;
    }
    int i = 1;
    while (i < argc) {
        if (strcmp("-g", argv[i])==0) {
            if (i==(argc-1)) {                                  //hinterer Existenzprüfer G-ID
                printf("Fehler!\n");
                printf("Kommandozeilenparameter bitte in folgender Form angeben:\n");
                printf("./sysprak-client -g <GAME-ID> -p <{1,2}>\n");
                return -1;
            }
            g=argv[i+1];
            char spanset[] = " ";
            if (strcspn(argv[i+1], spanset) != 13) {            //prüft die länge der game-id
                printf("Fehler!\n");
                printf("Die Game-ID muss 13-stellig sein!\n");
                return -1;
            }
        }
        if (strcmp("-p", argv[i])==0) {
            if (i==(argc-1)) {                                  //hinterer Existenzprüfer SN
                printf("Fehler!\n");
                printf("Kommandozeilenparameter bitte in folgender Form angeben:\n");
                printf("./sysprak-client -g <GAME-ID> -p <{1,2}>\n");
                return -1;
            }
            p=argv[i+1];
            /*char spanset[] = " ";
            if (strcspn(argv[i+1], spanset) != 1) {             //vorderer Existenzprüfer SN; überflüssig
                printf("Fehler!\n");
                printf("Kommandozeilenparameter bitte in folgender Form angeben:\n");
                printf("./sysprak-client -g <GAME-ID> -p <{1,2}>\n");
                return -1;
            }*/
            if ((strcmp("1", argv[i+1])!=0) && (strcmp("2", argv[i+1])!=0)) {   //Werteprüfer SN
                printf("Fehler!\n");
                printf("Kommandozeilenparameter bitte in folgender Form angeben:\n");
                printf("./sysprak-client -g <GAME-ID> -p <{1,2}>\n");
                return -1;
            }
        }
        i++;
    }


    //gethostbyname
    int l;  //Schleifenvariable für die IP-Liste
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
    res = inet_pton(AF_INET, inet_ntoa(*addr_list[0]), &sa.sin_addr);   //konvertiert die ip und speichert sie in &sa.sin_addr

    //connect
    if (connect(SocketFD, (struct sockaddr *)&sa, sizeof sa) == -1) {
        perror("connect failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }


    /* perform read write operations ... */
    performConnection(SocketFD);      //Protokollphase

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*
    int n = 0;

    char recvBuff[128];    //Buffer

    //Print Server version
    memset(recvBuff, '0',sizeof(recvBuff)); //Buffer wird mit 0 initialisiert
    n = recv(SocketFD, recvBuff, sizeof(recvBuff)-1, 0); //Schleife gibt den gesamten Inhalt des Buffers aus
    if(fputs(recvBuff, stdout) == EOF)
    {
        printf("\n Error : Fputs error\n");
    }
    if(n < 0)
    {
        printf("\n Recv error \n");
    }

    printf("\n");
    //send client version
    memset(recvBuff, '0',sizeof(recvBuff)); //Buffer wird mit 0 initialisiert
    strcpy(recvBuff, "VERSION 2.3\n");
    printf("%s\n", recvBuff); //testprint

    n=send(SocketFD, "VERSION 2.3\n", sizeof("VERSION 2.3\n"), 0);
    if(n < 0){
        printf("\n Send error \n");
    }

    //get Frage nach Game-ID
    memset(recvBuff, '0',sizeof(recvBuff)); //Buffer wird mit 0 initialisiert

    n = recv(SocketFD, recvBuff, sizeof(recvBuff)-1, 0); //Schleife gibt den gesamten Inhalt des Buffers aus
    if(fputs(recvBuff, stdout) == EOF)
    {
        printf("\n Error : Fputs error\n");
    }
    if(n < 0)
    {
        printf("\n Recv error \n");
    }

    printf("\n");
    //send game ID
    memset(recvBuff, '0',sizeof(recvBuff)); //Buffer wird mit 0 initialisiert
    strcpy(recvBuff, "ID 3d1oibv5qj0se\n");
    printf("%s\n", recvBuff); //testprint
    n=send(SocketFD, "ID 3d1oibv5qj0se\n", sizeof("ID 3d1oibv5qj0se\n"), 0);
    if(n < 0){
        printf("\n Send error \n");
    }

    //get playing gamekind name
    memset(recvBuff, '0',sizeof(recvBuff)); //Buffer wird mit 0 initialisiert

    n = recv(SocketFD, recvBuff, sizeof(recvBuff)-1, 0); //Schleife gibt den gesamten Inhalt des Buffers aus
    if(fputs(recvBuff, stdout) == EOF)
    {
        printf("\n Error : Fputs error\n");
    }
    if(n < 0)
    {
        printf("\n Recv error \n");
    }
    */
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    shutdown(SocketFD, SHUT_RDWR);

    close(SocketFD);
    return EXIT_SUCCESS;
}
