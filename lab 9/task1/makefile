all: virus

virus: task1.o 
	ld -melf_i386 task1.o -o virus
	

 
task1.o: task1.s
	nasm -f elf32 task1.s -o task1.o


.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o virus