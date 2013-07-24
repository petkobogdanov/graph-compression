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
 *	FILE main.cc: Main method.
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

/**
 * Main method
 * @param argc size of the command line
 * @param argv string containing the command line
 * @return 0 if everything works, 1 otherwise
 * @throws
**/

int main(int argc, char** argv)
{
	
	/*Setting the compression algorithms*/
	std::vector<std::string> compression_algorithms;
	compression_algorithms.push_back("ST");	//Slice Tree
	compression_algorithms.push_back("AL");	//Average Linkage
	compression_algorithms.push_back("WV");	//Wavelets
	Parameters::set_compression_algorithms(compression_algorithms);
	
	double sse;
	double sse_reduction;
	double compression_rate;
	double compression_time;
	double distance_str_time;
	unsigned int num_partitions;
	unsigned int budget;
	double normalized_sse;
	double root_mean_squared_error;
	double maximum_pointwise_error;
	double peak_signal_to_noise_ratio;
	ExecTime* exec_time_distance_str = new ExecTime();
	GraphCompressionAlgorithm* alg;
	
	/*Reading the input parameters*/
	if(Parameters::read(argc,argv))
	{
		/*Reading input graph with values*/
		Graph* graph = new Graph(Parameters::graph_file_name, Parameters::values_file_name);

		if(Parameters::num_samples > 0 && Parameters::num_samples < graph->size())
		{
			graph->set_sample(Parameters::num_samples);
		}
	
		/*Performing GraphCompression*/
		if(Parameters::compression_algorithm == "ST")
		{
			if(Parameters::num_samples > 0 && Parameters::num_samples < graph->size())
			{
				/*Slice tree with sampling*/
				exec_time_distance_str->start();
				/*Slice tree requires the distance structure*/
				graph->build_distance_str_slice_tree_sample();
				exec_time_distance_str->stop();
				distance_str_time = exec_time_distance_str->get_seconds();
			}
			else
			{
				/*Slice tree without sampling*/
				exec_time_distance_str->start();
				/*Slice tree requires the distance structure*/
				graph->build_distance_str_slice_tree();
				exec_time_distance_str->stop();
				distance_str_time = exec_time_distance_str->get_seconds();
			}
			
			if(Parameters::budget.size())
			{
				alg = new SliceTree(*graph); 
				GraphCompression::compress(*graph, *alg, Parameters::budget[0], 
					Parameters::output_file_name);
			}
			else
			{
				unsigned int budget_from_num_partitions = 
					SliceTree::budget(Parameters::num_partitions[0], 
					*graph);
				
				alg = new SliceTree(*graph); 
				GraphCompression::compress(*graph, *alg, budget_from_num_partitions, 
					Parameters::output_file_name);
			}
		}
		
		if(Parameters::compression_algorithm == "AL")
		{
			exec_time_distance_str->start();
			/*Average linkage requires the distance matrix*/
			graph->build_distance_matrix();
			exec_time_distance_str->stop();
			distance_str_time = exec_time_distance_str->get_seconds();
			
			if(Parameters::budget.size())
			{
				alg = new AverageLinkage(*graph);
				GraphCompression::compress(*graph, *alg, Parameters::budget[0], 
					Parameters::output_file_name);
			}
			else
			{
				unsigned int budget_from_num_partitions = 
					SliceTree::budget(Parameters::num_partitions[0], 
					*graph);
				
				alg = new AverageLinkage(*graph);
				GraphCompression::compress(*graph, *alg, budget_from_num_partitions, 
					Parameters::output_file_name);
			}
		}
		
		if(Parameters::compression_algorithm == "WV")
		{
			exec_time_distance_str->start();
			/*Wavelets requires a sorted vector*/
			//graph->build_bfs_vector();
			graph->build_priority_first_vector();
			exec_time_distance_str->stop();
			distance_str_time = exec_time_distance_str->get_seconds();
			
			if(Parameters::budget.size())
			{
				alg = new Wavelets(*graph);
				GraphCompression::compress(*graph, *alg, Parameters::budget[0], 
					Parameters::output_file_name);
			}
			else
			{
				/*We need the diameter of the graph to measure the
				* number of bytes necessary to represent a partition
				* in slice tree*/
				graph->build_distance_matrix();
				
				unsigned int budget_from_num_partitions = 
					SliceTree::budget(Parameters::num_partitions[0], 
					*graph);
				alg = new Wavelets(*graph);
				GraphCompression::compress(*graph, *alg, budget_from_num_partitions, 
					Parameters::output_file_name);
			}
		}

		sse = GraphCompression::sse();
		sse_reduction = GraphCompression::sse_reduction();
		compression_rate = GraphCompression::compression_rate();
		compression_time = GraphCompression::compression_time();
		num_partitions = GraphCompression::num_partitions();
		budget = GraphCompression::budget();
		normalized_sse = GraphCompression::normalized_sse();
		root_mean_squared_error = 
			GraphCompression::root_mean_squared_error();
		maximum_pointwise_error = 
			GraphCompression::maximum_pointwise_error();
		peak_signal_to_noise_ratio = 
			GraphCompression::peak_signal_to_noise_ratio();
		
		/*Statistics printed as output*/
		std::cout << "budget = " << budget << std::endl;
		std::cout << "sse = " << sse << std::endl;
		std::cout << "sse_reduction = " <<  sse_reduction << std::endl;
		std::cout << "normalized_sse = " << normalized_sse << std::endl; 
		std::cout << "root_mean_squared_error = " 
			<< root_mean_squared_error << std::endl; 
		std::cout << "maximum_pointwise_error = " 
			<< maximum_pointwise_error << std::endl; 
		std::cout << "peak_signal_to_noise_ratio = "
			<< peak_signal_to_noise_ratio << std::endl; 
		std::cout << "compression_rate = " << compression_rate << std::endl;
		std::cout << "compression_time = " << compression_time << std::endl;
		std::cout << "distance_computation_time = " << distance_str_time << std::endl;
		
		delete graph;
		delete alg;
	}

	delete exec_time_distance_str;

	return 0;
}

