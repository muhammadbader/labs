all : myShell

task0: 
	gcc -c -g mypipe.c -o mypipe.o
	gcc -g mypipe.o -o mypipe
clean0:
	rm -rf mypipe mypipe.o	

myShell: myShell.o LineParser.o
	gcc -g myShell.o LineParser.o -o myShell

LineParser.o: LineParser.c
	gcc -c -g LineParser.c -o LineParser.o

myShell.o: myShell.c
	gcc -c -g myShell.c -o myShell.o

clean:
	rm -f ./*o myShell