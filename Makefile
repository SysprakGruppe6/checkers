CC = gcc
FLAGS= -std=c99 -D_XOPEN_SOURCE=600 -Wall -Wextra -Werror
DEPS = project/performConnection.h project/ai.h project/SHM.h project/configParser.h
OBJ = tmp/performConnection.o tmp/ai.o tmp/main.o

all: sysprak-client

sysprak-client: $(OBJ)
	$(CC) $(FLAGS) -o $@ $^

tmp/%.o: project/%.c
	$(CC) $(FLAGS) -c -o $@ $<

play:	sysprak-client
	./sysprak-client -g $(GAME_ID) -p $(PLAYER)

.PHONY : clean
clean:
	-rm sysprak-client tmp/*
