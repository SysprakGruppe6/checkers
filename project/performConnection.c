#include <stdlib.h>
#include <stdio.h>
#include "performConnection.h"
//benoetigt mehr includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
//noch zu ueberpruefen welche noetig sind

//Funktion zum Clearen des Buffers
void bufferClear(char buffer[128]){
    memset(&buffer, ' ',sizeof(buffer[128]));
}

//Funktion zum Ausgeben der erhaltenen Servernachrichten
void recvServer(int SocketFD){
    //recv
    int n = 0;
    char buffer[2048];
    memset(buffer, '0',sizeof(buffer));
    n = read(SocketFD, buffer, sizeof(buffer));
    printf("Server: ");
        //if(fputs(buffer, stdout) == EOF)
        //{
        //    printf("\n Error : Fputs error\n");
        //}
        //if(n < 0)
        //{
        //    printf("\n Recv error \n");
        //}
        for(int i =  0; i<n; i++){
          printf("%c", buffer[i]);
        }
    printf("\n");
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

//Funktion welche die Protokollphase ausführt
void performConnection(int SocketFD){
    //bufferClear(buffer);      //Cleared den Buffer

    recvServer(SocketFD);   //gibt erste Nachricht des Servers aus
    //bufferClear(buffer);      //Cleared den Buffer

    sendServer(SocketFD, "VERSION 2.1\n", 12);    //sendet die Versionsnummer
    //bufferClear(buffer);      //Cleared den Buffer

    recvServer(SocketFD);   //gibt zweite Nachricht des Servers aus
    //bufferClear(buffer);      //Cleared den Buffer

    sendServer(SocketFD, "ID 08hcwormcqb75\n", 17);    //sendet die ID
    //bufferClear(buffer);      //Cleared den Buffer

    recvServer(SocketFD);   //gibt dritte Nachricht des Servers aus
    //bufferClear(buffer);      //Cleared den Buffer
    recvServer(SocketFD);
    //Protokollphase
    sendServer(SocketFD, "PLAYER 1\n", 9); //send player number
    recvServer(SocketFD);
    recvServer(SocketFD);

}

//Testfunktion
void pCTest(){
printf("Ich bin ein test für die Perform Connection\n");

}
