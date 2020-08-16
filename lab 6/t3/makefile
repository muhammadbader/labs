all : mypipeline

test:
	gcc -c -g test.c -o test.o
	gcc -g test.o -o test
	rm -rf test.o
ctest:
	rm -rf test 

mypipeline: mypipeline.o #LineParser.o
	gcc -g mypipeline.o -o mypipeline

# LineParser.o: LineParser.c
# 	gcc -c -g LineParser.c -o LineParser.o

mypipeline.o: mypipeline.c
	gcc -c -g mypipeline.c -o mypipeline.o

clean:
	rm -f ./*o mypipeline