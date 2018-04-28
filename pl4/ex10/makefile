#vars
INCLUDES =
SOURCES =
OBJFILES =
EXEC = insert consult initial consultall
FLAGS = -Wall -g
LIBS =  -lrt -lpthread

all: ${EXEC}

insert: sm.c shared_data.c insert.c 
	gcc $(FLAGS) -o insert $? $(LIBS)

consult: sm.c shared_data.c consult.c 
	gcc $(FLAGS) -o consult $? $(LIBS)

consultall: sm.c shared_data.c consultall.c
	gcc $(FLAGS) -o consultall $? $(LIBS)

initial: sm.c shared_data.c initial.c 
	gcc $(FLAGS) -o initial $? $(LIBS)
clean:
	rm -f ${OBJFILES} ${EXEC}

