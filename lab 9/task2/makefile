all: virus

virus: task2.o 
	ld -melf_i386 task2.o -o virus
	

 
task2.o: task2.s
	nasm -f elf32 task2.s -o task2.o


.PHONY: clean


clean: 
	rm -f *.o virus