#include <stdlib.h>
#include <stdio.h>
#include "performConnection.h"
#include <wait.h>
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
 void performConnection(int SocketFD, char* gId, char* pId, int shmid,struct gds *game_data_struct_V2, int pipe){

   for (int i = 0; i<15; i++){
     for (int j = 0; j<8; j++){
         game_data_struct_V2->spielfeld[i][j]='*';
     }
}

 //Serverkommunikation
     char* erhalten=malloc(sizeof(char[2048]));//BUFFER fuer erhaltene Nachrichten
     char* pipebuffer=malloc(sizeof(char[64]));//BUFFER für die PIPE
     strcpy(pipebuffer, "");
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
             //Wunschpiuelernummer aus shm senden
         }else
         if (strncmp(erhalten, "+ YOU", 5)==0) {
             //SPIELERNUMMER AUSLESEN
         }else
         if (strncmp(erhalten, "+ TOTAL", 7)==0) {
           spielfeldSchreiben(erhalten,game_data_struct_V2);
           Spielfeldausgabe(game_data_struct_V2->spielfeld);
             sendServer(SocketFD, "THINKING\n", 9);
             //Spielfeld in shm stopfen
         }else
         if(strncmp(erhalten, "+ OKTHINK", 9)==0){

             kill(getppid(), SIGUSR1);       //Signal/Denkanstoß für thinker
             read(pipe, pipebuffer, 64);
             //laenge des Spielzuges berechnen
             //sendServer();
             //1.Zug Hell -> HARDCODE C3:D4
             // sonstiger ZUG -> KI
         }


         /////IDLE-BEFEHLSSEQUENZ/////
         else if (strncmp(erhalten, "+ WAIT", 6)==0){
             sendServer(SocketFD, "OKWAIT\n", 6);
         }

         /*/////MOVE-BEFEHLSSEQUENZ/////
         else if (strncmp(erhalten, "+ MOVE", 6)==0){

             //SPIELFELD LESEN UND SIGNAL AN THINKER
         }
         */
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

}
