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
char buffer[128];

//Funktion zum Clearen des Buffers
void bufferClear(char buffer[128]){
    memset(&buffer, ' ',sizeof(buffer[128]));
}

//Funktion zum Ausgeben der erhaltenen Servernachrichten
void recvServer(int SocketFD, char buffer){
    //recv
    int n = 0;
    n = recv(SocketFD, &buffer, sizeof(buffer)-1, 0);
    printf("Server: ");
        if(fputs(&buffer, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
        if(n < 0)
        {
            printf("\n Recv error \n");
        }
    printf("\n");
}

//Funktion zum Senden von Nachrichten an den Server
void sendServer(int SocketFD, char buffer, char *nachricht){
    //send
    int n=0;
    strcpy(&buffer, nachricht);
    printf("Client: %s\n", &buffer); //testprint
    n=send(SocketFD, &buffer, sizeof(buffer), 0);
    if(n < 0){
        printf("\n Send error \n");
    }
}

//Funktion welche die Protokollphase ausführt
void performConnection(int SocketFD){
    //char buffer[128];   //Buffer
    bufferClear(buffer);      //Cleared den Buffer
    
    recvServer(SocketFD, buffer[0]);   //gibt erste Nachricht des Servers aus
    bufferClear(buffer);      //Cleared den Buffer
    
    sendServer(SocketFD, buffer[0], "VERSION 2.1");    //sendet die Versionsnummer
    bufferClear(buffer);      //Cleared den Buffer
    
    recvServer(SocketFD, buffer[0]);   //gibt zweite Nachricht des Servers aus
    bufferClear(buffer);      //Cleared den Buffer
    
    sendServer(SocketFD, buffer[0], "1111111111111");    //sendet die Versionsnummer
    bufferClear(buffer);      //Cleared den Buffer
    
    //Protokollphase
}

//Testfunktion
void pCTest(){
printf("Ich bin ein test für die Perform Connection\n");

}
