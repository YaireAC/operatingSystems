CC=gcc --std=c99 -g
exe_file = movies
$(exe_file): new.o dynarray.o
	$(CC) new.o dynarray.o -o $(exe_file)
new.o: new.c
	$(CC) -c new.c
dynarray.o: dynarray.c dynarray.h
	$(CC) -c dynarray.c
clean:
	rm -f *.out *.o $(exe_file)
