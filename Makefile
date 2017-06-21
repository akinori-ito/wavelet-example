CC=gcc
CFLAGS=-Wall -g -O3
EXT= #.exe

all: calc-error$(EXT) addnoise$(EXT) wavdiff$(EXT) wavelet-sample$(EXT)
clean:
	-rm -rf calc-error$(EXT) addnoise$(EXT) wavdiff$(EXT) wavelet-sample$(EXT) *.o *~

calc-error$(EXT): calc-error.o wavio.o
	$(CC) $(CFLAGS) -o calc-error calc-error.o wavio.o -lm

addnoise$(EXT): addnoise.o wavio.o
	$(CC) $(CFLAGS) -o addnoise addnoise.o wavio.o 

wavdiff$(EXT): wavdiff.o wavio.o
	$(CC) $(CFLAGS) -o wavdiff wavdiff.o wavio.o

wavelet-sample$(EXT): wavelet-sample.o wavio.o wavelet.o
	$(CC) $(CFLAGS) -o wavelet-sample wavelet-sample.o wavio.o wavelet.o

calc-error.o: calc-error.c wavio.h
addnoise.o: addnoise.c wavio.h
wavdiff.o: wavdiff.c wavio.h
wavelet-sample.o: wavelet-sample.c wavio.h wavelet.h
wavio.o: wavio.c wavio.h
wavelet.o: wavelet.c wavelet.h
