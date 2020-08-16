all: hexeditplus

hexeditplus: task1b.o
	gcc -g task1b.o -o hexeditplus

task1b.o: 
	gcc -Wall -c task1b.c -o task1b.o

.PHONY: clean
 clean:
	rm -rf task1b.o hexeditplus