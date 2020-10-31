all: testing 

testing: testing.o binaryTree.o stacklist.o directory.o list.o string.o fileIO.o array.o general.o 
	gcc -Wall -o testing objects/testing.o objects/binaryTree.o objects/stacklist.o objects/directory.o objects/list.o objects/string.o objects/fileIO.o objects/array.o objects/general.o  -g -lm

testing.o: main/testing.c
	gcc -Wall -c -o objects/testing.o main/testing.c -g -lm

binaryTree.o: /usr/include/clib/std/binaryTree.c
	gcc -Wall -c -o objects/binaryTree.o /usr/include/clib/std/binaryTree.c -g -lm

stacklist.o: /usr/include/clib/std/stacklist.c
	gcc -Wall -c -o objects/stacklist.o /usr/include/clib/std/stacklist.c -g -lm

directory.o: /usr/include/clib/std/directory.c
	gcc -Wall -c -o objects/directory.o /usr/include/clib/std/directory.c -g -lm

list.o: /usr/include/clib/std/list.c
	gcc -Wall -c -o objects/list.o /usr/include/clib/std/list.c -g -lm

string.o: /usr/include/clib/std/string.c
	gcc -Wall -c -o objects/string.o /usr/include/clib/std/string.c -g -lm

fileIO.o: /usr/include/clib/std/fileIO.c
	gcc -Wall -c -o objects/fileIO.o /usr/include/clib/std/fileIO.c -g -lm

array.o: /usr/include/clib/std/array.c
	gcc -Wall -c -o objects/array.o /usr/include/clib/std/array.c -g -lm

general.o: /usr/include/clib/std/general.c
	gcc -Wall -c -o objects/general.o /usr/include/clib/std/general.c -g -lm

