sysprak_client: output/main.o
	gcc -o sysprak_client output/main.o

output/main.o: project/main.c
	gcc -c project/main.c -o output/main.o
