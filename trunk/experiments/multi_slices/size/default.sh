#!/bin/bash

#       Default parameters for experiments
#       @Arlei Silva

num_graphs=10
num_runs_sampling=1
max_radius=2
num_edges=5
num_partitions_data=16
num_partitions_alg=16
num_vertices=20000
sse=40000
sse_reduction=20000
graph_generator='../../../graph_generator.py'
graph_compression='../../../graph_compression'
graph_name_prefix='syn'
param_sizes=(5000 10000 15000 20000)
delta_fast_sampling=0.4
delta_slow_sampling=0.1
rho_fast_sampling=0.1
rho_slow_sampling=0.6
rate_fast_sampling=0.01
rate_slow_sampling=0.1
num_threads=1
