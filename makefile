all: testing 

testing: testing.o list.o directory.o string.o general.o array.o fileIO.o binaryTree.o cool.o 
	gcc -Wall -o testing objects/testing.o objects/list.o objects/directory.o objects/string.o objects/general.o objects/array.o objects/fileIO.o objects/binaryTree.o objects/cool.o  -g -lm

testing.o: main/testing.c
	gcc -Wall -c -o objects/testing.o main/testing.c -g -lm

list.o: std/c/list.c
	gcc -Wall -c -o objects/list.o std/c/list.c -g -lm

directory.o: std/c/directory.c
	gcc -Wall -c -o objects/directory.o std/c/directory.c -g -lm

string.o: std/c/string.c
	gcc -Wall -c -o objects/string.o std/c/string.c -g -lm

general.o: std/c/general.c
	gcc -Wall -c -o objects/general.o std/c/general.c -g -lm

array.o: std/c/array.c
	gcc -Wall -c -o objects/array.o std/c/array.c -g -lm

fileIO.o: std/c/fileIO.c
	gcc -Wall -c -o objects/fileIO.o std/c/fileIO.c -g -lm

binaryTree.o: std/c/binaryTree.c
	gcc -Wall -c -o objects/binaryTree.o std/c/binaryTree.c -g -lm

cool.o: src/cool.c
	gcc -Wall -c -o objects/cool.o src/cool.c -g -lm

