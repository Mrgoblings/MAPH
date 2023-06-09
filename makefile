all: main visualize
	gcc main.o visualize.o -o out
	rm *.o
	./out

main: main.c
	gcc -c main.c -o main.o

visualise: visualize.c
	gcc -c visualize.c -o visualize.o