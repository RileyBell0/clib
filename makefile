IDIR=/src/h
BUILD=gcc -Wall -c -o
BSRC=src/std/
SRC=src/
OUTDIR= build/
CFLAGS= -g -lm

# array
# bintree
# general
# fileIO
# list
# string

all: build

build: array.o bintree.o general.o fileIO.o list.o string.o window.o
	
array.o: $(BSRC)array/array.c $(BSRC)array/array.h
	$(BUILD) $(OUTDIR)array.o $(BSRC)array/array.c $(CFLAGS)

bintree.o: $(BSRC)bintree/bintree.c $(BSRC)bintree/bintree.h
	$(BUILD) $(OUTDIR)bintree.o $(BSRC)bintree/bintree.c $(CFLAGS)
	
general.o: $(BSRC)general/general.c $(BSRC)general/general.h
	$(BUILD) $(OUTDIR)general.o $(BSRC)general/general.c $(CFLAGS)

fileIO.o: $(BSRC)fileIO/fileIO.c $(BSRC)fileIO/fileIO.h
	$(BUILD) $(OUTDIR)fileIO.o $(BSRC)fileIO/fileIO.c $(CFLAGS)

list.o: $(BSRC)list/list.c $(BSRC)list/list.h
	$(BUILD) $(OUTDIR)list.o $(BSRC)list/list.c $(CFLAGS)
	
string.o: $(BSRC)string/string.c $(BSRC)string/string.h
	$(BUILD) $(OUTDIR)string.o $(BSRC)string/string.c $(CFLAGS)

window.o: $(BSRC)window/window.c $(BSRC)window/window.h
	$(BUILD) $(OUTDIR)window.o $(BSRC)window/window.c $(CFLAGS)


ifeq ("x","y")

Sample Setup

REPLACE.o: $(BSRC)REPLACE/REPLACE.c $(BSRC)REPLACE/REPLACE.h
	$(BUILD) $(OUTDIR)REPLACE.o $(BSRC)REPLACE/REPLACE.c $(CFLAGS)

endif
