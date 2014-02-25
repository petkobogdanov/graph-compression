#!/bin/bash

#       Runs experiment 1: execution time x number of samples
#       @Arlei Silva

bash exp1_generate_graphs.sh
bash exp1_index_graphs.sh
bash exp1_run_st.sh
bash exp1_run_stbs.sh
bash exp1_run_stus.sh
