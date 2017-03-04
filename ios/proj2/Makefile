CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra  -pedantic
LFLAGS=-lpthread -lrt

OBJ=proj2_sharedres.o proj2_passenger.o proj2_car.o proj2.o
BIN=proj2

all: $(BIN) 

proj2: $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

proj2.o: proj2.c
proj2_sharedres.o: proj2_sharedres.h proj2_sharedres.c

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean: clear

clear:
	rm -f *.o proj2 
