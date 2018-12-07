#include <stdlib.h>
#include <stdio.h>
#include "performConnection.h"

//Funktion zum Clearen des Buffers
void bufferClear(){
    memset(buffer, ' ',sizeof(buffer));
}

//Funktion zum Ausgeben der erhaltenen Servernachrichten
void recvServer(int SocketFD /*, Buffer*/){
    //recv
    int n = 0;
    n = recv(SocketFD, buffer, sizeof(buffer)-1, 0);
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
}

//Funktion zum Senden von Nachrichten an den Server
void sendServer(int SocketFD /*, Buffer, Nachricht*/){
    //send
}

//Funktion welche die Protokollphase ausführt
void performConnection(int SocketFD){
    char buffer[128];   //Buffer
    bufferClear();      //Cleared den Buffer
    
    recvServer(SocketFD, buffer);   //gibt erste Nachricht des Servers aus
    bufferClear();      //Cleared deb Buffer
    
    //Protokollphase
    
    printf("Test für Header");
    //returnwert für Fehlerbehandlung
}

//Testfunktion
void pCTest(){
printf("Ich bin ein test für die Perform Connection\n");

}
