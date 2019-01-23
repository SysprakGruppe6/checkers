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
void spielfeldSchreiben(char buffer[2048],struct gds *game_data_struct_V2){
for(int i=0;i<2048;i++){
if (
(buffer[i] == '1'||buffer[i] == '2'||buffer[i] == '3'||buffer[i] == '4'||buffer[i] == '5'||buffer[i] == '6'||buffer[i] == '7'||buffer[i] == '8' )
&&
(buffer[i+2] == '*'||buffer[i+2] == 'b'||buffer[i+2] == 'w')
){

  int reihe = buffer[i] - '0';
  for (int j=0; j<16; j=j+1) {
    int k = 2;
  game_data_struct_V2->spielfeld[j][reihe-1]=buffer[i+j+k];
  k=k+1;
  }
}
}
}

//Print Spielfeld
void Spielfeldausgabe (char feld[15][8]){
  printf("Spielfeld Clientside\n");
  for (int i = 7; i>=0; i--){
          printf("%d ",i+1);
    for (int j = 0; j<15; j++){
      printf("%c",feld[j][i]);
    }
    printf("\n");
  }
printf("Spielfeld Ende\n");
}

//Funktion welche die Protokollphase ausführt
void performConnection(int SocketFD, char* gId, char* pId, int shmid,struct gds *game_data_struct_V2){
//printf("TEZZT %d",game_data_struct_V2->gameover);
game_data_struct_V2->gameover = 1;
   for (int i = 0; i<15; i++){
     for (int j = 0; j<8; j++){
         game_data_struct_V2->spielfeld[i][j]='*';
     }
   }
 //Spielfeldausgabe(game_data_struct_V2->spielfeld);
//Serverkommunikation
    char* erhalten=malloc(sizeof(char[2048]));
    while (game_data_struct_V2->gameover==1) {
        recvServer(SocketFD, erhalten);       //empfaengt im jeden durchlauf die Servernachricht
        /////PROTOKOLLPHASE-PROLOG/////
        if (strncmp(erhalten, "+ MNM Gameserver", 16)==0) {
            sendServer(SocketFD, "VERSION 2.1\n", 12);
        }else
        if (strncmp(erhalten, "+ Client version accepted", 25)==0) {
            char* gameId = malloc(sizeof(char)*17);
            strcpy(gameId, "ID ");
            strcat(gameId, gId);
            strcat(gameId, "\n");
            sendServer(SocketFD, gameId , 17);
        }else
        if (strncmp(erhalten, "+ PLAYING", 9)==0) {
        }else
        if (strncmp(erhalten, "+ Game from", 11)==0) {
            sendServer(SocketFD, "PLAYER 1\n", 9);
        }else
        if (strncmp(erhalten, "+ YOU", 5)==0) {
            //SPIELERNUMMER AUSLESEN
        }else
        if (strncmp(erhalten, "+ TOTAL", 7)==0) {
            sendServer(SocketFD, "THINKING\n", 9);
            spielfeldSchreiben(erhalten,game_data_struct_V2);
            Spielfeldausgabe(game_data_struct_V2->spielfeld);
            //1.Zug HARDCODE
        }


        /////IDLE-BEFEHLSSEQUENZ/////
        else if (strncmp(erhalten, "+ WAIT", 6)==0){
            sendServer(SocketFD, "OKWAIT\n", 6);
        }
        /////MOVE-BEFEHLSSEQUENZ/////
        else if (strncmp(erhalten, "+ MOVE", 6)==0){
            //SPIELFELD LESEN UND SIGNAL AN THINKER
        }

        /////GAMEOVER-BEFEHLSSEQUENZ/////
        else if (strncmp(erhalten, "+ GAMEOVER", 10)==0){
            printf("Spiel vorbei!\n");
            game_data_struct_V2->gameover=0;

        }

        /////REAKTION AUF SERVER-FEHLERMELDUNG/////
        else if (strncmp(erhalten, "- ", 2)==0) {
            printf("Fehler bei der Serverkommunikation\n");
            printf("Spiel automatisch verloren!\n");
            game_data_struct_V2->gameover=0;
        }
        //CASE FUER LEERE NACHRICHT VOM Server
        //EVTL MIT LOOP COUNTER
        //UM DAUERSCHLEIFE ZU VERHINDERN
    }
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
