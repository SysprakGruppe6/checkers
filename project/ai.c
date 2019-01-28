#include <stdlib.h>
#include <stdio.h>
#include "ai.h"
//benoetigt mehr includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/shm.h> // include für Shared Memory
#include <sys/ipc.h> // include für Shared Memory
#include <signal.h>

void my_handler (int signum){
    if(signum==SIGUSR1){
        printf("Signal angekommen\n");
    }
}

 //Gibt den "Inhalt" einer Position aus
 char getSpielstein(int pos, struct gds *game_data_struct_V2){
   return game_data_struct_V2->spielfeld[pos][1];
 }

//Checkt ob wir auf eine von beiden Ränder sind
int checkBorder(int pos){
  if(pos%8-5==0)
    return -1; //Linker Rand
  if(pos%8-4==0)
    return 1; //Rechter Rand
  else
    return 0;
}

//Checkt welcher spieler ein Spielstein gehört
int getColour (char spielstein){
  if (spielstein=='w'||spielstein=='W')//  1 = Weiss
    return 1;
  if (spielstein=='b'||spielstein=='B')// -1 = Schwarz
    return -1;
  else
    return 0; //0 = Leer oder Fehler
}

//Gibt ein Array mit den möglichen gegnerische Steine aus
char * getRivals(char spielstein){
  char * result = malloc(sizeof(char)*2);
  if(getColour(spielstein)==1){
    result[0]='b';
    result[1]='B';
  }
  if(getColour(spielstein)==-1){
    result[0]='w';
    result[1]='W';
  }
  else{
    result[0]='*';
    result[1]='*';
  }
  return result;
}

//Checkt ob die Stelle pos2 von einen gegnerischen Stein von pos1 besetzt ist
int compare(int pos1, int pos2, struct gds *game_data_struct_V2){
  char * rivals  = getRivals(getSpielstein(pos1, game_data_struct_V2));
  char * ourTeam = getRivals(rivals[0]);
  char spielstein2 = getSpielstein(pos2, game_data_struct_V2);
  if(getSpielstein(pos1, game_data_struct_V2)=='*'||getSpielstein(pos2, game_data_struct_V2)=='*')//Check ob einer von beiden * ist. Evtll ist dann zeile 76/77 überflüssig, kA
    return 0;
  if (spielstein2==rivals[0]||spielstein2==rivals[1])//falls gegner auf pos2
    return 1;
  if (spielstein2==ourTeam[0]||spielstein2==ourTeam[1])//falls teammate auf pos2
    return -1;
  else
    return 0;
}

//Checkt ob Spielstein an pos1 den Spielstein an pos2 schlagen kann. Falls ja, returnt es die neue Position von pos1 Stein, sonnst 0.
// int strikeCheck(int pos1, int pos2){
//   switch(getSpielstein(pos1))){
//     case 'w':
//       switch (checkBorder(pos1)) {
//         case -1:
//           if(pos2==pos1-4 && getSpielstein(pos1-7)=='*')
//             return pos1-7;
//           else
//             return 0;
//         case 1:
//           if(pos2==pos1-4 && getSpielstein(pos1-9)=='*')
//             return pos1-9;
//           else
//             return 0;
//         case 0:
//           if (checkBorder(pos2)==1||checkBorder(pos2)==-1)
//             return 0;
//           if(pos2==pos1-4 && getSpielstein(pos1-9)=='*')
//       }
//     case 'b':
//
//     case 'W':
//
//     case 'B':
//
//     case '*':
//       return 0;
//     default:
//       return 0;
//   }
// }

void think(struct gds *game_data_struct_V2){
	printf("Am denken ...\n");
    //Test-Spielzug
    char testMoveW[64];
    strcpy(testMoveW, "A3:B4\n");

    char testMoveB[64];
    strcpy(testMoveW, "B6:A5\n");

    if(game_data_struct_V2->spielernummer=='1'){
        strcpy(game_data_struct_V2->currentMove, testMoveW);
        memset(testMoveW, 0, 64);
    } else
    if(game_data_struct_V2->spielernummer=='2'){
        strcpy(game_data_struct_V2->currentMove, testMoveB);
        memset(testMoveB, 0, 64);
    }

    printf("test\n");

}
