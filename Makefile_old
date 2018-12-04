FLAGS= 
EXTFLAGS= -Wall -Wextra -Werror

all: sysprak_client

test: FLAGS += $(EXTFLAGS)
test: sysprak_client

sysprak_client: tmp/main.o
	gcc $(FLAGS) -o sysprak_client tmp/main.o

tmp/main.o: project/main.c
	gcc $(FLAGS) -c project/main.c -o tmp/main.o

.PHONY : clean
clean:
	-rm sysprak_client tmp/* $(objects)
