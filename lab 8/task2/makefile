all: exec

exec: myELF.o
	gcc -m32 -g -Wall -o myELF myELF.o

myELF.o: myELF.c
	gcc -m32 -c -g -Wall -o myELF.o myELF.c

,PHONY: clean

clean:
	rm *.o myELF