all : main factorial

main: main.o
	gcc -Wall -g main.o -o main
	
main.o: main.c
	gcc -Wall -g -c main.c -o main.o

factorial: factorial.c
	gcc -Wall -g factorial.c -o factorial

run: main
	./main

clean:
	rm -f factorial main.o main

	