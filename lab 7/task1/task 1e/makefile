all: hexeditplus

hexeditplus: task1e.o
	gcc -g task1e.o -o hexeditplus

task1e.o: 
	gcc -Wall -c task1e.c -o task1e.o

.PHONY: clean
 clean:
	rm -rf task1e.o hexeditplus