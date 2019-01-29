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
int j = 1;
printf("Spielfeld wird eingegeben\n");
int line = 0;
for(int i=0;i<2048;i++){
if (
(buffer[i] == '1'||buffer[i] == '2'||buffer[i] == '3'||buffer[i] == '4'||buffer[i] == '5'||buffer[i] == '6'||buffer[i] == '7'||buffer[i] == '8' )
&&
(buffer[i+2] == '*'||buffer[i+2] == 'b'||buffer[i+2] == 'w')
){
  line++;
  i++;
  for(int v=0;v<16;v=v+1){
    if(v%4==1){
        if(line%2==0){
      game_data_struct_V2->spielfeld[j]=buffer[i+v];
      j++;
    }
  }
  if(v%4==3){
    if(line%2==1){
    game_data_struct_V2->spielfeld[j]=buffer[i+v];
    j++;
  }
}
}
}
}
}


//Verarbeitet Zug im SHM-Spielfeld, zugtyp -> geschlagen oder nicht
void SpielfeldZug(int zugtyp,char Zug[39],struct gds *game_data_struct_V2){
// Falls Zug = leer -> terminiert nicht
//länge des Spielzugs herausfinden
int i = 4;
//while(Zug[i]=!' '){
//i++;
//}
//Spielfeld ändern im Falle Zug ohne Schlagen
int Stelle1 = ((8-(Zug[1]-'0'))*4)+(SpielfeldUmwandeln(Zug[0]));
int Stelle2= ((8-(Zug[i]-'0'))*4)+(SpielfeldUmwandeln(Zug[i-1]));
printf(" Text an stelle %d \n",Stelle1);
printf(" Text an stelle %d  \n",Stelle2);
char temp;
temp = game_data_struct_V2->spielfeld[Stelle2];
game_data_struct_V2->spielfeld[Stelle2]
=
game_data_struct_V2->spielfeld[Stelle1];
game_data_struct_V2->spielfeld[Stelle1] = temp;
//Spielfeldausgabe?

}



//Print Spielfeld
void Spielfeldausgabe (char feld[32]){
  printf("Spielfeld Clientside\n");
  int x = 1;
  for (int i = 0; i<8; i++){
          printf("%d ",(8-i));
    for (int j = 1; j<5; j++){
      if((i%2)==0){
        printf(" ~  %c ",feld[x]);
      }
      else{
        printf(" %c  ~ ",feld[x]);

      }
      x++;
    }
    printf("\n");
  }
printf("Spielfeld Ende\n");
}

//Spielfeld Buchstaben Umwandeln
int SpielfeldUmwandeln(char eingabe){
int i = 0;
switch(eingabe) {
	case 'A': i=1; break;
	case 'B': i=1; break;
  case 'C': i=2; break;
  case 'D': i=2; break;
  case 'E': i=3; break;
  case 'F': i=3; break;
  case 'G': i=4; break;
  case 'H': i=4; break;
	default: i=0; break;
}

return i;
}
 //Funktion welche die Protokollphase ausführt
 void performConnection(int SocketFD, struct gds *game_data_struct_V2, int pipe){
    for (int i = 1; i<33; i++){
      game_data_struct_V2->spielfeld[i]='*';
    }
    game_data_struct_V2->spielfeld[4] = 'w';
    Spielfeldausgabe(game_data_struct_V2->spielfeld);
    SpielfeldZug(1,"H8:G7",game_data_struct_V2);
    Spielfeldausgabe(game_data_struct_V2->spielfeld);
    //Serverkommunikation
        char* erhalten=malloc(sizeof(char[2048]));//BUFFER fuer erhaltene Nachrichten
        char* pipebuffer=malloc(sizeof(char[64]));//BUFFER für die PIPE
        strcpy(pipebuffer, "");
        int protokollphasenendenchecker =1;
        while (game_data_struct_V2->gameover==1) {
            recvServer(SocketFD, erhalten);       //empfaengt im jeden durchlauf die Servernachricht
            /////PROTOKOLLPHASE-PROLOG/////
            if (strncmp(erhalten, "+ MNM Gameserver", 16)==0) {
                sendServer(SocketFD, "VERSION 2.1\n", 12);
            }else
            if (strncmp(erhalten, "+ Client version accepted", 25)==0) {
                char* gameId = malloc(sizeof(char)*17);
                strcpy(gameId, "ID ");
                strncat(gameId, game_data_struct_V2->gameID, 13);
                strcat(gameId, "\n");
                sendServer(SocketFD, gameId , 17);
            }else
            if (strncmp(erhalten, "+ PLAYING", 9)==0) {
            }else
            if (strncmp(erhalten, "+ Game from", 11)==0) {

              char* spielid = malloc(sizeof(char)*9);
              strcpy(spielid, "PLAYER ");
              spielid[7]=game_data_struct_V2->spielernummer;
              strcat(spielid, "\n");
              sendServer(SocketFD, spielid , 9);
            }else
            if (strncmp(erhalten, "+ YOU", 5)==0) {
                game_data_struct_V2->spielernummer=erhalten[6];
            }else
            if (strncmp(erhalten, "+ TOTAL", 7)==0) {
              //SPIELFELD IN STRUCT SPEICHERN
              spielfeldSchreiben(erhalten,game_data_struct_V2);
              Spielfeldausgabe(game_data_struct_V2->spielfeld);
                 if(game_data_struct_V2->spielernummer=='0'){
                   sendServer(SocketFD, "THINKING\n", 9);
              }else{
                sendServer(SocketFD, "THINKING\n", 9);

              }
            }else
            /////ENDE-PROTOKOLLPHASE/////

            /////MOVE-BEFEHLSSEQUENZ/////
            if(strncmp(erhalten, "+ MOVE ", 7)==0){
              sleep(10);
              sendServer(SocketFD, "THINKING\n", 9);
              //SPIELFELD IN STRUCT SPEICHERN
            }else


            /////SPIELZUG/////
            if(strncmp(erhalten, "+ OKTHINK", 9)==0){
               if(game_data_struct_V2->spielernummer=='0' && protokollphasenendenchecker==1){
                 sendServer(SocketFD, "PLAY C3:D4\n", 11);
                 //Spielfeld ausgegeben werden
                 Spielfeldausgabe(game_data_struct_V2->spielfeld);
                 protokollphasenendenchecker=0;
               }else{

                kill(game_data_struct_V2->pid_parent, SIGUSR1);       //Signal/Denkanstoß für thinker
                read(pipe, pipebuffer, 64);
                //laenge des Spielzuges berechnen
                //sendServer(); Spielzug
                //Spielfeld ausgegeben werden
                Spielfeldausgabe(game_data_struct_V2->spielfeld);
              }
            }


            /////IDLE-BEFEHLSSEQUENZ/////
            else if (strncmp(erhalten, "+ WAIT", 6)==0){
                sendServer(SocketFD, "OKWAIT\n", 6);
            }


            /////GAMEOVER-BEFEHLSSEQUENZ/////
            else if (strncmp(erhalten, "+ GAMEOVER", 10)==0){
                printf("Spiel vorbei!\n");
                game_data_struct_V2->gameover=0;
            }

            /////REAKTION AUF SERVER-FEHLERMELDUNG/////
            else if (strncmp(erhalten, "- ", 2)==0) {
                printf("Fehler bei der Serverkommunikation\n");
                //printf("Spiel automatisch verloren!\n");
                game_data_struct_V2->gameover=0;
            }
            //CASE FUER LEERE NACHRICHT VOM Server
            //EVTL MIT LOOP COUNTER
            //UM DAUERSCHLEIFE ZU VERHINDERN
        }

   }
