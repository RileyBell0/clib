all: main 

main: main.o dict.o list.o array.o vector.o filter.o tree.o directory.o error.o config.o memory.o fileIO.o string.o index.o
	gcc -arch arm64 -Wall -o output/build/main output/objects/main.o output/objects/dict.o output/objects/list.o output/objects/array.o output/objects/vector.o output/objects/filter.o output/objects/tree.o output/objects/directory.o output/objects/error.o output/objects/config.o output/objects/memory.o output/objects/fileIO.o output/objects/string.o output/objects/index.o 

main.o: src/main/main.c
	gcc -arch arm64 -Wall -c -o output/objects/main.o src/main/main.c 

dict.o: src/std/struc/c/dict.c
	gcc -arch arm64 -Wall -c -o output/objects/dict.o src/std/struc/c/dict.c 

list.o: src/std/struc/c/list.c
	gcc -arch arm64 -Wall -c -o output/objects/list.o src/std/struc/c/list.c 

array.o: src/std/struc/c/array.c
	gcc -arch arm64 -Wall -c -o output/objects/array.o src/std/struc/c/array.c 

vector.o: src/std/struc/c/vector.c
	gcc -arch arm64 -Wall -c -o output/objects/vector.o src/std/struc/c/vector.c 

filter.o: src/std/struc/c/filter.c
	gcc -arch arm64 -Wall -c -o output/objects/filter.o src/std/struc/c/filter.c 

tree.o: src/std/struc/c/tree.c
	gcc -arch arm64 -Wall -c -o output/objects/tree.o src/std/struc/c/tree.c 

directory.o: src/std/sys/c/directory.c
	gcc -arch arm64 -Wall -c -o output/objects/directory.o src/std/sys/c/directory.c 

error.o: src/std/sys/c/error.c
	gcc -arch arm64 -Wall -c -o output/objects/error.o src/std/sys/c/error.c 

config.o: src/std/adv/c/config.c
	gcc -arch arm64 -Wall -c -o output/objects/config.o src/std/adv/c/config.c 

memory.o: src/std/c/memory.c
	gcc -arch arm64 -Wall -c -o output/objects/memory.o src/std/c/memory.c 

fileIO.o: src/std/c/fileIO.c
	gcc -arch arm64 -Wall -c -o output/objects/fileIO.o src/std/c/fileIO.c 

string.o: src/std/c/string.c
	gcc -arch arm64 -Wall -c -o output/objects/string.o src/std/c/string.c 

index.o: src/std/c/index.c
	gcc -arch arm64 -Wall -c -o output/objects/index.o src/std/c/index.c 

clean:
	rm output/objects/*o output/build/*
