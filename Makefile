CC = gcc
FLAGS= -std=c99 -D_XOPEN_SOURCE=600
CFLAGS= -Wall -Wextra -Werror
DEPS = project/performConnection.h project/ai.h
OBJ = tmp/performConnection.o tmp/ai.o tmp/main.o

all: sysprak-client

test: FLAGS += $(CFLAGS)
test: sysprak-client

sysprak-client: $(OBJ)
	$(CC) $(FLAGS) -o $@ $^

tmp/%.o: project/%.c
	$(CC) $(FLAGS) -c -o $@ $<

play:	sysprak-client
	./sysprak-client -g $(GAME_ID) -p $(PLAYER)
	
.PHONY : clean
clean:
	-rm sysprak-client tmp/*
