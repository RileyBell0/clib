all: projectMake testing list_files_in_dir projectScan

projectMake: projectMake.o configLoader.o int.o memory.o avlBinTree.o stack.o list.o string.o fileIO.o argparse.o array.o alist.o filePath.o sorting.o console.o directory.o
	clang -Wall -o build/projectMake objects/projectMake.o objects/configLoader.o objects/int.o objects/memory.o objects/avlBinTree.o objects/stack.o objects/list.o objects/string.o objects/fileIO.o objects/argparse.o objects/array.o objects/alist.o objects/filePath.o objects/sorting.o objects/console.o objects/directory.o -g

testing: testing.o configLoader.o int.o memory.o avlBinTree.o stack.o list.o string.o fileIO.o argparse.o array.o alist.o filePath.o sorting.o console.o directory.o
	clang -Wall -o build/testing objects/testing.o objects/configLoader.o objects/int.o objects/memory.o objects/avlBinTree.o objects/stack.o objects/list.o objects/string.o objects/fileIO.o objects/argparse.o objects/array.o objects/alist.o objects/filePath.o objects/sorting.o objects/console.o objects/directory.o -g

list_files_in_dir: list_files_in_dir.o configLoader.o int.o memory.o avlBinTree.o stack.o list.o string.o fileIO.o argparse.o array.o alist.o filePath.o sorting.o console.o directory.o
	clang -Wall -o build/list_files_in_dir objects/list_files_in_dir.o objects/configLoader.o objects/int.o objects/memory.o objects/avlBinTree.o objects/stack.o objects/list.o objects/string.o objects/fileIO.o objects/argparse.o objects/array.o objects/alist.o objects/filePath.o objects/sorting.o objects/console.o objects/directory.o -g

projectScan: projectScan.o configLoader.o int.o memory.o avlBinTree.o stack.o list.o string.o fileIO.o argparse.o array.o alist.o filePath.o sorting.o console.o directory.o
	clang -Wall -o build/projectScan objects/projectScan.o objects/configLoader.o objects/int.o objects/memory.o objects/avlBinTree.o objects/stack.o objects/list.o objects/string.o objects/fileIO.o objects/argparse.o objects/array.o objects/alist.o objects/filePath.o objects/sorting.o objects/console.o objects/directory.o -g

projectMake.o: src/main/projectMake.c
	clang -Wall -c -o objects/projectMake.o src/main/projectMake.c -g

testing.o: src/main/testing.c
	clang -Wall -c -o objects/testing.o src/main/testing.c -g

list_files_in_dir.o: src/main/list_files_in_dir.c
	clang -Wall -c -o objects/list_files_in_dir.o src/main/list_files_in_dir.c -g

projectScan.o: src/main/projectScan.c
	clang -Wall -c -o objects/projectScan.o src/main/projectScan.c -g

configLoader.o: src/std/c/configLoader.c
	clang -Wall -c -o objects/configLoader.o src/std/c/configLoader.c -g

int.o: src/std/c/int.c
	clang -Wall -c -o objects/int.o src/std/c/int.c -g

memory.o: src/std/c/memory.c
	clang -Wall -c -o objects/memory.o src/std/c/memory.c -g

avlBinTree.o: src/std/c/avlBinTree.c
	clang -Wall -c -o objects/avlBinTree.o src/std/c/avlBinTree.c -g

stack.o: src/std/c/stack.c
	clang -Wall -c -o objects/stack.o src/std/c/stack.c -g

list.o: src/std/c/list.c
	clang -Wall -c -o objects/list.o src/std/c/list.c -g

string.o: src/std/c/string.c
	clang -Wall -c -o objects/string.o src/std/c/string.c -g

fileIO.o: src/std/c/fileIO.c
	clang -Wall -c -o objects/fileIO.o src/std/c/fileIO.c -g

argparse.o: src/std/c/argparse.c
	clang -Wall -c -o objects/argparse.o src/std/c/argparse.c -g

array.o: src/std/c/array.c
	clang -Wall -c -o objects/array.o src/std/c/array.c -g

alist.o: src/std/c/alist.c
	clang -Wall -c -o objects/alist.o src/std/c/alist.c -g

filePath.o: src/std/c/filePath.c
	clang -Wall -c -o objects/filePath.o src/std/c/filePath.c -g

sorting.o: src/std/c/sorting.c
	clang -Wall -c -o objects/sorting.o src/std/c/sorting.c -g

console.o: src/std/cmd/c/console.c
	clang -Wall -c -o objects/console.o src/std/cmd/c/console.c -g

directory.o: src/std/dir/c/directory.c
	clang -Wall -c -o objects/directory.o src/std/dir/c/directory.c -g

clean:
	rm objects/*.o build/*
