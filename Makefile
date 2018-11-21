sysprak_client: output/main.o
	gcc -o sysprak_client output/main.o

output/main.o: projekt/main.c
	gcc -c projekt/main.c -o output/main.o
