#!/bin/bash

#       Runs experiment 2: execution time x size of the graph
#       @Arlei Silva

bash exp2_generate_graphs.sh
bash exp2_index_graphs.sh
bash exp2_run_st.sh
bash exp2_run_stbs.sh
bash exp2_run_stus.sh
