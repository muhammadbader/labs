all: exec

exec: menu_map.c
	gcc -g -c menu_map.c -o menu_map.o
	gcc -g menu_map.o -o menu_map
	rm menu_map.o

.PHONY: clean
clean:
	rm -rf menu_map
