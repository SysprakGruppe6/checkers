CC = gcc
FLAGS= -std=c99
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

.PHONY : clean
clean:
	-rm sysprak-client tmp/*
