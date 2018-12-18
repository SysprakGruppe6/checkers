#ifndef configParser

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/shm.h> // include für Shared Memory
#include <sys/ipc.h> // include für Shared Memory
#define BUFFER  128

struct parameters{
	char hostName[BUFFER];
	char portNr[BUFFER];
	char gameType[BUFFER];
};

struct parameters read_cfg (char *cfgpath){

    struct parameters file; //result

    FILE *cfgfile = fopen(cfgpath, "r");

    char pName[BUFFER];
    char pValue[BUFFER];
    char line[BUFFER];

    while(fgets(line, BUFFER, cfgfile) != NULL){
        sscanf(line, "%s = %s", pName, pValue);

        if(line[0]!='#'){
            if(strcmp(pName, "HOSTNAME") == 0) {
                strcpy(file.hostName, pValue);
            }
            if(strcmp(pName, "PORT_NR") == 0) {
                strcpy(file.portNr, pValue);
            }
            if(strcmp(pName, "GAME_TYPE") == 0) {
                strcpy(file.gameType, pValue);
            }
        }
    }

    fclose(cfgfile);
    return file;
}

#endif
