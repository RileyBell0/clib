all: projectMake projectScan testing

projectMake: projectMake.o directory.o avlBinTree.o sorting.o list.o fileIO.o array.o stacklist.o filePath.o string.o configLoader.o alist.o general.o int.o
	clang -arch arm64 -Wall -O3 -o build/projectMake objects/projectMake.o objects/directory.o objects/avlBinTree.o objects/sorting.o objects/list.o objects/fileIO.o objects/array.o objects/stacklist.o objects/filePath.o objects/string.o objects/configLoader.o objects/alist.o objects/general.o objects/int.o -g

projectScan: projectScan.o directory.o avlBinTree.o sorting.o list.o fileIO.o array.o stacklist.o filePath.o string.o configLoader.o alist.o general.o int.o
	clang -arch arm64 -Wall -O3 -o build/projectScan objects/projectScan.o objects/directory.o objects/avlBinTree.o objects/sorting.o objects/list.o objects/fileIO.o objects/array.o objects/stacklist.o objects/filePath.o objects/string.o objects/configLoader.o objects/alist.o objects/general.o objects/int.o -g

testing: testing.o directory.o avlBinTree.o sorting.o list.o fileIO.o array.o stacklist.o filePath.o string.o configLoader.o alist.o general.o int.o
	clang -arch arm64 -Wall -O3 -o build/testing objects/testing.o objects/directory.o objects/avlBinTree.o objects/sorting.o objects/list.o objects/fileIO.o objects/array.o objects/stacklist.o objects/filePath.o objects/string.o objects/configLoader.o objects/alist.o objects/general.o objects/int.o -g

projectMake.o: src/main/projectMake.c
	clang -arch arm64 -Wall -O3 -c -o objects/projectMake.o src/main/projectMake.c -g

projectScan.o: src/main/projectScan.c
	clang -arch arm64 -Wall -O3 -c -o objects/projectScan.o src/main/projectScan.c -g

testing.o: src/main/testing.c
	clang -arch arm64 -Wall -O3 -c -o objects/testing.o src/main/testing.c -g

directory.o: src/std/dir/c/directory.c
	clang -arch arm64 -Wall -O3 -c -o objects/directory.o src/std/dir/c/directory.c -g

avlBinTree.o: src/std/c/avlBinTree.c
	clang -arch arm64 -Wall -O3 -c -o objects/avlBinTree.o src/std/c/avlBinTree.c -g

sorting.o: src/std/c/sorting.c
	clang -arch arm64 -Wall -O3 -c -o objects/sorting.o src/std/c/sorting.c -g

list.o: src/std/c/list.c
	clang -arch arm64 -Wall -O3 -c -o objects/list.o src/std/c/list.c -g

fileIO.o: src/std/c/fileIO.c
	clang -arch arm64 -Wall -O3 -c -o objects/fileIO.o src/std/c/fileIO.c -g

array.o: src/std/c/array.c
	clang -arch arm64 -Wall -O3 -c -o objects/array.o src/std/c/array.c -g

stacklist.o: src/std/c/stacklist.c
	clang -arch arm64 -Wall -O3 -c -o objects/stacklist.o src/std/c/stacklist.c -g

filePath.o: src/std/c/filePath.c
	clang -arch arm64 -Wall -O3 -c -o objects/filePath.o src/std/c/filePath.c -g

string.o: src/std/c/string.c
	clang -arch arm64 -Wall -O3 -c -o objects/string.o src/std/c/string.c -g

configLoader.o: src/std/c/configLoader.c
	clang -arch arm64 -Wall -O3 -c -o objects/configLoader.o src/std/c/configLoader.c -g

alist.o: src/std/c/alist.c
	clang -arch arm64 -Wall -O3 -c -o objects/alist.o src/std/c/alist.c -g

general.o: src/std/c/general.c
	clang -arch arm64 -Wall -O3 -c -o objects/general.o src/std/c/general.c -g

int.o: src/std/c/int.c
	clang -arch arm64 -Wall -O3 -c -o objects/int.o src/std/c/int.c -g

clean:
	rm objects/*.o build/*
