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

void globalVars(struct gds *game_data_struct_V2){
    char *spielfeld = game_data_struct_V2->spielfeld;
    char *spielzug = malloc(sizeof(char)*32);
}

void my_handler (int signum){
    if(signum==SIGUSR1){
        printf("Signal angekommen\n");
    }
}

char umwandel(int n){
  char *ergebnis;
  ergebnis = malloc(sizeof(char[2]));

  switch (n) {
    case 1 : strcpy(ergebnis, "B8"); break;
    case 2 : strcpy(ergebnis, "D8"); break;
    case 3 : strcpy(ergebnis, "F8"); break;
    case 4 : strcpy(ergebnis, "H8"); break;
    case 5 : strcpy(ergebnis, "A7"); break;
    case 6 : strcpy(ergebnis, "C7"); break;
    case 7 : strcpy(ergebnis, "E7"); break;
    case 8 : strcpy(ergebnis, "G7"); break;
    case 9 : strcpy(ergebnis, "B6"); break;
    case 10: strcpy(ergebnis, "D6"); break;
    case 11: strcpy(ergebnis, "F6"); break;
    case 12: strcpy(ergebnis, "H6"); break;
    case 13: strcpy(ergebnis, "A5"); break;
    case 14: strcpy(ergebnis, "C5"); break;
    case 15: strcpy(ergebnis, "E5"); break;
    case 16: strcpy(ergebnis, "G5"); break;
    case 17: strcpy(ergebnis, "B4"); break;
    case 18: strcpy(ergebnis, "D4"); break;
    case 19: strcpy(ergebnis, "F4"); break;
    case 20: strcpy(ergebnis, "H4"); break;
    case 21: strcpy(ergebnis, "A3"); break;
    case 22: strcpy(ergebnis, "C3"); break;
    case 23: strcpy(ergebnis, "E3"); break;
    case 24: strcpy(ergebnis, "G3"); break;
    case 25: strcpy(ergebnis, "B2"); break;
    case 26: strcpy(ergebnis, "D2"); break;
    case 27: strcpy(ergebnis, "F2"); break;
    case 28: strcpy(ergebnis, "H2"); break;
    case 29: strcpy(ergebnis, "A1"); break;
    case 30: strcpy(ergebnis, "C1"); break;
    case 31: strcpy(ergebnis, "E1"); break;
    case 32: strcpy(ergebnis, "G1"); break;
  }

  return *ergebnis;
}

//Gibt den "Inhalt" einer Position aus
// char getSpielstein(int pos){
//     if(pos<1 || pos>32)
//         return null;
//     return spielfeld[pos];
// }


//Checkt ob wir auf eine von beiden Ränder sind
// int checkBorder(int pos){
//   if(pos%8-5==0)
//     return -1; //Linker Rand
//   if(pos%8-4==0)
//     return 1; //Rechter Rand
//   else
//     return 0;
// }

//Checkt welcher spieler ein Spielstein gehört
// int getColour (char spielstein){
//   if (spielstein=='w'||spielstein=='W')//  1 = Weiss
//     return 1;
//   if (spielstein=='b'||spielstein=='B')// -1 = Schwarz
//     return -1;
//   else
//     return 0; //0 = Leer oder Fehler
// }
//
// //Gibt ein Array mit den möglichen gegnerische Steine aus
// char * getRivals(char spielstein){
//   char * result = malloc(sizeof(char)*2);
//   if(getColour(spielstein)==1){
//     result[0]='b';
//     result[1]='B';
//   }
//   if(getColour(spielstein)==-1){
//     result[0]='w';
//     result[1]='W';
//   }
//   else{
//     result[0]='*';
//     result[1]='*';
//   }
//   return result;
// }

//Gibt ein Array mit den vier Nachbarpositionen aus
// int * getNeighbors(int pos){
//     int * result = malloc(sizeof(int)*4);
//     int modpos = pos%8;
//     if(modpos==1||modpos==2||modpos==3||modpos==4){
//         for(int i =0; i<4;i++){
//             switch(i){
//                 case 1:
//                     result[i]=pos-4;
//                 case 2:
//                     result[i]=pos-3;
//                 case 3:
//                     result[i]=pos+5;
//                 case 4:
//                     result[i]=pos+4;
//             }
//         }
//
//     }
//     if(modpos==5||modpos==6||modpos==7||modpos==0){
//         for(int i =0; i<4;i++){
//             switch(i){
//                 case 1:
//                     result[i]=pos-5;
//                 case 2:
//                     result[i]=pos-4;
//                 case 3:
//                     result[i]=pos+4;
//                 case 4:
//                     result[i]=pos+3;
//             }
//         }
//     }
//     for(int i =0; i<4;i++){
//         if (result[i]<1 || result[i]>32)
//             result[i]=0;
//     }
//     if(checkBorder==-1){
//         result[0]=0;
//         result[3]=0;
//     }
//     if(checkBorder==1){
//         result[1]=0;
//         result[2]=0;
//     }
//     return result;
// }

//Checkt ob die Stelle pos2 von einen gegnerischen Stein von pos1 besetzt ist
// int compare(int pos1, int pos2){
//   char * rivals  = getRivals(getSpielstein(pos1));
//   char * ourTeam = getRivals(rivals[0]);
//   char spielstein2 = getSpielstein(pos2);
//   if(getSpielstein(pos1)=='*'||getSpielstein(pos2)=='*')//Check ob einer von beiden * ist. Evtll ist dann zeile 76/77 überflüssig, kA
//     return 0;
//   if (spielstein2==rivals[0]||spielstein2==rivals[1])//falls gegner auf pos2
//     return 1;
//   if (spielstein2==ourTeam[0]||spielstein2==ourTeam[1])//falls teammate auf pos2
//     return -1;
//   else
//     return 0;
// }

//Checkt ob Spielstein an pos1 den Spielstein an pos2 schlagen kann. Falls ja, returnt es die neue Position von pos1 Stein, sonnst 0.
// int strikeCheck(int pos1){
//     char * rivals  = getRivals(getSpielstein(pos1));
//     switch(getSpielstein(pos1))){
//         case 'w':
//             switch (checkBorder(pos1)) {
//                 case -1:
//                     if(compare((pos1-4), pos1)== 1 && getSpielstein(pos1-7)=='*')
//                         if(spielzug="")
//                             spielzug += umwandeln(pos1)
//                         spielfeld[pos1]='
//                         return pos1-7;
//                     else
//                         return 0;
//                 case 1:
//                     if(compare((pos1-4), pos1)==1&& getSpielstein(pos1-9)=='*')
//                         return 1;
//                     else
//                         return 0;
//                 case 0:
//                     int * spielzuege = malloc(sizeof(int)*2);
//                     if(compare)
//
//             }
//         case 'b':
//
//         case 'W':
//
//         case 'B':
//
//         case '*':
//             return 0;
//         default:
//             return 0;
//     }
// }

void think(struct gds *game_data_struct_V2){
	printf("Am denken ...\n");
  printf("Feldtest:%s \n",umwandel(25));
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
