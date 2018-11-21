sysprak_client: output/main.o
	gcc -o sysprak_client output/main.o

main.o: projekt/main.c
	gcc -c projekt/main.c
