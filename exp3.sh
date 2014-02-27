#!/bin/bash

#       Runs experiment 2: execution time x size of the graph
#       @Arlei Silva

bash exp3_generate_graphs.sh
bash exp3_index_graphs.sh
bash exp3_run_st.sh
bash exp3_run_stbs.sh
bash exp3_run_stus.sh
