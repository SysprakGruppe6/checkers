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

char * umwandel(int n){
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

  return ergebnis;
}

//Gibt den "Inhalt" einer Position aus
char getSpielstein(int pos, char * spielfeld){
    if(pos<1 || pos>32)
        return '0';
    return spielfeld[pos];
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

//Gibt ein Array mit den vier Nachbarpositionen aus
int * getNeighbors(int pos){
    int * result = malloc(sizeof(int)*4);
    int modpos = pos%8;
    if(modpos==1||modpos==2||modpos==3||modpos==4){
        for(int i =0; i<4;i++){
            switch(i){
                case 1:
                    result[i]=pos-4;
                case 2:
                    result[i]=pos-3;
                case 3:
                    result[i]=pos+5;
                case 4:
                    result[i]=pos+4;
            }
        }

    }
    if(modpos==5||modpos==6||modpos==7||modpos==0){
        for(int i =0; i<4;i++){
            switch(i){
                case 1:
                    result[i]=pos-5;
                case 2:
                    result[i]=pos-4;
                case 3:
                    result[i]=pos+4;
                case 4:
                    result[i]=pos+3;
            }
        }
    }
    for(int i =0; i<4;i++){
        if (result[i]<1 || result[i]>32)
            result[i]=0;
    }
    if(checkBorder(pos)==-1){
        result[0]=0;
        result[3]=0;
    }
    if(checkBorder(pos)==1){
        result[1]=0;
        result[2]=0;
    }
    return result;
}

//Checkt ob die Stelle pos2 von einen gegnerischen Stein von pos1 besetzt ist
int compare(int pos1, int pos2, char * spielfeld){
  char * rivals  = getRivals(getSpielstein(pos1, spielfeld));
  char * ourTeam = getRivals(rivals[0]);
  char spielstein2 = getSpielstein(pos2, spielfeld);
  if(getSpielstein(pos1, spielfeld)=='*'||getSpielstein(pos2, spielfeld)=='*')//Check ob einer von beiden * ist. Evtll ist dann zeile 76/77 überflüssig, kA
    return 0;
  if (spielstein2==rivals[0]||spielstein2==rivals[1])//falls gegner auf pos2
    return 1;
  if (spielstein2==ourTeam[0]||spielstein2==ourTeam[1])//falls teammate auf pos2
    return -1;
  else
    return 0;
}

//Checkt ob Spielstein an pos1 schlagen kann. Falls ja, returnt es die neue Position von pos1 Stein, sonnst 0.
int strike(int pos, char * spielfeld){
    char * rivals     = getRivals(getSpielstein(pos, spielfeld));
    int  * neighbors  = getNeighbors(pos);
    int nowPos  = pos;
    switch(getSpielstein(pos, spielfeld)){
        case 'w':
            for(int i=0;i<2;i++){
                if(compare(pos, neighbors[i], spielfeld)==1 && getSpielstein(getNeighbors(neighbors[i])[i], spielfeld)=='*'){
//                     if(spielzug="")
//                         spielzug += umwandel(pos);
//                     pielzug += ":";
//                     spielzug += umwandel(getNeighbors(neighbors[i])[i]);
                    spielfeld[pos]='*';
                    spielfeld[neighbors[i]]='*';
                    spielfeld[getNeighbors(neighbors[i])[i]]='w';
                    return spielfeld[getNeighbors(neighbors[i])[i]];
                }
            }
            return 0;
        case 'b':
            for(int i=2;i<4;i++){
                if(compare(pos, neighbors[i], spielfeld)==1 && getSpielstein(getNeighbors(neighbors[i])[i], spielfeld)=='*'){
//                     if(spielzug="")
//                         spielzug += umwandel(pos);
//                     pielzug += ":";
//                     spielzug += umwandel(getNeighbors(neighbors[i])[i]);
                    spielfeld[pos]='*';
                    spielfeld[neighbors[i]]='*';
                    spielfeld[getNeighbors(neighbors[i])[i]]='b';
                    return spielfeld[getNeighbors(neighbors[i])[i]];
                }
            }
            return 0;
        case 'W':
            for(int i=0;i<4;i++){
                for(int d=0;d<7;d++){
                    nowPos=getNeighbors(nowPos)[i];
                    if(compare(pos, nowPos, spielfeld)==-1 || getSpielstein(nowPos, spielfeld)=='0')
                        return 0;
                    if(compare(pos, nowPos, spielfeld)==1){
                        if(getSpielstein(getNeighbors(nowPos)[i], spielfeld)=='*'){
//                             if(spielzug="")
//                                 spielzug += umwandel(pos);
//                             pielzug += ":";
//                             spielzug += umwandel(getNeighbors(nowPos)[i]);
                            spielfeld[pos]='*';
                            spielfeld[nowPos]='*';
                            spielfeld[getNeighbors(nowPos)[i]]='W';
                            return spielfeld[getNeighbors(nowPos)[i]];
                        }
                        return 0;
                    }
                }
            }
            return 0;
        case 'B':
            for(int i=0;i<4;i++){
                for(int d=0;d<7;d++){
                    nowPos=getNeighbors(nowPos)[i];
                    if(compare(pos, nowPos, spielfeld)==-1 || getSpielstein(nowPos, spielfeld)=='0')
                        return 0;
                    if(compare(pos, nowPos, spielfeld)==1){
                        if(getSpielstein(getNeighbors(nowPos)[i], spielfeld)=='*'){
//                             if(spielzug="")
//                                 spielzug += umwandel(pos);
//                             spielzug += ":";
//                             spielzug += umwandel(getNeighbors(nowPos)[i]);
                            spielfeld[pos]='*';
                            spielfeld[nowPos]='*';
                            spielfeld[getNeighbors(nowPos)[i]]='B';
                            return spielfeld[getNeighbors(nowPos)[i]];
                        }
                        return 0;
                    }
                }
            }
            return 0;
        case '*':
            return 0;
        default:
            return 0;
    }
}

int move(int pos, char * spielfeld){
    int  * neighbors  = getNeighbors(pos);
    switch(getSpielstein(pos, spielfeld)){
        case 'w':
            for(int i=0;i<2;i++){
                if(getSpielstein(neighbors[i], spielfeld)=='*'){
//                     spielzug += umwandel(pos);
//                     spielzug += ":";
//                     spielzug += umwandel(neighbors[i]);
                    spielfeld[pos]='*';
                    spielfeld[neighbors[i]]='w';
                    return spielfeld[neighbors[i]];
                }
            }
            return 0;
        case 'b':
            for(int i=2;i<4;i++){
                if(getSpielstein(neighbors[i], spielfeld)=='*'){
//                     spielzug += umwandel(pos);
//                     spielzug += ":";
//                     spielzug += umwandel(neighbors[i]);
                    spielfeld[pos]='*';
                    spielfeld[neighbors[i]]='b';
                    return spielfeld[neighbors[i]];
                }
            }
            return 0;
        case 'W':
            for(int i=0;i<4;i++){
                if(getSpielstein(neighbors[i], spielfeld)=='*'){
//                     spielzug += umwandel(pos);
//                     spielzug += ":";
//                     spielzug += umwandel(neighbors[i]);
                    spielfeld[pos]='*';
                    spielfeld[neighbors[i]]='W';
                    return spielfeld[neighbors[i]];
                }
            }
            return 0;
        case 'B':
            for(int i=0;i<4;i++){
                if(getSpielstein(neighbors[i], spielfeld)=='*'){
//                     spielzug += umwandel(pos);
//                     spielzug += ":";
//                     spielzug += umwandel(neighbors[i]);
                    spielfeld[pos]='*';
                    spielfeld[neighbors[i]]='B';
                    return spielfeld[neighbors[i]];
                }
            }
            return 0;
        case '*':
            return 0;
        default:
            return 0;
    }
}

void think(struct gds *game_data_struct_V2){

    printf("THINKER: %c\n",game_data_struct_V2->spielfeld[5] );
    printf("Am denken ...\n");
    
    char *spielfeld = malloc(sizeof(char)*32);
    memcpy(spielfeld, game_data_struct_V2->spielfeld, sizeof(game_data_struct_V2->spielfeld));
    
    char *spielzug = malloc(sizeof(char)*32);

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

}
