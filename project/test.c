#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h> //für getaddrinfo
#define GAMEKINDNAME "Checkers"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
int main(int argc, char * argv[])
{
    
    //Game-id und Spielernummer
    char *g;
    char *p;
    
    //Schleife für Kommandozeilenparameter
    int i = 1;
    while (i < argc) {
        if (strcmp("-g", argv[i])==0) {
            g=argv[i+1];
        }
        if (strcmp("-p", argv[i])==0) {
            p=argv[i+1];
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
    
    
    //Speichershit
    memset(&sa, 0, sizeof sa);
    
    sa.sin_family = AF_INET;
    sa.sin_port = htons(PORTNUMBER);
    res = inet_pton(AF_INET, "10.155.92.35", &sa.sin_addr);                   //konvertiert die ip und speichert sie in &sa.sin_addr
    
    //testet die connection
    if (connect(SocketFD, (struct sockaddr *)&sa, sizeof sa) == -1) {
        perror("connect failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
    
    /* perform read write operations ... */
    
    shutdown(SocketFD, SHUT_RDWR);
    
    close(SocketFD);
    return EXIT_SUCCESS;
}

