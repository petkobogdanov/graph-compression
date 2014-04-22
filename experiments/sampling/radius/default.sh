#!/bin/bash

#       Default parameters for experiments
#       @Arlei Silva

num_graphs=10
num_runs_sampling=10
max_radius=3
num_edges=5
num_partitions_data=32
num_partitions_alg=2
num_vertices=100000
sse=200000
sse_reduction=100000
graph_generator='../../../graph_generator.py'
graph_compression='../../../graph_compression'
graph_name_prefix='syn'
param_radius=(1 2 3)
delta_fast_sampling=0.4
delta_slow_sampling=0.1
rho_fast_sampling=0.1
rho_slow_sampling=0.6
rate_fast_sampling=0.01
rate_slow_sampling=0.1
num_threads=1
radius_time='radius_time'
radius_pruning='radius_pruning'
radius_approximation='radius_approximation'

