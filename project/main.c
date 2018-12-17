#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>     
#include "performConnection.h"
#include <sys/shm.h> // include für Shared Memory
#include <sys/ipc.h> // include für Shared Memory
#define GAMEKINDNAME "Checkers"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"


int main(int argc, char * argv[])
{

    printf("%i \n",SHmem());
    //Test für Shared Memory


    if (fork()==0){					//beginn connector
        pid_t parent_id = getppid();			//ID des Elternprozesses
	pid_t child_id = getpid();			//ID des Kindprozesses
	printf("Prozess IDS:\n");			//testprint
    	printf("child : %d parent: %d\n", child_id, parent_id);

    //Game-id und Spielernummer
    char *g;        //Variable für die Game-ID
    char *p;        //Variable für die Spielernummer
    char *c = "client.conf";
    
    const char ERR[] = "Fehler!\n";
    const char MSG[] = "Kommandozeilenparameter bitte in folgender Form angeben:\n./sysprak-client -g <GAME-ID> -p <{1,2}>\n";
    
    //Schleife für Kommandozeilenparameter
    if (argc<5) {                                               //prüft ob zu wenige Parameter angegeben wurden
        printf(ERR);
        printf(MSG);
        return -1;
    }
    int i = 1;
    while (i < argc) {
        if (strcmp("-g", argv[i])==0) {
            if (i==(argc-1)) {                                  //hinterer Existenzprüfer G-ID
                printf(ERR);
                printf(MSG);
                return -1;
            }
            g=argv[i+1];
            char spanset[] = " ";
            if (strcspn(argv[i+1], spanset) != 13) {            //prüft die länge der game-id
                printf(ERR);
                printf("Die Game-ID muss 13-stellig sein!\n");
                return -1;
            }
        }
        if (strcmp("-p", argv[i])==0) {
            if (i==(argc-1)) {                                  //hinterer Existenzprüfer SN
                printf(ERR);
                printf(MSG);
                return -1;
            }
            p=argv[i+1];
            /*char spanset[] = " ";
            if (strcspn(argv[i+1], spanset) != 1) {             //vorderer Existenzprüfer SN; überflüssig
                printf(ERR);
                printf("Kommandozeilenparameter bitte in folgender Form angeben:\n");
                printf("./sysprak-client -g <GAME-ID> -p <{1,2}>\n");
                return -1;
            }*/
            if ((strcmp("1", argv[i+1])!=0) && (strcmp("2", argv[i+1])!=0)) {   //Werteprüfer SN
                printf(ERR);
                printf(MSG);
                return -1;
            }
        }
        if(strcmp("-c", argv[i])==0){
            if(access(argv[i+1], F_OK) == -1){
                printf(ERR);
                printf("Bitte vergewisser dich, das die angegebene Config-Datei existiert");
            }
            c=argv[i+1];
        }
        i++;
    }

    //gethostbyname
    int l;  //Schleifenvariable für die IP-Liste
    struct hostent *he;
    struct in_addr **addr_list;
    // get the host info
    if ((he = gethostbyname(HOSTNAME)) == NULL) {
        printf("gethostbyname error\n");
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
    performConnection(SocketFD, g, p);      //Protokollphase

    shutdown(SocketFD, SHUT_RDWR);

    close(SocketFD);

    }//ende connector
    else {//beginn thinker
        printf("i bims eins thinker\n");
        waitpid(-1, NULL, 0);		//Parent wartet auf ende des Kindprozesses
	printf("parent out");
    }//end thinker
    return EXIT_SUCCESS;
}
