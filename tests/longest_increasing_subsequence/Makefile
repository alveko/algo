CXX	?= g++

CFLAGS	= -std=c++11 -c -Wall
INCL	= -I/usr/local/include -I../../..
LDFLAGS	= -L/usr/local/lib -lboost_program_options

EXE	= lis
SRC	= lis.cc
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
	rm -f $(EXE) *.o massif.out.* out_*

test: clean all
	@for tc in `find tcs -type f | sort`; do \
	    echo "\n***** ./$(EXE) < $$tc"; \
	    ./$(EXE) < $$tc; \
	done
