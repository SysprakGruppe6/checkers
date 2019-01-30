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
#include "configParser.h"
#include "ai.h"
#include <signal.h>
#include <sys/shm.h> // include für Shared Memory
#include <sys/ipc.h> // include für Shared Memory
#include <sys/stat.h> // include für Shared Memory
#define ERR "Fehler!\n"
#define MSG "Kommandozeilenparameter bitte in folgender Form angeben:\n./sysprak-client -g <GAME-ID> -p <{1,2}>\n"

int main(int argc, char * argv[])
{
//Struct erstellen
printf("\n"); //ACHTUNG NICHT LÖSCHEN
//struct gds *game_data_struct_V2;
//game_data_struct_V2 = malloc(sizeof(struct gds));
//game_data_struct_V2->anzahl_spieler = 0;
//Test für Shared Memory
int shm_addr = SHmem(sizeof(struct gds));
//int *SHM = shmat(shm_addr,NULL,0);
//SHM = malloc(sizeof(game_data_struct_V2));

//verschieben in SHM
//memmove(SHM,&game_data_struct_V2,sizeof(game_data_struct_V2));

    int pfd[2];
    pipe(pfd);

    //game_data_struct_V2->gameover=1;

    if (fork()==0){					//beginn connector


      struct gds *gameData, *ptr;

        if((ptr = (struct gds*) shmat(shm_addr, NULL, 0)) == (struct gds*) -1){
            perror("shmat child");
            return -1;
        }

        gameData = ptr;

        gameData->gameover=1;


         pid_t parent_id = getppid();			//ID des Elternprozesses
         gameData->pid_parent = parent_id;
         close(pfd[1]);           //Schliessen der Schreibseite der pipe
	       pid_t child_id = getpid();			//ID des Kindprozesses
         gameData->pid_child = child_id;
	       printf("Prozess IDS:\n");			//testprint
    	   printf("child : %d parent: %d\n",gameData->pid_child,gameData->pid_parent);

    //Game-id und Spielernummer
    char *g;        //Variable für die Game-ID
    char p;        //Variable für die Spielernummer
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
                p=optarg[0]-1;
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

    //Put command line parameters into SMH

    strcpy(gameData->gameID, g);

    //config parameters
    struct parameters cfg = read_cfg(c);
    printf("%s \n",cfg.hostName);
    printf("\n");
    printf("%d", cfg.portNr);
    printf("\n");
    printf("%s \n",cfg.gameType);



    //////////GETHOSTBYNAME//////////
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

    gameData->spielernummer=p;
    //////////SOCKET//////////
    struct sockaddr_in sa;
    int res;
    int SocketFD;
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

    //////////CONNECT//////////
    if (connect(SocketFD, (struct sockaddr *)&sa, sizeof sa) == -1) {
        perror("connect failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }


    //////////PROTOKOLLPHASE//////////
    //game_data_struct_V2->gameover = 1;
    performConnection(SocketFD, gameData, pfd[0]);

    shutdown(SocketFD, SHUT_RDWR);

    close(SocketFD);

    }//ende connector

    //////////THINKER//////////
    else {

      struct gds *gameData, *ptr;

        if((ptr = (struct gds*) shmat(shm_addr, NULL, 0)) == (struct gds*) -1){
            perror("shmat parent");
            return -1;
        }

        gameData = ptr;

        gameData->gameover=1;

        printf("i bims eins thinker %i \n",gameData->gameover==1);
        close(pfd[0]);// Schliessen der Leseseite

        char test[9]="PiPeTeSt\n";
        //SCHLEIFE, DIE SOLANGE DAS SPIEL LAEUFT AUF DEM SIGNAL THINK() AUFRUFT
        while(gameData->gameover==1){
          printf("i bims eins thinker V2\n");
            signal(SIGUSR1, my_handler);
            pause();
              printf("i bims eins thinker 3\n");
            think(gameData);
            write(pfd[1], test, sizeof(test));
//            write(pfd[1], game_data_struct_V2->currentMove, sizeof(game_data_struct_V2->currentMove));//Schreibt Testmove in die pipe
        }

        waitpid(-1, NULL, 0); //Wartet auf ende des Connectors
        printf("thinker out");
    }//end thinker

    //SHM lösen
    shmdt(shm_addr);
    printf("SharedMemory gelöst \n");


    return EXIT_SUCCESS;
}
