all: exec

exec: task2.c
	gcc -c -g task2.c -o task2.o	
	gcc -g task2.o -o task2
	rm task2.o

.PHONY: clean
clean:
	rm -rf task2