#ifndef performConnection
#include <sys/shm.h> // include für Shared Memory
#include <sys/ipc.h> // include für Shared Memor
#include <sys/stat.h> // include für Shared Memory
//#define performConnection
//struct
//Functions
void performConnection(int SocketFD, char* gId, char* pId);
int SHmem(int size);
void sendServer(int SocketFD, char *nachricht, int laenge);
void recvServer(int SocketFD);

#endif
