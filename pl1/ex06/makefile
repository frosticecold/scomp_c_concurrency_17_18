#
EXEC = ex_b ex_c ex_d
main:
	gcc -Wall -g ex_b.c -o ex_b
	gcc -Wall -g ex_c.c -o ex_c
	gcc -Wall -g ex_d.c -o ex_d

#run: ${EXEC}
#	./${EXEC}

clean:
	rm -f *.o ex_b ex_c ex_d

#vg:
#	valgrind ./${EXEC}

#debug:
#	gdb -tui ${EXEC}