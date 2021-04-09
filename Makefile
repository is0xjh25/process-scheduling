allocate: main.o utilities.o scheduler.o
	gcc -o allocate main.o utilities.o scheduler.o -lm
main.o: main.c
	gcc -c -Wall main.c
utilites.o: utilites.c utilities.h
	gcc -c -Wall utilities.c
scheduler.o: scheduler.c scheduler.h
	gcc -c -Wall scheduler.c


clean:
	rm -f *.o
	rm -f allocate
