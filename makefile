all: main visualise
	gcc main.o visualise.o -o out
	rm *.o
	./out

main: main.c
	gcc -c main.c -o main.o

visualise: visualise.c
	gcc -c visualise.c -o visualise.o