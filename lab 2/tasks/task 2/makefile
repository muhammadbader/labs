all: exec

exec: task2.c
	gcc -c -g menu_map.c -o menu_map.o
	gcc -g menu_map.o -o menu_map
	rm menu_map.o

.PHONY: clean
clean:
	rm -rf menu_map
