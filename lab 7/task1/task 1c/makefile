all: hexeditplus

hexeditplus: task1c.o
	gcc -g task1c.o -o hexeditplus

task1c.o: 
	gcc -Wall -c task1c.c -o task1c.o

.PHONY: clean
 clean:
	rm -rf task1c.o hexeditplus