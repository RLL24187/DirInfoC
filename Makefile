all: dirinfo.o
	gcc -o dirinfo.out dirinfo.o

dirinfo.o: dirinfo.c
	gcc -c dirinfo.c

clean:
	rm -f *.o *.out dirinfo

run:
	./dirinfo.out
