fdriver: driver.c sq.o 
	gcc driver.c sq.o -lc -o fdriver

sq.o: sq.h sq.c
	gcc -c sq.c

llist.o:  list.h llist.c
	gcc -c llist.c

sq_slow.o: llist.o sq.h sq_slow.c
	gcc -c sq_slow.c

driver: driver.c sq_slow.o
	gcc driver.c sq_slow.o llist.o -lc -o driver


