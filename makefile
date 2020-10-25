all: testing 

testing: testing.o list.o directory.o string.o general.o array.o fileIO.o binaryTree.o 
	gcc -Wall -o testing objects/testing.o objects/list.o objects/directory.o objects/string.o objects/general.o objects/array.o objects/fileIO.o objects/binaryTree.o  -g -lm

testing.o: main/testing.c
	gcc -Wall -c -o objects/testing.o main/testing.c -g -lm

list.o: /usr/include/clib/std/c/list.c
	gcc -Wall -c -o objects/list.o /usr/include/clib/std/c/list.c -g -lm

directory.o: /usr/include/clib/std/c/directory.c
	gcc -Wall -c -o objects/directory.o /usr/include/clib/std/c/directory.c -g -lm

string.o: /usr/include/clib/std/c/string.c
	gcc -Wall -c -o objects/string.o /usr/include/clib/std/c/string.c -g -lm

general.o: /usr/include/clib/std/c/general.c
	gcc -Wall -c -o objects/general.o /usr/include/clib/std/c/general.c -g -lm

array.o: /usr/include/clib/std/c/array.c
	gcc -Wall -c -o objects/array.o /usr/include/clib/std/c/array.c -g -lm

fileIO.o: /usr/include/clib/std/c/fileIO.c
	gcc -Wall -c -o objects/fileIO.o /usr/include/clib/std/c/fileIO.c -g -lm

binaryTree.o: /usr/include/clib/std/c/binaryTree.c
	gcc -Wall -c -o objects/binaryTree.o /usr/include/clib/std/c/binaryTree.c -g -lm

