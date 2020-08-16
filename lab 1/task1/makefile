all:exec

exec: encoder.c
	gcc -g -c -o encoder.o encoder.c
	gcc -g encoder.o -o encoder
	rm encoder.o

.PHONY: clean
clean:
	rm -rf encoder