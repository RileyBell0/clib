all: projectMake projectScan main testing

projectMake: projectMake.o directory.o avlBinTree.o sorting.o list.o fileIO.o array.o stacklist.o filePath.o string.o configLoader.o alist.o general.o int.o
	clang -arch arm64 -Wall -O3 -o build/projectMake objects/projectMake.o objects/directory.o objects/avlBinTree.o objects/sorting.o objects/list.o objects/fileIO.o objects/array.o objects/stacklist.o objects/filePath.o objects/string.o objects/configLoader.o objects/alist.o objects/general.o objects/int.o -g

projectScan: projectScan.o directory.o avlBinTree.o sorting.o list.o fileIO.o array.o stacklist.o filePath.o string.o configLoader.o alist.o general.o int.o
	clang -arch arm64 -Wall -O3 -o build/projectScan objects/projectScan.o objects/directory.o objects/avlBinTree.o objects/sorting.o objects/list.o objects/fileIO.o objects/array.o objects/stacklist.o objects/filePath.o objects/string.o objects/configLoader.o objects/alist.o objects/general.o objects/int.o -g

main: main.o directory.o avlBinTree.o sorting.o list.o fileIO.o array.o stacklist.o filePath.o string.o configLoader.o alist.o general.o int.o
	clang -arch arm64 -Wall -O3 -o build/main objects/main.o objects/directory.o objects/avlBinTree.o objects/sorting.o objects/list.o objects/fileIO.o objects/array.o objects/stacklist.o objects/filePath.o objects/string.o objects/configLoader.o objects/alist.o objects/general.o objects/int.o -g

testing: testing.o directory.o avlBinTree.o sorting.o list.o fileIO.o array.o stacklist.o filePath.o string.o configLoader.o alist.o general.o int.o
	clang -arch arm64 -Wall -O3 -o build/testing objects/testing.o objects/directory.o objects/avlBinTree.o objects/sorting.o objects/list.o objects/fileIO.o objects/array.o objects/stacklist.o objects/filePath.o objects/string.o objects/configLoader.o objects/alist.o objects/general.o objects/int.o -g

projectMake.o: main/projectMake.c
	clang -arch arm64 -Wall -O3 -c -o objects/projectMake.o main/projectMake.c -g

projectScan.o: main/projectScan.c
	clang -arch arm64 -Wall -O3 -c -o objects/projectScan.o main/projectScan.c -g

main.o: main/main.c
	clang -arch arm64 -Wall -O3 -c -o objects/main.o main/main.c -g

testing.o: main/testing.c
	clang -arch arm64 -Wall -O3 -c -o objects/testing.o main/testing.c -g

directory.o: std/dir/c/directory.c
	clang -arch arm64 -Wall -O3 -c -o objects/directory.o std/dir/c/directory.c -g

avlBinTree.o: std/c/avlBinTree.c
	clang -arch arm64 -Wall -O3 -c -o objects/avlBinTree.o std/c/avlBinTree.c -g

sorting.o: std/c/sorting.c
	clang -arch arm64 -Wall -O3 -c -o objects/sorting.o std/c/sorting.c -g

list.o: std/c/list.c
	clang -arch arm64 -Wall -O3 -c -o objects/list.o std/c/list.c -g

fileIO.o: std/c/fileIO.c
	clang -arch arm64 -Wall -O3 -c -o objects/fileIO.o std/c/fileIO.c -g

array.o: std/c/array.c
	clang -arch arm64 -Wall -O3 -c -o objects/array.o std/c/array.c -g

stacklist.o: std/c/stacklist.c
	clang -arch arm64 -Wall -O3 -c -o objects/stacklist.o std/c/stacklist.c -g

filePath.o: std/c/filePath.c
	clang -arch arm64 -Wall -O3 -c -o objects/filePath.o std/c/filePath.c -g

string.o: std/c/string.c
	clang -arch arm64 -Wall -O3 -c -o objects/string.o std/c/string.c -g

configLoader.o: std/c/configLoader.c
	clang -arch arm64 -Wall -O3 -c -o objects/configLoader.o std/c/configLoader.c -g

alist.o: std/c/alist.c
	clang -arch arm64 -Wall -O3 -c -o objects/alist.o std/c/alist.c -g

general.o: std/c/general.c
	clang -arch arm64 -Wall -O3 -c -o objects/general.o std/c/general.c -g

int.o: std/c/int.c
	clang -arch arm64 -Wall -O3 -c -o objects/int.o std/c/int.c -g

clean:
	rm objects/*.o build/*
