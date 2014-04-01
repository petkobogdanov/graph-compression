CFLAGS = -Wall -O3 -g -pthread #-pg

CC = g++ $(CFLAGS)

RM = rm -f

GP_OBJS = main.o io.o graph.o graph_compression.o getopt_pp.o perf.o
EI_OBJS = experiment_independence.o io.o graph.o graph_compression.o getopt_pp.o perf.o

all: graph_compression experiment_independence

graph_compression: $(GP_OBJS)
	@echo ""
	@echo " --- graph_compression ---"
	@$(CC) $(GP_OBJS) -o graph_compression 
	@echo ""

experiment_independence: $(EI_OBJS)
	@echo ""
	@echo " --- experiment_independence ---"
	@$(CC) $(EI_OBJS) -o experiment_independence
	@echo ""

%.o: %.cc %.h
	@echo " --- COMPILING OBJECT \"$@\""
	@$(CC) $< -c 

clean:
	$(RM) graph_compression experiment_independence *.o
	clear

