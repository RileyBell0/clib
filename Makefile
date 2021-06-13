all: list_files_in_dir projectMake projectScan testing

list_files_in_dir: list_files_in_dir.o alist.o argparse.o array.o avlBinTree.o configLoader.o fileIO.o filePath.o int.o list.o memory.o sorting.o stack.o string.o console.o directory.o
	gcc -Wall -o build\list_files_in_dir objects\list_files_in_dir.o objects\alist.o objects\argparse.o objects\array.o objects\avlBinTree.o objects\configLoader.o objects\fileIO.o objects\filePath.o objects\int.o objects\list.o objects\memory.o objects\sorting.o objects\stack.o objects\string.o objects\console.o objects\directory.o -g

projectMake: projectMake.o alist.o argparse.o array.o avlBinTree.o configLoader.o fileIO.o filePath.o int.o list.o memory.o sorting.o stack.o string.o console.o directory.o
	gcc -Wall -o build\projectMake objects\projectMake.o objects\alist.o objects\argparse.o objects\array.o objects\avlBinTree.o objects\configLoader.o objects\fileIO.o objects\filePath.o objects\int.o objects\list.o objects\memory.o objects\sorting.o objects\stack.o objects\string.o objects\console.o objects\directory.o -g

projectScan: projectScan.o alist.o argparse.o array.o avlBinTree.o configLoader.o fileIO.o filePath.o int.o list.o memory.o sorting.o stack.o string.o console.o directory.o
	gcc -Wall -o build\projectScan objects\projectScan.o objects\alist.o objects\argparse.o objects\array.o objects\avlBinTree.o objects\configLoader.o objects\fileIO.o objects\filePath.o objects\int.o objects\list.o objects\memory.o objects\sorting.o objects\stack.o objects\string.o objects\console.o objects\directory.o -g

testing: testing.o alist.o argparse.o array.o avlBinTree.o configLoader.o fileIO.o filePath.o int.o list.o memory.o sorting.o stack.o string.o console.o directory.o
	gcc -Wall -o build\testing objects\testing.o objects\alist.o objects\argparse.o objects\array.o objects\avlBinTree.o objects\configLoader.o objects\fileIO.o objects\filePath.o objects\int.o objects\list.o objects\memory.o objects\sorting.o objects\stack.o objects\string.o objects\console.o objects\directory.o -g

list_files_in_dir.o: src\main\list_files_in_dir.c
	gcc -Wall -c -o objects\list_files_in_dir.o src\main\list_files_in_dir.c -g

projectMake.o: src\main\projectMake.c
	gcc -Wall -c -o objects\projectMake.o src\main\projectMake.c -g

projectScan.o: src\main\projectScan.c
	gcc -Wall -c -o objects\projectScan.o src\main\projectScan.c -g

testing.o: src\main\testing.c
	gcc -Wall -c -o objects\testing.o src\main\testing.c -g

alist.o: src\std\c\alist.c
	gcc -Wall -c -o objects\alist.o src\std\c\alist.c -g

argparse.o: src\std\c\argparse.c
	gcc -Wall -c -o objects\argparse.o src\std\c\argparse.c -g

array.o: src\std\c\array.c
	gcc -Wall -c -o objects\array.o src\std\c\array.c -g

avlBinTree.o: src\std\c\avlBinTree.c
	gcc -Wall -c -o objects\avlBinTree.o src\std\c\avlBinTree.c -g

configLoader.o: src\std\c\configLoader.c
	gcc -Wall -c -o objects\configLoader.o src\std\c\configLoader.c -g

fileIO.o: src\std\c\fileIO.c
	gcc -Wall -c -o objects\fileIO.o src\std\c\fileIO.c -g

filePath.o: src\std\c\filePath.c
	gcc -Wall -c -o objects\filePath.o src\std\c\filePath.c -g

int.o: src\std\c\int.c
	gcc -Wall -c -o objects\int.o src\std\c\int.c -g

list.o: src\std\c\list.c
	gcc -Wall -c -o objects\list.o src\std\c\list.c -g

memory.o: src\std\c\memory.c
	gcc -Wall -c -o objects\memory.o src\std\c\memory.c -g

sorting.o: src\std\c\sorting.c
	gcc -Wall -c -o objects\sorting.o src\std\c\sorting.c -g

stack.o: src\std\c\stack.c
	gcc -Wall -c -o objects\stack.o src\std\c\stack.c -g

string.o: src\std\c\string.c
	gcc -Wall -c -o objects\string.o src\std\c\string.c -g

console.o: src\std\cmd\c\console.c
	gcc -Wall -c -o objects\console.o src\std\cmd\c\console.c -g

directory.o: src\std\dir\c\directory.c
	gcc -Wall -c -o objects\directory.o src\std\dir\c\directory.c -g

clean:
	rm objects\*.o build\*
