CXX	= g++
CF_REL	= -c -Wall -O3
CF_DBG	= -c -Wall -g -DDEBUG
CFLAGS	= $(CF_REL) -I/usr/local/include
LDFLAGS	= -L/usr/local/lib -lboost_program_options

SRC	= \
	rmq.cc \
	rmq_main.cc \
	rmq_naive.cc \
	rmq_sparse_table.cc \
	rmq_segment_tree.cc

OBJ	= $(SRC:.cc=.o)
EXE	= rmq

all:	$(SRC) $(EXE)

debug:	CFLAGS = $(CF_DBG)
debug:	all

$(EXE): $(OBJ) 
	$(CXX) $(OBJ) -o $@ $(LDFLAGS) 

.cc.o:
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXE) *.o massif.out.* out_*

memtest: all
	@rm -f massif.out.*
	@valgrind --tool=massif ./$(EXE) >/dev/null 2>&1
	@ms_print massif.out.* | egrep '^ *([0-9,]+ +){3}|total' | sort -u -k 3 | tail -2 | sort
	@rm -f massif.out.*
