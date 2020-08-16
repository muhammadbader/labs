all: exec

exec: tgreeting.s
	nasm -f elf32 tgreeting.s -o ggreet.o
	ld -m elf_i386 ggreet.o -o ggreet
	rm ggreet.o
.PHONY: clean
clean:
	rm ggreet