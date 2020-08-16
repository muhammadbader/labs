all: hexeditplus

hexeditplus: task1a.o
	gcc -g task1a.o -o hexeditplus

task1a.o: 
	gcc -Wall -c task1a.c -o task1a.o

.PHONY: clean
 clean:
	rm -rf task1a.o hexeditplus