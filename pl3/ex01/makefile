writer: writer.o
	gcc -Wall -g writer.o -o writer -lrt

writer.o: writer.c 
	gcc -c -g -Wall writer.c -o writer.o

reader: reader.o
	gcc -Wall -g reader.o -o reader -lrt

reader.o: reader.c 
	gcc -c -g -Wall reader.c -o reader.o

run: writer reader clean
	bash -c "./writer"

clean:
	rm -f writer.o reader.o

remove: clean
	rm -f writer reader