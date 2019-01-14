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

void think(){
	printf("Am denken ...\n");

	//while(Spiel läuft==wahr){
		pause();		//wartet auf Signal vom Connector
		//berechnen des Spielzuges (MS04)
		//send(Spielzug, pipe);
		printf("test");
	//}

}
