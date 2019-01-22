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

    printf("test");

}
