#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#define GAMEKINDNAME "Checkers"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"

int main(int argc, char * argv[]){
  
  //Game-id und Spielernummer
    char *g;
    char *p;
    
  //Schleife für kommandozeilenparameter
    int i = 1;
    while (i < argc) {
        if (strcmp("-g", argv[i])==0) {
            g=argv[i+1];
        }
        if (strcmp("-p", argv[i])==0) {
            p=argv[i+1];
        }
        i++;
    }
    
  //Socket
    int clientsocket;
    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
/*
  //Connect
    char buffer[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    
*/
  printf("%s %s\n", g, p);
  //printf("%s %s %i \n", GAMEKINDNAME, HOSTNAME, PORTNUMBER);
    
  return 0;
}  
