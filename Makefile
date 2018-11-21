sysprak_client: main.o
	gcc -o sysprak_client main.o

main.o: projekt/main.c
	gcc -c projekt/main.c
