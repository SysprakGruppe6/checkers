CC = gcc
CFLAGS = -Wall
DEPS = performConnection.h
OBJ = performConnection.o main.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

sysprak_client: $(OBJ)
	gcc $(CFLAGS) -o $@ $^
