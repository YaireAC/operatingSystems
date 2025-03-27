CC=gcc --std=c99 -g
exe_file = movies_by_year
$(exe_file): main.o
	$(CC) main.o -o $(exe_file)
main.o: main.c
	$(CC) -c main.c
clean:
	rm -f *.out *.o $(exe_file)