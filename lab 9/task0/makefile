all: virus

virus: task0.o
	ld -melf_i386 task0.o -o virus

task0: task0.s
	nasm -f elf32 task0.s -o task0.o


.PHONY: clean
clean:
	rm -f *.o virus