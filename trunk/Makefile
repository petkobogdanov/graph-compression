CC = g++

CFLAGS = -Wall -O3 -g #-pg

RM = rm -f

OBJS = main.o io.o graph.o graph_compression.o getopt_pp.o perf.o

all: graph_compression

graph_compression: $(OBJS)
	@echo ""
	@echo " --- graph_compression ---"
	@$(CC) $(CFLAGS) $(OBJS) -o graph_compression 
	@echo ""

%.o: %.cc %.h
	@echo " --- COMPILING OBJECT \"$@\""
	@$(CC) $(CFLAGS) $< -c 

clean:
	$(RM) graph_compression *.o
	clear

