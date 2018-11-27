sysprak_client: tmp/main.o
	gcc -o sysprak_client tmp/main.o

tmp/main.o: project/main.c
	gcc -c project/main.c -o tmp/main.o

.PHONY : clean
clean:
	-rm sysprak_client tmp/* $(objects)
