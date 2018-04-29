#vars
INCLUDES =
SOURCES =
OBJFILES =
EXEC = clear cliente vendedor
FLAGS = -Wall -g
LIBS =  -lrt -lpthread

all: ${EXEC}

clear: sm.c shared_data.c clear.c
	gcc $(FLAGS) -o clear $? $(LIBS)

cliente: sm.c shared_data.c cliente.c 
	gcc $(FLAGS) -o cliente $? $(LIBS)

vendedor: sm.c shared_data.c vendedor.c
	gcc $(FLAGS) -o vendedor $? $(LIBS)

clean:
	rm -f ${OBJFILES} ${EXEC}

