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
#include <getopt.h>
#include "performConnection.h"
#include "configParser.h"
#include "ai.h"
#include <sys/shm.h> // include für Shared Memory
#include <sys/ipc.h> // include für Shared Memory
#include <sys/stat.h> // include für Shared Memory
#define ERR "Fehler!\n"
#define MSG "Kommandozeilenparameter bitte in folgender Form angeben:\n./sysprak-client -g <GAME-ID> -p <{1,2}>\n"

int main(int argc, char * argv[])
{

  //struct für Gamedaten
  typedef struct {
  char gameID[13];
  char spielernummer;
  int anzahl_spieler;
  int pid_connector;
  int pid_thinker;
}  gds;

//Struct erstellen
printf("Versuche Struct zu erstellen");
gds game_data_struct_V2;
printf("Struct erstellt");
//Test für Shared Memory
int shm_addr = SHmem(sizeof(gds));
printf("Shared Memory angebunden an %i \n",shm_addr);

// Struct in SHM speichern
//gds *game_data_struct = shmat(shm_addr,NULL,0);

//Daten im Struct speichern
//game_data_struct.anzahl_spieler = 0;


//verschiben in SHM
int moved = 0;
//moved = memmove(shmat(shm_addr,NULL,0),&game_data_struct_V2,sizeof(gds));
printf("Struct verschoben nach %i \n",moved);

//SHM lösen
//shmdt(game_data_struct);
printf("SharedMemory nicht gelöst \n");



    if (fork()==0){					//beginn connector
        pid_t parent_id = getppid();			//ID des Elternprozesses
	pid_t child_id = getpid();			//ID des Kindprozesses
	printf("Prozess IDS:\n");			//testprint
    	printf("child : %d parent: %d\n", child_id, parent_id);

    //Game-id und Spielernummer
    char *g;        //Variable für die Game-ID
    char *p;        //Variable für die Spielernummer
    char *c = "client.conf";

    char spanset[] = " ";

    int test = 1;

    //Schleife für Kommandozeilenparameter
    if (argc<5) {                                               //prüft ob zu wenige Parameter angegeben wurden
        printf(ERR);
        printf(MSG);
        return -1;
    }
    int opt;
    while ((opt=getopt(argc, argv, "g:p:c"))!=-1) {

        switch(opt){
            case 'g':
                if (strcspn(optarg, spanset) != 13) {            //prüft die länge der game-id
                    printf(ERR);
                    printf("Die Game-ID muss 13-stellig sein!\n");
                    return -1;
                }

                g=optarg;

                break;

            case 'p':
                if ((strcmp(optarg,"1")!=0) && (strcmp(optarg,"2")!=0)) {   //Werteprüfer SN
                    printf(ERR);
                    printf(MSG);
                    return -1;
                }
                p=optarg;
                break;

            case 'c':
                if(access(optarg, F_OK) == -1){ //Check ob File existiert
                    printf(ERR);
                    printf("Bitte vergewisser dich, das die angegebene Config-Datei existiert\n");
                    return -1;
                }

                c=optarg;
                break;

            case '?':                           //hinterer Existenzprüfer
                printf(ERR);
                printf(MSG);
                break;

            default:
                break;
        }

    }

    //config parameters
    struct parameters cfg = read_cfg(c);

    printf("\n\n\n TEST\n\n\n");
    printf(cfg.hostName);
    printf("\n");
    printf("%d", cfg.portNr);
    printf("\n");
    printf(cfg.gameType);
    printf("\n\n\n TEST\n\n\n");



    //gethostbyname
    int l;  //Schleifenvariable für die IP-Liste
    struct hostent *he;
    struct in_addr **addr_list;
    // get the host info
    if ((he = gethostbyname(cfg.hostName)) == NULL) {
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
    sa.sin_port = htons(cfg.portNr);
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
	think();
  waitpid(-1, NULL, 0); //Wartet auf ende des Connectors
  printf("thinker out");
    }//end thinker
    return EXIT_SUCCESS;
}
