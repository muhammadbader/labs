all: hexeditplus

hexeditplus: task1d.o
	gcc -g task1d.o -o hexeditplus

task1d.o: 
	gcc -Wall -c task1d.c -o task1d.o

.PHONY: clean
 clean:
	rm -rf task1d.o hexeditplus