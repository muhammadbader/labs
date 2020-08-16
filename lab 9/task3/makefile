all: virus

virus: task3.o
	ld -melf_i386 task3.o -o virus

task3.o: task3.s
	nasm -f elf task3.s -o task3.o


.PHONY: clean

clean:
	rm -f *.o virus