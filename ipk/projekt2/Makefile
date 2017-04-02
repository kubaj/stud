CC = g++

# define any compile-time flags
CFLAGS = -Wall -g -pthread -std=c++11

LIBS = -pthread

# define the C source files
SRCS = FileController.cpp Http.cpp IPKUtils.cpp
CSRC = client.cpp
SSRC = server.cpp

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.cpp=.o)
COBJS = $(CSRC:.cpp=.o)
SOBJS = $(SSRC:.cpp=.o)

# define the executable file 
SERVER = ftrestd
CLIENT = ftrest

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:    $(SERVER) $(CLIENT)

$(SERVER): $(OBJS) $(SOBJS)
	$(CC) $(CFLAGS) -o $(SERVER) $(SOBJS) $(OBJS) $(LFLAGS) $(LIBS)

$(CLIENT): $(OBJS) $(COBJS)
	$(CC) $(CFLAGS) -o $(CLIENT) $(COBJS) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CC) $(CFLAGS) -c $<  -o $@

clean:
	$(RM) *.o *~ $(SERVER) $(CLIENT) 

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
