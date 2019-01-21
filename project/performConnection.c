#include <stdlib.h>
#include <stdio.h>
#include "performConnection.h"
//#include "SHM.h"
//benoetigt mehr includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <sys/shm.h> // include für Shared Memory
#include <sys/ipc.h> // include für Shared Memory
#include <sys/stat.h> // include für Shared Memory

//Funktion zum Ausgeben der erhaltenen Servernachrichten
// todo: letzte recievte nachricht inm shm abspeichern
void recvServer(int SocketFD, char* buf){
    int n = 0;
    char buffer[2048];
    memset(buffer, '0',sizeof(buffer));
    n = read(SocketFD, buffer, sizeof(buffer));
    printf("Server: ");
        for(int i =  0; i<n; i++){
          printf("%c", buffer[i]);
        }
    printf("\n");
    for(int i=0; i<2048; i++){
      buf[i]=buffer[i];
    }
    memset(buffer, '0',sizeof(buffer));
}

//Funktion zum Senden von Nachrichten an den Server
void sendServer(int SocketFD, char *nachricht, int laenge){
    //send
    char buffer[laenge];
    memset(buffer, '0',sizeof(buffer));
    int n=0;
    strcpy(buffer, nachricht);
    printf("Client: %s\n", buffer); //testprint
    n=write(SocketFD, buffer, sizeof(buffer));
    if(n < 0){
        printf("\n Send error \n");
    }
}

//Funktion für Shared Memory
 struct SHmem_str{
   size_t groesse;
  int err;
} mem ;

int SHmem(int size){

  mem.groesse = size;
  mem.err=shmget(IPC_PRIVATE, mem.groesse,0);

  return mem.err;
}

//Funktion um Spielfeld in SHM übertragen
void spielfeldSchreiben(char buffer[2048]){
int i = 0;
while(i<2048){
if (buffer[i] == 1|2|3|4|5|6|7|8){
i = Spielfeldreihe(buffer[i],i);

}
}
}

//Linie de Spielfeld in SHmem
int Spielfeldreihe(int reihe,int array){
for (int i=0; i<8; i++) {
//game_data_struct_V2->spielfeld[reihe-1][i];
}
return array+8;
}




//Funktion welche die Protokollphase ausführt
void performConnection(int SocketFD, char* gId, char* pId, int shmid,struct gds *game_data_struct_V2){
game_data_struct_V2->gameover=2;
printf("Test für Struct-Übergabe %d \n",game_data_struct_V2->gameover);
/*
//SHM anbinden
struct gds *game_data_struct_V2 = shmat(shmid,NULL,0);
game_data_struct_V2=malloc(sizeof(game_data_struct_V2));
//game_data_struct_V2 = shmat(shmid,NULL,0);


//SHM testen
game_data_struct_V2->anzahl_spieler=1;
//shmdt(game_data_struct_V2);
printf("SHM Test in PerformConnection - Child PID:%d \n",game_data_struct_V2->pid_child);*/

//Serverkommunikation
    char* erhalten;
  /*  while (1) {
        recvServer(SocketFD, erhalten);       //empfaengt im jeden durchlauf die Servernachricht
        if (strncmp(erhalten, "+ MNM Gameserver", 16)==0) {
            sendServer(SocketFD, "VERSION 2.1\n", 12);    //sendet die Versionsnummer
        }else
        if (strncmp(erhalten, "+ Client version accepted", 25)==0) {
            char* gameId = malloc(sizeof(char)*17);
            strcpy(gameId, "ID ");
            strcat(gameId, gId);
            strcat(gameId, "\n");
            sendServer(SocketFD, gameId , 17);          //sendet die Game-ID
        }else
        if (strncmp(erhalten, "+ PLAYING", 9)==0) {
        }else
        if (strncmp(erhalten, "+ Game from", 11)==0) {
            sendServer(SocketFD, "PLAYER 1\n", 9);      //send player number
        }else
        if (strncmp(erhalten, "+ YOU", 5)==0) {
        }else
        if (strncmp(erhalten, "+ TOTAL", 7)==0) {
            sendServer(SocketFD, "THINKING\n", 9);      //sendet thinking
        }
        //TO BE CONTINUED
        if (strncmp(erhalten, "- ", 2)==0) {
            printf("Fehler bei der Serverkommunikation\n");
            return;      //sendet thinking
        }
    }*/
/*
//Serverkommunikation
    recvServer(SocketFD);   //gibt erste Nachricht des Servers aus
    sendServer(SocketFD, "VERSION 2.1\n", 12);    //sendet die Versionsnummer
    recvServer(SocketFD);   //gibt zweite Nachricht des Servers aus
    char* gameId = malloc(sizeof(char)*17);
    strcpy(gameId, "ID ");
    strcat(gameId, gId);
    strcat(gameId, "\n");
    sendServer(SocketFD, gameId , 17);    //sendet die ID
    recvServer(SocketFD);   //gibt dritte Nachricht des Servers aus
    recvServer(SocketFD);
    sendServer(SocketFD, "PLAYER 1\n", 9); //send player number
    recvServer(SocketFD);
    recvServer(SocketFD);
    sendServer(SocketFD, "THINKING\n", 9);
    recvServer(SocketFD);
*/
/*
    while(//gamedatastructv2->gameover==0
    ){
        recvServer(SocketFD); //empfangt Spieldaten vom Server
        kill(parent_id, SIGUSR1);        //Signal/Denkanstoß für thinker
        read(pipe);           //ließt ergegnis vom thinker
        sendServer(SocketFD, "ZUg im shm", länge spielzug); //sendet Spielzug an Server
        //ausgabe des Spielfeldes
        recvServer(SocketFD); //empfangt Antwort vom Server
    }
*/
//SHM lösen
//shmdt(game_data_struct_V2);


}
