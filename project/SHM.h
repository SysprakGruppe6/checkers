#ifndef SHM
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h> // include für Shared Memory
#include <sys/ipc.h> // include für Shared Memory
struct gds {
 char gameID[13];
 char spielernummer;
 char spielfeld [33];
 int anzahl_spieler;
 pid_t pid_child;
 pid_t pid_parent;
 int gameover;
 char currentMove[32];
};


#endif
