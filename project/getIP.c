/////////////////////////////////////////////////////
// Bestimmt die IP des Servers mithilfe des        //
// Hostnames und der Funktion gethostbyname()      //
/////////////////////////////////////////////////////
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
//#include <wait.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"

int getIP(struct in_addr **addr_list){

    int l;  //Schleifenvariable für die IP-Liste
    struct hostent *he;
    //struct in_addr **addr_list;
    // get the host info
    if ((he = gethostbyname(HOSTNAME)) == NULL) {
        printf("Error bei gethostbyname()\n");
        return -1;
    }
    // print information about this host:
    printf("Official name is: %s\n", he->h_name);
    printf("IP addresses: ");
    addr_list = (struct in_addr **)he->h_addr_list;
    for(l = 0; addr_list[l] != NULL; l++) {
        printf("%s", inet_ntoa(*addr_list[l]));
    }
    printf("\n");
    return 0;
}  
