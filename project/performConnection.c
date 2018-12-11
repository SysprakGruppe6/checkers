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
    char buffer[128];
    memset(&buffer, ' ',sizeof(buffer[128]));
    n = read(SocketFD, buffer, sizeof(buffer)-1);
    printf("Server: ");
        if(fputs(buffer, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
        if(n < 0)
        {
            printf("\n Recv error \n");
        }
    printf("\n");
    memset(&buffer, ' ',sizeof(buffer[128]));
}

//Funktion zum Senden von Nachrichten an den Server
void sendServer(int SocketFD, char *nachricht){
    //send
    char buffer[128];
    memset(buffer, ' ',sizeof(buffer[128]));
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

    sendServer(SocketFD, "VERSION 2.1\n");    //sendet die Versionsnummer
    //bufferClear(buffer);      //Cleared den Buffer

    recvServer(SocketFD);   //gibt zweite Nachricht des Servers aus
    //bufferClear(buffer);      //Cleared den Buffer

    sendServer(SocketFD, "ID 1111111111111\n");    //sendet die ID
    //bufferClear(buffer);      //Cleared den Buffer

    recvServer(SocketFD);   //gibt dritte Nachricht des Servers aus
    //bufferClear(buffer);      //Cleared den Buffer

    //Protokollphase
}

//Testfunktion
void pCTest(){
printf("Ich bin ein test für die Perform Connection\n");

}
