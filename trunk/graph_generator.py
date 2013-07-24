"""
Copyright (c) 2013, Arlei Silva
All rights reserved.
      
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
	  
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	        
@author: Arlei Silva (arleilps@gmail.com)
"""

import sys
import getopt
import math
import random
import networkx
from collections import deque

class Graph(object):
    def __init__(self, num_vertices, num_edges, num_partitions, radius, deviation, max_average):
	G =  networkx.barabasi_albert_graph(num_vertices, num_edges)
        self.edges = []
	self.values = []
	self.partition_assignments = []
	self.partitions = []
	self.centers = {}
        
	for i in range(0, num_vertices):
	    self.edges.append([])
	    self.values.append(0)
	    self.partition_assignments.append(0)

	for e in G.edges(data=True):
	    self.edges[e[0]].append(e[1])
	    self.edges[e[1]].append(e[0])
        
	partition = {}
	partition["average"] = 0
	partition["center"] = -1
	
	self.partitions.append(partition)
         
	for p in range(1, num_partitions):
	    partition = self.create_new_partition(p, radius, deviation, max_average)
	    self.partitions.append(partition)

	self.sse = self.compute_sse()
	self.sse_data = self.compute_sse_data()
    
    def write_values(self, output_file_name):
        output_file = open(output_file_name, 'w')
        for v in range(0, len(self.edges)):
	    #output_file.write(str(v)+",0,"+str(self.values[v])+"\n")
	    output_file.write(str(v)+","+str(self.values[v])+"\n")
	     
	output_file.close() 
    
    def write_graph(self, output_file_name):
        output_file = open(output_file_name, 'w')
        for v in range(0, len(self.edges)):
	    for u in self.edges[v]:
	        if v > u:
		    output_file.write(str(v)+","+str(u)+"\n")
	     
	output_file.close() 

    def write_statistics(self, output_file_name):
        output_file = open(output_file_name, 'w')
	output_file.write("sse_data = "+str(self.sse_data)+"\n")
	output_file.write("sse_optimal_partitioning = "+str(self.sse)+"\n")
	output_file.write("optimal_sse_reduction = "+str(float(self.sse_data-self.sse)/self.sse_data)+"\n")
        output_file.write("partitions:\n")
	
	for p in range(0, len(self.partitions)):
	    output_file.write(str(p)+","+str(self.partitions[p]["center"])+","+str(self.partitions[p]["average"])+","+str(self.partitions[p]["sse"])+"\n")
        
        output_file.write("assignments:\n")

	for v in range(0, len(self.partition_assignments)):
	    output_file.write(str(v)+","+str(self.partition_assignments[v])+"\n")
	
	output_file.close() 

    def create_new_partition(self, partition_id, radius, deviation, max_average):
        partition = {}
	partition["average"] = random.uniform(0, max_average)
	partition["center"] = random.randint(0, len(self.edges)-1)

	while partition["center"] in self.centers or self.partition_assignments[partition["center"]] != 0:
	    partition["center"] = random.randint(0, len(self.edges)-1)

	self.set_values_partition(partition_id, partition["center"], partition["average"], deviation, radius)

        return partition
    
    def compute_sse(self):
        sse = 0
	for p in range(0, len(self.partitions)):
	    average = 0
	    n = 0

	    for v in range(0, len(self.partition_assignments)):
	        if self.partition_assignments[v] == p:
		    average = average + self.values[v]
                    n = n + 1
	    
	    if n > 0:
                average = float(average) / n
            
	    sse_partition = 0

	    for v in range(0, len(self.partition_assignments)):
	        if self.partition_assignments[v] == p:
		    sse_partition = sse_partition + math.pow(average - self.values[v], 2)
            
	    self.partitions[p]["sse"] = sse_partition
	    sse = sse + sse_partition

        return sse

    def compute_sse_data(self):
        sse = 0
	average = 0

	for v in self.values:
	    average = average + v

	average = float(average) / len(self.values)

	for v in self.values:
            sse = sse + math.pow(average - v, 2)

	return sse

    def set_values_partition(self, partition_id, center, average, deviation, radius):
        distances = {}

	q = deque([center])
	distances[center] = 0
	self.values[center] = random.gauss(average, deviation)
	self.partition_assignments[center] = partition_id

	while len(q) > 0:
	    u = q.popleft()
	    
	    for z in self.edges[u]:
	        if z not in distances or distances[z] > distances[u] + 1:
		    if distances[u] + 1 <= radius:
                        distances[z] = distances[u] + 1
			q.append(z)
	                self.values[z] = random.gauss(average, deviation)
	                self.partition_assignments[z] = partition_id

class Usage(Exception):
    def __init__(self, msg):
        self.msg = msg
	    
def main(argv=None):
    if argv is None:
        argv = sys.argv

    #	Parameters:
    #		- output file name	o
    #		- number of vertices v
    #		- number of edges new vertex e (preferential attachment)
    #		- number of partitions	p
    #		- partition radius	r
    #		- standart deviation values partition d
    #		- max partition average	m
    #

    try:
        try:
            opts, input_files = getopt.getopt(argv[1:], "o:v:e:p:r:d:m:h", ["output=","num-vertices=","num-edges=","num-partitions=","radius=","deviation=","max-average=","help"])
        except getopt.error, msg:
            raise Usage(msg)
 
        output_file_name = ""
	num_vertices = 0
	num_edges = 0
	num_partitions = 0
	radius = 0
	deviation = 0
	max_average = 0

        for opt,arg in opts:
	    if opt in ('-o', '--output'):
	        output_file_name = arg
	    if opt in ('-v', '--num-vertices'):
	        num_vertices = int(arg)
	    if opt in ('-e', '--num-edges'):
	        num_edges = int(arg)
	    if opt in ('-p', '--num-partitions'):
	        num_partitions = int(arg)
	    if opt in ('-r', '--radius'):
	        radius = int(arg)
	    if opt in ('-d', '--deviation'):
	        deviation = float(arg)
	    if opt in ('-m', '--max-average'):
	        max_average = float(arg)
	    if opt in ('-h', '--help'):
	        print "python graph_generator.py [-o <output_file>] [-v <num-vertices>] [-e <num-edges>] [-p <num-partitions] [-r <radius>] [-d <deviation>] [-m <max-average>]"
	        sys.exit()
        
        g = Graph(num_vertices, num_edges, num_partitions, radius, deviation, max_average)

	g.write_values(output_file_name + ".data")
	g.write_graph(output_file_name + ".graph")
	g.write_statistics(output_file_name + ".stats")

    except Usage, err:
        print >>sys.stderr, err.msg
        print >>sys.stderr, "for help use --help"
        return 2

if __name__ == "__main__":
    sys.exit(main())
