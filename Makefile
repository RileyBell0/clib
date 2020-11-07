all: projectMake testing projectScan 

projectMake: projectMake.o configLoader.o int.o stacklist.o avlBinTree.o directory.o list.o string.o fileIO.o array.o general.o sorting.o 
	gcc -Wall -o projectMake objects/projectMake.o objects/configLoader.o objects/int.o objects/stacklist.o objects/avlBinTree.o objects/directory.o objects/list.o objects/string.o objects/fileIO.o objects/array.o objects/general.o objects/sorting.o  -g

testing: testing.o configLoader.o int.o stacklist.o avlBinTree.o directory.o list.o string.o fileIO.o array.o general.o sorting.o 
	gcc -Wall -o testing objects/testing.o objects/configLoader.o objects/int.o objects/stacklist.o objects/avlBinTree.o objects/directory.o objects/list.o objects/string.o objects/fileIO.o objects/array.o objects/general.o objects/sorting.o  -g

projectScan: projectScan.o configLoader.o int.o stacklist.o avlBinTree.o directory.o list.o string.o fileIO.o array.o general.o sorting.o 
	gcc -Wall -o projectScan objects/projectScan.o objects/configLoader.o objects/int.o objects/stacklist.o objects/avlBinTree.o objects/directory.o objects/list.o objects/string.o objects/fileIO.o objects/array.o objects/general.o objects/sorting.o  -g

projectMake.o: main/projectMake.c
	gcc -Wall -c -o objects/projectMake.o main/projectMake.c -g

testing.o: main/testing.c
	gcc -Wall -c -o objects/testing.o main/testing.c -g

projectScan.o: main/projectScan.c
	gcc -Wall -c -o objects/projectScan.o main/projectScan.c -g

configLoader.o: /usr/include/clib/std/c/configLoader.c
	gcc -Wall -c -o objects/configLoader.o /usr/include/clib/std/c/configLoader.c -g

int.o: /usr/include/clib/std/c/int.c
	gcc -Wall -c -o objects/int.o /usr/include/clib/std/c/int.c -g

stacklist.o: /usr/include/clib/std/c/stacklist.c
	gcc -Wall -c -o objects/stacklist.o /usr/include/clib/std/c/stacklist.c -g

avlBinTree.o: /usr/include/clib/std/c/avlBinTree.c
	gcc -Wall -c -o objects/avlBinTree.o /usr/include/clib/std/c/avlBinTree.c -g

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

sorting.o: /usr/include/clib/std/c/sorting.c
	gcc -Wall -c -o objects/sorting.o /usr/include/clib/std/c/sorting.c -g

