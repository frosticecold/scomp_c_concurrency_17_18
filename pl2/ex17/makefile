all: main validate

main: main.o
	gcc -Wall -g main.o -o main
	
main.o: main.c
	gcc -Wall -g -c main.c -o main.o

validate: validate.c
	gcc -Wall -g validate.c -o validate

run: main
	./main

clean:
	rm -f validate main.o main

	