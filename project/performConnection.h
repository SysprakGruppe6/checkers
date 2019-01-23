#ifndef performConnection
#include <sys/shm.h> // include für Shared Memory
#include <sys/ipc.h> // include für Shared Memor
#include <sys/stat.h> // include für Shared Memory
#include <sys/types.h>
#include "SHM.h"
//#define performConnection
//struct
//Functions
void performConnection(int SocketFD, char* gId, char* pId, int shmid,struct gds *);
int SHmem(int size);
void spielfeldSchreiben(char buffer[2048],struct gds *game_data_struct_V2);
void Spielfeldausgabe (char feld[15][8]);
void sendServer(int SocketFD, char *nachricht, int laenge);
void recvServer(int SocketFD, char* buf);

#endif
