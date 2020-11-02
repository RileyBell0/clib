all: projectMake testing projectScan 

projectMake: projectMake.o binaryTree.o stacklist.o directory.o list.o string.o fileIO.o array.o general.o 
	gcc -Wall -o projectMake objects/projectMake.o objects/binaryTree.o objects/stacklist.o objects/directory.o objects/list.o objects/string.o objects/fileIO.o objects/array.o objects/general.o  -g

testing: testing.o binaryTree.o stacklist.o directory.o list.o string.o fileIO.o array.o general.o 
	gcc -Wall -o testing objects/testing.o objects/binaryTree.o objects/stacklist.o objects/directory.o objects/list.o objects/string.o objects/fileIO.o objects/array.o objects/general.o  -g

projectScan: projectScan.o binaryTree.o stacklist.o directory.o list.o string.o fileIO.o array.o general.o 
	gcc -Wall -o projectScan objects/projectScan.o objects/binaryTree.o objects/stacklist.o objects/directory.o objects/list.o objects/string.o objects/fileIO.o objects/array.o objects/general.o  -g

projectMake.o: main/projectMake.c
	gcc -Wall -c -o objects/projectMake.o main/projectMake.c -g

testing.o: main/testing.c
	gcc -Wall -c -o objects/testing.o main/testing.c -g

projectScan.o: main/projectScan.c
	gcc -Wall -c -o objects/projectScan.o main/projectScan.c -g

binaryTree.o: /usr/include/clib/std/c/binaryTree.c
	gcc -Wall -c -o objects/binaryTree.o /usr/include/clib/std/c/binaryTree.c -g

stacklist.o: /usr/include/clib/std/c/stacklist.c
	gcc -Wall -c -o objects/stacklist.o /usr/include/clib/std/c/stacklist.c -g

directory.o: /usr/include/clib/std/c/directory.c
	gcc -Wall -c -o objects/directory.o /usr/include/clib/std/c/directory.c -g

list.o: /usr/include/clib/std/c/list.c
	gcc -Wall -c -o objects/list.o /usr/include/clib/std/c/list.c -g

string.o: /usr/include/clib/std/c/string.c
	gcc -Wall -c -o objects/string.o /usr/include/clib/std/c/string.c -g

fileIO.o: /usr/include/clib/std/c/fileIO.c
	gcc -Wall -c -o objects/fileIO.o /usr/include/clib/std/c/fileIO.c -g

array.o: /usr/include/clib/std/c/array.c
	gcc -Wall -c -o objects/array.o /usr/include/clib/std/c/array.c -g

general.o: /usr/include/clib/std/c/general.c
	gcc -Wall -c -o objects/general.o /usr/include/clib/std/c/general.c -g

