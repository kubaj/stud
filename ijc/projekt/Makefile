CC = gcc
CFLAGS = -std=c99 -O2 -pedantic -Wall -Wextra 
#CFLAGS = -std=c99 -pedantic -Wall -Wextra 
CFLAGS2 = -DUSE_INLINE

LIBS = -lm
REMOVE = rm -rf
BIN = steg-decode
BIN2 = primes
BIN3 = primes-inline

all: $(BIN) $(BIN2) $(BIN3)

$(BIN): steg-decode.o ppm.o error.o eratosthenes.o
	$(CC) steg-decode.o ppm.o error.o eratosthenes.o -o $(BIN) $(LIBS)

$(BIN2): primes.o error.o eratosthenes.o
	$(CC) primes.o error.o eratosthenes.o -o $(BIN2) $(LIBS)

$(BIN3): primes.o error.o eratosthenes_inline.o
	$(CC) primes.o error.o eratosthenes_inline.o -o $(BIN3) $(LIBS)

primes.o: primes.c bit_array.h
	$(CC) -c primes.c -o primes.o $(CFLAGS)

eratosthenes.o: eratosthenes.c bit_array.h
	$(CC) -c eratosthenes.c -o eratosthenes.o $(CFLAGS)

eratosthenes_inline.o: eratosthenes.c bit_array.h
	$(CC) -c eratosthenes.c -o eratosthenes_inline.o $(CFLAGS) $(CFLAGS2)

error.o: error.c error.h
	$(CC) -c error.c -o error.o $(CFLAGS)

steg-decode.o: steg-decode.c bit_array.h
	$(CC) -c steg-decode.c -o steg-decode.o $(CFLAGS)

ppm.o: 	ppm.c ppm.h
	$(CC) -c ppm.c -o ppm.o $(CFLAGS)

clean: clear
clear:
	$(REMOVE) *.o $(BIN) $(BIN2) $(BIN3)
