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


int * getTeam(int playerNr, char * spielfeld){
    int * result = malloc(sizeof(int)*12);
    int counter = 0;
    switch(playerNr){
        case 0://Weiss
            for(int i=1;i<33;i++){
                if(getSpielstein(i, spielfeld)=='w'||getSpielstein(i, spielfeld)=='W'){
                    result[counter]=i;
                    counter++;
                }
            }
            break;
        case 1://Schwarz
            for(int i=1;i<33;i++){
                if(getSpielstein(i, spielfeld)=='b'||getSpielstein(i, spielfeld)=='B'){
                    result[counter]=i;
                    counter++;
                }
            }
            break;
    }
    return result;
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


//Gibt ein Array mit den möglichen gegnerische Steine aus
char * getRivals(char spielstein){
  char * result = malloc(sizeof(char)*2);
  if(spielstein=='w'||spielstein=='W'){
    result[0]='b';
    result[1]='B';
  }
  else
  if(spielstein=='b'||spielstein=='B'){
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
                case 0:
                    result[i]=pos-4;
                    break;
                case 1:
                    result[i]=pos-3;
                    break;
                case 2:
                    result[i]=pos+5;
                    break;
                case 3:
                    result[i]=pos+4;
                    break;
            }
        }

    }
    if(modpos==5||modpos==6||modpos==7||modpos==0){
        for(int i =0; i<4;i++){
            switch(i){
                case 0:
                    result[i]=pos-5;
                    break;
                case 1:
                    result[i]=pos-4;
                    break;
                case 2:
                    result[i]=pos+4;
                    break;
                case 3:
                    result[i]=pos+3;
                    break;
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
int strike(int pos, char * spielfeld, char * currentMove){
    int  * neighbors  = getNeighbors(pos);
    int nowPos  = pos;
    switch(getSpielstein(pos, spielfeld)){
        case 'w':
            for(int i=0;i<2;i++){
                if(compare(pos, neighbors[i], spielfeld)==1 && getSpielstein(getNeighbors(neighbors[i])[i], spielfeld)=='*'){
                    if(strcmp(currentMove, "")==0)
                        strcat(currentMove, umwandel(pos));
                    strcat(currentMove, ":");
                    strcat(currentMove, umwandel(getNeighbors(neighbors[i])[i]));
                    printf("MOVETESTSTRIKEw:%s\n", currentMove);
                    spielfeld[pos]='*';
                    spielfeld[neighbors[i]]='*';
                    spielfeld[getNeighbors(neighbors[i])[i]]='w';
                    printf("return strike w:%s\n", currentMove);
                    return getNeighbors(neighbors[i])[i];
                }
            }
            return 0;
        case 'b':
            for(int i=2;i<4;i++){
                if(compare(pos, neighbors[i], spielfeld)==1 && getSpielstein(getNeighbors(neighbors[i])[i], spielfeld)=='*'){
                    if(strcmp(currentMove, "")==0)
                        strcat(currentMove, umwandel(pos));
                    strcat(currentMove, ":");
                    strcat(currentMove, umwandel(getNeighbors(neighbors[i])[i]));
                    printf("MOVETESTSTRIKEb:%s\n", currentMove);
                    spielfeld[pos]='*';
                    spielfeld[neighbors[i]]='*';
                    spielfeld[getNeighbors(neighbors[i])[i]]='b';
                    return getNeighbors(neighbors[i])[i];
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
                            if(strcmp(currentMove, "")==0)
                                strcat(currentMove, umwandel(pos));
                            strcat(currentMove, ":");
                            strcat(currentMove, umwandel(getNeighbors(neighbors[i])[i]));
                            printf("MOVETESTSTRIKEW:%s\n", currentMove);
                            spielfeld[pos]='*';
                            spielfeld[nowPos]='*';
                            spielfeld[getNeighbors(nowPos)[i]]='W';
                            return getNeighbors(nowPos)[i];
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
                            if(strcmp(currentMove, "")==0)
                                strcat(currentMove, umwandel(pos));
                            strcat(currentMove, ":");
                            strcat(currentMove, umwandel(getNeighbors(neighbors[i])[i]));
                            printf("MOVETESTSTRIKEB:%s\n", currentMove);
                            spielfeld[pos]='*';
                            spielfeld[nowPos]='*';
                            spielfeld[getNeighbors(nowPos)[i]]='B';
                            return getNeighbors(nowPos)[i];
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

int move(int pos, char * spielfeld, char * currentMove){
    int  * neighbors  = getNeighbors(pos);
    switch(getSpielstein(pos, spielfeld)){
        case 'w':
            for(int i=0;i<2;i++){
                if(getSpielstein(neighbors[i], spielfeld)=='*'){
                    strcpy(currentMove, umwandel(pos));
                    strcat(currentMove, ":");
                    strcat(currentMove, umwandel(neighbors[i]));
                    printf("MOVETESTMOVw:%s\n", currentMove);
                    spielfeld[pos]='*';
                    spielfeld[neighbors[i]]='w';
                    return neighbors[i];
                }
            }
            return 0;
        case 'b':
            for(int i=2;i<4;i++){
                if(getSpielstein(neighbors[i], spielfeld)=='*'){
                    strcpy(currentMove, umwandel(pos));
                    strcat(currentMove, ":");
                    strcat(currentMove, umwandel(neighbors[i]));
                    printf("MOVETESTMOVb:%s\n", currentMove);
                    spielfeld[pos]='*';
                    spielfeld[neighbors[i]]='b';
                    return neighbors[i];
                }
            }
            return 0;
        case 'W':
            for(int i=0;i<4;i++){
                if(getSpielstein(neighbors[i], spielfeld)=='*'){
                    strcpy(currentMove, umwandel(pos));
                    strcat(currentMove, ":");
                    strcat(currentMove, umwandel(neighbors[i]));
                    printf("MOVETESTMOVW:%s\n", currentMove);
                    spielfeld[pos]='*';
                    spielfeld[neighbors[i]]='W';
                    return neighbors[i];
                }
            }
            return 0;
        case 'B':
            for(int i=0;i<4;i++){
                if(getSpielstein(neighbors[i], spielfeld)=='*'){
                    strcat(currentMove, umwandel(pos));
                    strcat(currentMove, ":");
                    strcat(currentMove, umwandel(neighbors[i]));
                    printf("MOVETESTMOVB:%s\n", currentMove);
                    spielfeld[pos]='*';
                    spielfeld[neighbors[i]]='B';
                    return neighbors[i];
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

    Spielfeldausgabe(game_data_struct_V2->spielfeld);

    printf("Thinker Am denken ...\n");

    char *spielfeld = malloc(sizeof(char)*32);
    memcpy(spielfeld, game_data_struct_V2->spielfeld, sizeof(game_data_struct_V2->spielfeld));

    Spielfeldausgabe(spielfeld);
    int *meinTeam = getTeam(game_data_struct_V2->spielernummer, spielfeld);

    int tmpPos = 0;
    for(int i=0; i<12; i++){//Check ob irgendein Spielstein Schlagen kann
      tmpPos=meinTeam[i];
      while(tmpPos!=0){
        tmpPos=strike(tmpPos, spielfeld, game_data_struct_V2->currentMove);
      }
      if(strcmp(game_data_struct_V2->currentMove, "")!=0){
        break;
      }
    }

    if(strcmp(game_data_struct_V2->currentMove, "")==0){
      for(int i=0; i<12; i++){//Check ob irgendein Spielstein Bewegen kann
          move(meinTeam[i], spielfeld, game_data_struct_V2->currentMove);
          if(strcmp(game_data_struct_V2->currentMove, "")!=0)
            break;
        }
    }
	return;
}
