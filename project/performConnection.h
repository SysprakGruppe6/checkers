#ifndef performConnection
//#define performConnection

void bufferClear(char buffer[128]);
void performConnection(int SocketFD);
void pCTest();
void sendServer(int SocketFD, char *nachricht, int laenge);
void recvServer(int SocketFD);

#endif
