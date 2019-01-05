#ifndef performConnection
#include <sys/shm.h> // include für Shared Memory
#include <sys/ipc.h> // include für Shared Memor
#include <sys/stat.h> // include für Shared Memory
//#define performConnection
//struct
//Functions
void bufferClear(char buffer[128]);
void performConnection(int SocketFD, char* gId, char* pId);
int SHmem();
void pCTest();
void sendServer(int SocketFD, char *nachricht, int laenge);
void recvServer(int SocketFD);

#endif
