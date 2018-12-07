CC = gcc
FLAGS=
CFLAGS= -Wall -Wextra -Werror
DEPS = project/performConnection.h
OBJ = tmp/performConnection.o tmp/main.o

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
