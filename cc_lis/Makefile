CXX	= g++
CF_REL	= -c -Wall -O3
CF_DBG	= -c -Wall -g -DDEBUG
CFLAGS	= -std=c++11 $(CF_REL) -I/usr/local/include -I../../
LDFLAGS	= -L/usr/local/lib -lboost_program_options

SRC	= lis.cc
OBJ	= $(SRC:.cc=.o)
EXE	= lis

.PHONY: all clean test
all:	$(SRC) $(EXE)
debug:	CFLAGS = $(CF_DBG)
debug:	all

$(EXE): $(OBJ) 
	$(CXX) $(OBJ) -o $@ $(LDFLAGS) 

.cc.o:
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXE) $(OBJ) *.o massif.out.* out_*

test: clean all
	@for tc in `find tcs -type f | sort`; do \
	    echo "\n***** ./$(EXE) < $$tc"; \
	    ./$(EXE) < $$tc; \
	done
