/**
Copyright (c) 2013, Arlei Silva
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

@author: Arlei Silva (arleilps@gmail.com)
**/

/**
 *	FILE experiment_independence.cc: Main method.
**/

/*std includes*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/*my includes*/
#include "io.h"
#include "graph.h"
#include "graph_compression.h"

std::pair<double, double> exp_indep(const unsigned int center, 
	const unsigned int radius, Graph* graph)
{
	std::pair<double, double> values;
	values.first = 0;
	values.second = 0;
	std::list<unsigned int>* vertices_at_dist_r;
	unsigned int vertex;
	
	for(unsigned int r = 0; r <= radius; r++)
	{
		if(r < graph->max_distance(center))
		{
			vertices_at_dist_r = graph->vertices_at_distance
				(center, r);
	
			for(std::list<unsigned int>::iterator it = vertices_at_dist_r->begin();
				it != vertices_at_dist_r->end();++it)
			{
				vertex = *it;
				values.second += graph->count(vertex) * graph->value(vertex);
				values.first += graph->count(vertex) * graph->weight(vertex);
			}
		}
	}

	values.first = (double) values.first / graph->get_lambda();
	values.second = (double) values.second / graph->get_lambda();

	return values;
}

/**
 * Main method
 * @param argc size of the command line
 * @param argv string containing the command line
 * @return 0 if everything works, 1 otherwise
 * @throws
**/

int main(int argc, char** argv)
{
	
	if(Parameters::read(argc,argv))
	{
		/*Reading input graph with values*/
		Graph* graph = new Graph(Parameters::graph_file_name, Parameters::values_file_name, Parameters::directed);
		graph->read_partition_sizes(Parameters::partition_sizes_file_name,
			Parameters::max_radius);
		graph->set_biased_sample(Parameters::num_samples);
		graph->build_distance_str_slice_tree_sample(Parameters::max_radius);
		std::pair<double, double> values;
		std::ofstream output_file(Parameters::output_file_name.c_str());

		for(unsigned int v = 0; v < graph->size(); v++)
		{
			values = exp_indep(v, Parameters::max_radius, graph);
			output_file << values.first << "," << values.second << std::endl;
		}

		output_file.close();
	}


	return 0;
}


