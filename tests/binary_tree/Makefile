CXX	?= g++

CFLAGS	= -std=c++11 -c -Wall
INCL	= -I/usr/local/include -I../../..
LDFLAGS	= -L/usr/local/lib -lboost_program_options

EXE	= binary_tree
SRC	= binary_tree.cc
OBJ	= $(SRC:.cc=.o)

.PHONY: all clean

all:	CFLAGS += -O3
all:	$(EXE)

debug:	CFLAGS += -g -DDEBUG
debug:	$(EXE)

$(EXE): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

.cc.o:
	$(CXX) $(CFLAGS) $(INCL) $< -o $@

clean:
	rm -f $(EXE) *.o
