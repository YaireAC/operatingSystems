
CC=gcc --std=c99 -g
exe_file = smallsh
$(exe_file): main.o dynarray.o
	$(CC) main.o dynarray.o -o $(exe_file)
main.o: main.c
	$(CC) -c main.c
dynarray.o: dynarray.c dynarray.h
	$(CC) -c dynarray.c
clean:
	rm -f *.out *.o $(exe_file)