#vars
INCLUDES =
SOURCES =
OBJFILES =
EXEC = reader writer clear
FLAGS = -Wall -g
LIBS =  -lrt -lpthread

all: ${EXEC}

reader: sm.c shared_data.c reader.c
	gcc $(FLAGS) -o reader $? $(LIBS)

writer: sm.c shared_data.c writer.c
	gcc $(FLAGS) -o writer $? $(LIBS)

clear: sm.c shared_data.c clear.c
	gcc $(FLAGS) -o clear $? $(LIBS)
clean:
	rm -f ${OBJFILES} ${EXEC}

