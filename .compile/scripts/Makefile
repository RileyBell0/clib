all: projectMake projectScan 

projectMake: projectMake.o dict.o list.o array.o vector.o filter.o tree.o directory.o error.o config.o memory.o fileIO.o string.o index.o
	clang -arch arm64 -Wall -O3 -o .compile/prog/make .compile/output/projectMake.o .compile/output/dict.o .compile/output/list.o .compile/output/array.o .compile/output/vector.o .compile/output/filter.o .compile/output/tree.o .compile/output/directory.o .compile/output/error.o .compile/output/config.o .compile/output/memory.o .compile/output/fileIO.o .compile/output/string.o .compile/output/index.o 

projectScan: projectScan.o dict.o list.o array.o vector.o filter.o tree.o directory.o error.o config.o memory.o fileIO.o string.o index.o
	clang -arch arm64 -Wall -O3 -o .compile/prog/scan .compile/output/projectScan.o .compile/output/dict.o .compile/output/list.o .compile/output/array.o .compile/output/vector.o .compile/output/filter.o .compile/output/tree.o .compile/output/directory.o .compile/output/error.o .compile/output/config.o .compile/output/memory.o .compile/output/fileIO.o .compile/output/string.o .compile/output/index.o 

projectMake.o: .compile/src/main/projectMake.c
	clang -arch arm64 -Wall -O3 -c -o .compile/output/projectMake.o .compile/src/main/projectMake.c 

projectScan.o: .compile/src/main/projectScan.c
	clang -arch arm64 -Wall -O3 -c -o .compile/output/projectScan.o .compile/src/main/projectScan.c 

dict.o: .compile/src/std/struc/c/dict.c
	clang -arch arm64 -Wall -O3 -c -o .compile/output/dict.o .compile/src/std/struc/c/dict.c 

list.o: .compile/src/std/struc/c/list.c
	clang -arch arm64 -Wall -O3 -c -o .compile/output/list.o .compile/src/std/struc/c/list.c 

array.o: .compile/src/std/struc/c/array.c
	clang -arch arm64 -Wall -O3 -c -o .compile/output/array.o .compile/src/std/struc/c/array.c 

vector.o: .compile/src/std/struc/c/vector.c
	clang -arch arm64 -Wall -O3 -c -o .compile/output/vector.o .compile/src/std/struc/c/vector.c 

filter.o: .compile/src/std/struc/c/filter.c
	clang -arch arm64 -Wall -O3 -c -o .compile/output/filter.o .compile/src/std/struc/c/filter.c 

tree.o: .compile/src/std/struc/c/tree.c
	clang -arch arm64 -Wall -O3 -c -o .compile/output/tree.o .compile/src/std/struc/c/tree.c 

directory.o: .compile/src/std/sys/c/directory.c
	clang -arch arm64 -Wall -O3 -c -o .compile/output/directory.o .compile/src/std/sys/c/directory.c 

error.o: .compile/src/std/sys/c/error.c
	clang -arch arm64 -Wall -O3 -c -o .compile/output/error.o .compile/src/std/sys/c/error.c 

config.o: .compile/src/std/adv/c/config.c
	clang -arch arm64 -Wall -O3 -c -o .compile/output/config.o .compile/src/std/adv/c/config.c 

memory.o: .compile/src/std/c/memory.c
	clang -arch arm64 -Wall -O3 -c -o .compile/output/memory.o .compile/src/std/c/memory.c 

fileIO.o: .compile/src/std/c/fileIO.c
	clang -arch arm64 -Wall -O3 -c -o .compile/output/fileIO.o .compile/src/std/c/fileIO.c 

string.o: .compile/src/std/c/string.c
	clang -arch arm64 -Wall -O3 -c -o .compile/output/string.o .compile/src/std/c/string.c 

index.o: .compile/src/std/c/index.c
	clang -arch arm64 -Wall -O3 -c -o .compile/output/index.o .compile/src/std/c/index.c 

clean:
	rm .compile/output/*o .compile/prog/*
