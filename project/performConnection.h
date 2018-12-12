#ifndef performConnection
//#define performConnection

void bufferClear(char buffer[128]);
void performConnection(int SocketFD, char* gId, char* pId);
int SHmem();
void pCTest();
void sendServer(int SocketFD, char *nachricht, int laenge);
void recvServer(int SocketFD);

#endif
