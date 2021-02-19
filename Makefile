all: projectScan testing

projectScan: projectScan.o avlBinTree.o directory.o sorting.o list.o fileIO.o array.o stacklist.o string.o configLoader.o general.o int.o
	clang -Wall -O3 -o build/projectScan objects/projectScan.o objects/avlBinTree objects/directory objects/sorting objects/list objects/fileIO objects/array objects/stacklist objects/string objects/configLoader objects/general objects/int -g

projectScan.o: main/projectScan.c
	clang -Wall -O3 -c -o objects/projectScan.o main/projectScan.c -g

testing: testing.o avlBinTree.o directory.o sorting.o list.o fileIO.o array.o stacklist.o string.o configLoader.o general.o int.o
	clang -Wall -O3 -o build/testing objects/testing.o objects/avlBinTree objects/directory objects/sorting objects/list objects/fileIO objects/array objects/stacklist objects/string objects/configLoader objects/general objects/int -g

testing.o: main/testing.c
	clang -Wall -O3 -c -o objects/testing.o main/testing.c -g

avlBinTree.o: std/c/avlBinTree.c
	clang -Wall -O3 -c -o objects/avlBinTree std/c/avlBinTree.c -g

directory.o: std/c/directory.c
	clang -Wall -O3 -c -o objects/directory std/c/directory.c -g

sorting.o: std/c/sorting.c
	clang -Wall -O3 -c -o objects/sorting std/c/sorting.c -g

list.o: std/c/list.c
	clang -Wall -O3 -c -o objects/list std/c/list.c -g

fileIO.o: std/c/fileIO.c
	clang -Wall -O3 -c -o objects/fileIO std/c/fileIO.c -g

array.o: std/c/array.c
	clang -Wall -O3 -c -o objects/array std/c/array.c -g

stacklist.o: std/c/stacklist.c
	clang -Wall -O3 -c -o objects/stacklist std/c/stacklist.c -g

string.o: std/c/string.c
	clang -Wall -O3 -c -o objects/string std/c/string.c -g

configLoader.o: std/c/configLoader.c
	clang -Wall -O3 -c -o objects/configLoader std/c/configLoader.c -g

general.o: std/c/general.c
	clang -Wall -O3 -c -o objects/general std/c/general.c -g

int.o: std/c/int.c
	clang -Wall -O3 -c -o objects/int std/c/int.c -g

clean:
	rm objects/*.o build/*
