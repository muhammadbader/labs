all : myShell

myShell: myShell.o LineParser.o
	gcc myShell.o LineParser.o -o myShell

LineParser.o: LineParser.c
	gcc -c -Wall -g LineParser.c -o LineParser.o

myShell.o: myShell.c
	gcc -c -Wall -g myShell.c -o myShell.o

clean:
	rm -f ./*o myShell