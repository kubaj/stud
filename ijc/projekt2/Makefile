CC = gcc
CPP = g++
CFLAGS=-std=c99
CXXFLAGS=-std=c++11
CPPFLAGS=-Wall -Wextra -pedantic

HTABLE_OBJ = htable_clear.o htable_foreach.o htable_free.o \
	     htable_hash_function.o htable_init.o htable_init2.o \
	     htable_lookup_add.o htable_remove.o

HTABLE_OBJ_SHR = htable_clear_shr.o htable_foreach_shr.o htable_free_shr.o \
	     	 htable_hash_function_shr.o htable_init_shr.o htable_init2_shr.o \
	     	 htable_lookup_add_shr.o htable_remove_shr.o

LIBS = -lhtable
REMOVE = rm -rf
BIN = tail
BIN2 = tail2
BIN3 = wordcount
BIN4 = wordcount-dynamic

all: libhtable.a libhtable.so $(BIN) $(BIN2) $(BIN3) $(BIN4)

$(BIN): tail.c 
	$(CC) $(CFLAGS) $(CPPFLAGS) tail.c -o tail

$(BIN2): tail2.cc 
	$(CPP) $(CXXFLAGS) $(CPPFLAGS) tail2.cc -o tail2

$(BIN3): wordcount.o io.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -static wordcount.o io.o -o $(BIN3) -L. $(LIBS)

$(BIN4): wordcount.o io.o
	$(CC) $(CFLAGS) $(CPPFLAGS) wordcount.o io.o -o $(BIN4) -L. $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

%_shr.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -fPIC $< -o $@

libhtable.a: $(HTABLE_OBJ)
	ar -rcs libhtable.a $(HTABLE_OBJ)

libhtable.so: $(HTABLE_OBJ_SHR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -shared -fPIC -o libhtable.so $(HTABLE_OBJ_SHR)

clean: clear
clear:
	$(REMOVE) *.o *.a *.so $(BIN) $(BIN2) $(BIN3) $(BIN4)
