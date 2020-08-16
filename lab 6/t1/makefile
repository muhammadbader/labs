all : myShell

test: 
	gcc -c -g test.c -o test.o
	gcc -g test.o -o test
ctest:
	rm -rf test test.o	

myShell: myShell.o LineParser.o
	gcc -g myShell.o LineParser.o -o myShell

LineParser.o: LineParser.c
	gcc -c -g LineParser.c -o LineParser.o

myShell.o: myShell.c
	gcc -c -g myShell.c -o myShell.o

clean:
	rm -f ./*o myShell