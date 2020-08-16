all: exec

exec: task1.c
	gcc -g -c task1.c -o task1.o
	gcc -g task1.o -o task1
	rm -rf task1.o 

.PHONY: clean
clean:
	rm -rf task1