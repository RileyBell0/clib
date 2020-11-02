all: projectMake testing projectScan 

projectMake: projectMake.o binaryTree.o stacklist.o directory.o list.o string.o fileIO.o array.o general.o 
	gcc -Wall -o projectMake objects/projectMake.o objects/binaryTree.o objects/stacklist.o objects/directory.o objects/list.o objects/string.o objects/fileIO.o objects/array.o objects/general.o  -g -lm

testing: testing.o binaryTree.o stacklist.o directory.o list.o string.o fileIO.o array.o general.o 
	gcc -Wall -o testing objects/testing.o objects/binaryTree.o objects/stacklist.o objects/directory.o objects/list.o objects/string.o objects/fileIO.o objects/array.o objects/general.o  -g -lm

projectScan: projectScan.o binaryTree.o stacklist.o directory.o list.o string.o fileIO.o array.o general.o 
	gcc -Wall -o projectScan objects/projectScan.o objects/binaryTree.o objects/stacklist.o objects/directory.o objects/list.o objects/string.o objects/fileIO.o objects/array.o objects/general.o  -g -lm

projectMake.o: main/projectMake.c
	gcc -Wall -c -o objects/projectMake.o main/projectMake.c -g -lm

testing.o: main/testing.c
	gcc -Wall -c -o objects/testing.o main/testing.c -g -lm

projectScan.o: main/projectScan.c
	gcc -Wall -c -o objects/projectScan.o main/projectScan.c -g -lm

binaryTree.o: /usr/include/clib/std/c/binaryTree.c
	gcc -Wall -c -o objects/binaryTree.o /usr/include/clib/std/c/binaryTree.c -g -lm

stacklist.o: /usr/include/clib/std/c/stacklist.c
	gcc -Wall -c -o objects/stacklist.o /usr/include/clib/std/c/stacklist.c -g -lm

directory.o: /usr/include/clib/std/c/directory.c
	gcc -Wall -c -o objects/directory.o /usr/include/clib/std/c/directory.c -g -lm

list.o: /usr/include/clib/std/c/list.c
	gcc -Wall -c -o objects/list.o /usr/include/clib/std/c/list.c -g -lm

string.o: /usr/include/clib/std/c/string.c
	gcc -Wall -c -o objects/string.o /usr/include/clib/std/c/string.c -g -lm

fileIO.o: /usr/include/clib/std/c/fileIO.c
	gcc -Wall -c -o objects/fileIO.o /usr/include/clib/std/c/fileIO.c -g -lm

array.o: /usr/include/clib/std/c/array.c
	gcc -Wall -c -o objects/array.o /usr/include/clib/std/c/array.c -g -lm

general.o: /usr/include/clib/std/c/general.c
	gcc -Wall -c -o objects/general.o /usr/include/clib/std/c/general.c -g -lm

