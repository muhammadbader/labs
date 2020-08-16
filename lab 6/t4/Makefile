all: myshell

myshell: task4.o LineParser.o
	gcc -g -Wall -o myshell task4.o LineParser.o

task4.o: task4.c
	gcc -g -Wall -c -o task4.o task4.c

LineParser.o: LineParser.c LineParser.h
	gcc -g -Wall -c -o LineParser.o LineParser.c 

.PHONY: clean

clean: 
	rm -f *.o myshell
