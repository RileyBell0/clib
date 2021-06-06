all: projectMake projectScan testing list_files_in_dir

projectMake: projectMake.o console.o directory.o avlBinTree.o sorting.o list.o memory.o fileIO.o array.o stack.o filePath.o string.o configLoader.o argparse.o alist.o int.o
	clang -arch arm64 -Wall -O3 -o build/projectMake objects/projectMake.o objects/console.o objects/directory.o objects/avlBinTree.o objects/sorting.o objects/list.o objects/memory.o objects/fileIO.o objects/array.o objects/stack.o objects/filePath.o objects/string.o objects/configLoader.o objects/argparse.o objects/alist.o objects/int.o -g

projectScan: projectScan.o console.o directory.o avlBinTree.o sorting.o list.o memory.o fileIO.o array.o stack.o filePath.o string.o configLoader.o argparse.o alist.o int.o
	clang -arch arm64 -Wall -O3 -o build/projectScan objects/projectScan.o objects/console.o objects/directory.o objects/avlBinTree.o objects/sorting.o objects/list.o objects/memory.o objects/fileIO.o objects/array.o objects/stack.o objects/filePath.o objects/string.o objects/configLoader.o objects/argparse.o objects/alist.o objects/int.o -g

testing: testing.o console.o directory.o avlBinTree.o sorting.o list.o memory.o fileIO.o array.o stack.o filePath.o string.o configLoader.o argparse.o alist.o int.o
	clang -arch arm64 -Wall -O3 -o build/testing objects/testing.o objects/console.o objects/directory.o objects/avlBinTree.o objects/sorting.o objects/list.o objects/memory.o objects/fileIO.o objects/array.o objects/stack.o objects/filePath.o objects/string.o objects/configLoader.o objects/argparse.o objects/alist.o objects/int.o -g

list_files_in_dir: list_files_in_dir.o console.o directory.o avlBinTree.o sorting.o list.o memory.o fileIO.o array.o stack.o filePath.o string.o configLoader.o argparse.o alist.o int.o
	clang -arch arm64 -Wall -O3 -o build/list_files_in_dir objects/list_files_in_dir.o objects/console.o objects/directory.o objects/avlBinTree.o objects/sorting.o objects/list.o objects/memory.o objects/fileIO.o objects/array.o objects/stack.o objects/filePath.o objects/string.o objects/configLoader.o objects/argparse.o objects/alist.o objects/int.o -g

projectMake.o: src/main/projectMake.c
	clang -arch arm64 -Wall -O3 -c -o objects/projectMake.o src/main/projectMake.c -g

projectScan.o: src/main/projectScan.c
	clang -arch arm64 -Wall -O3 -c -o objects/projectScan.o src/main/projectScan.c -g

testing.o: src/main/testing.c
	clang -arch arm64 -Wall -O3 -c -o objects/testing.o src/main/testing.c -g

list_files_in_dir.o: src/main/list_files_in_dir.c
	clang -arch arm64 -Wall -O3 -c -o objects/list_files_in_dir.o src/main/list_files_in_dir.c -g

console.o: src/std/cmd/c/console.c
	clang -arch arm64 -Wall -O3 -c -o objects/console.o src/std/cmd/c/console.c -g

directory.o: src/std/dir/c/directory.c
	clang -arch arm64 -Wall -O3 -c -o objects/directory.o src/std/dir/c/directory.c -g

avlBinTree.o: src/std/c/avlBinTree.c
	clang -arch arm64 -Wall -O3 -c -o objects/avlBinTree.o src/std/c/avlBinTree.c -g

sorting.o: src/std/c/sorting.c
	clang -arch arm64 -Wall -O3 -c -o objects/sorting.o src/std/c/sorting.c -g

list.o: src/std/c/list.c
	clang -arch arm64 -Wall -O3 -c -o objects/list.o src/std/c/list.c -g

memory.o: src/std/c/memory.c
	clang -arch arm64 -Wall -O3 -c -o objects/memory.o src/std/c/memory.c -g

fileIO.o: src/std/c/fileIO.c
	clang -arch arm64 -Wall -O3 -c -o objects/fileIO.o src/std/c/fileIO.c -g

array.o: src/std/c/array.c
	clang -arch arm64 -Wall -O3 -c -o objects/array.o src/std/c/array.c -g

stack.o: src/std/c/stack.c
	clang -arch arm64 -Wall -O3 -c -o objects/stack.o src/std/c/stack.c -g

filePath.o: src/std/c/filePath.c
	clang -arch arm64 -Wall -O3 -c -o objects/filePath.o src/std/c/filePath.c -g

string.o: src/std/c/string.c
	clang -arch arm64 -Wall -O3 -c -o objects/string.o src/std/c/string.c -g

configLoader.o: src/std/c/configLoader.c
	clang -arch arm64 -Wall -O3 -c -o objects/configLoader.o src/std/c/configLoader.c -g

argparse.o: src/std/c/argparse.c
	clang -arch arm64 -Wall -O3 -c -o objects/argparse.o src/std/c/argparse.c -g

alist.o: src/std/c/alist.c
	clang -arch arm64 -Wall -O3 -c -o objects/alist.o src/std/c/alist.c -g

int.o: src/std/c/int.c
	clang -arch arm64 -Wall -O3 -c -o objects/int.o src/std/c/int.c -g

clean:
	rm objects/*.o build/*
