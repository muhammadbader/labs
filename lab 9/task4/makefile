all: virus

virus: task4.o
	ld -melf_i386 task4.o -o virus

task4.o: task4.s
	nasm -f elf task4.s -o task4.o


.PHONY: clean

clean:
	rm -f *.o virus