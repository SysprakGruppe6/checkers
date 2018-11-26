sysprak_client: tmp/main.o
	gcc -o sysprak_client tmp/main.o

tmp/main.o: project/test.c
	gcc -c project/test.c -o tmp/main.o

.PHONY : clean
clean:
	-rm sysprak_client tmp/* $(objects)
