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
	compression_algorithms.push_back("ST");	  //Standard Slice Tree
	compression_algorithms.push_back("STUS"); //Slice Tree with Uniform Sampling
	compression_algorithms.push_back("STBS"); //Slice Tree with Biased Sampling
	compression_algorithms.push_back("AL");	  //Average Linkage
	compression_algorithms.push_back("WV");	  //Wavelets
	
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
		
		if(Parameters::compression_algorithm == "")
		{
			graph->pre_compute_partition_sizes(Parameters::num_threads, 
				Parameters::partition_sizes_file_name);
		}
		else
		{
			graph->read_partition_sizes(Parameters::partition_sizes_file_name);
		}

		/*Performing GraphCompression*/
		if(Parameters::compression_algorithm == "ST")
		{
			/*Standard slice tree*/
			exec_time_distance_str->start();
			/*Slice tree requires the distance structure*/
			graph->build_distance_str_slice_tree();
			exec_time_distance_str->stop();
			distance_str_time = exec_time_distance_str->get_seconds();
			
			if(Parameters::budget > 0)
			{
				alg = new SliceTree(*graph); 
				GraphCompression::compress(*graph, *alg, Parameters::budget, 
					Parameters::output_file_name);
			}
			else
			{
				unsigned int budget_from_num_partitions = 
					SliceTree::budget(Parameters::num_partitions, 
					*graph);
				
				alg = new SliceTree(*graph); 
				GraphCompression::compress(*graph, *alg, budget_from_num_partitions, 
					Parameters::output_file_name);
			}
		}
		
		if(Parameters::compression_algorithm == "STUS")
		{
			/*Slice tree with uniform sampling*/
			graph->set_sample(Parameters::num_samples);
			
			exec_time_distance_str->start();
			/*Slice tree requires the distance structure*/
			graph->build_distance_str_slice_tree_sample();
			exec_time_distance_str->stop();
			distance_str_time = exec_time_distance_str->get_seconds();

			if(Parameters::budget > 0)
			{
				alg = new SliceTreeUnifSamp(*graph, Parameters::delta,
					Parameters::rho, Parameters::num_samples); 
				GraphCompression::compress(*graph, *alg, Parameters::budget, 
					Parameters::output_file_name);
			}
			else
			{
				unsigned int budget_from_num_partitions = 
					SliceTree::budget(Parameters::num_partitions, 
					*graph);
				
				alg = new SliceTreeUnifSamp(*graph, Parameters::delta, 
					Parameters::rho, Parameters::num_samples); 
				GraphCompression::compress(*graph, *alg, budget_from_num_partitions, 
					Parameters::output_file_name);
			}
		}
		
		if(Parameters::compression_algorithm == "STBS")
		{
			/*Slice tree with biased sampling*/
			graph->set_biased_sample(Parameters::num_samples);
			
			exec_time_distance_str->start();
			/*Slice tree requires the distance structure*/
			graph->build_distance_str_slice_tree();
			exec_time_distance_str->stop();
			distance_str_time = exec_time_distance_str->get_seconds();

			if(Parameters::budget > 0)
			{
				alg = new SliceTreeBiasSamp(*graph, Parameters::delta,
					Parameters::rho, Parameters::num_samples); 
				GraphCompression::compress(*graph, *alg, Parameters::budget, 
					Parameters::output_file_name);
			}
			else
			{
				unsigned int budget_from_num_partitions = 
					SliceTree::budget(Parameters::num_partitions, 
					*graph);
				
				alg = new SliceTreeBiasSamp(*graph, Parameters::delta, 
					Parameters::rho, Parameters::num_samples); 
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
			
			if(Parameters::budget > 0)
			{
				alg = new AverageLinkage(*graph);
				GraphCompression::compress(*graph, *alg, Parameters::budget, 
					Parameters::output_file_name);
			}
			else
			{
				unsigned int budget_from_num_partitions = 
					SliceTree::budget(Parameters::num_partitions, 
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
			
			if(Parameters::budget > 0)
			{
				alg = new Wavelets(*graph);
				GraphCompression::compress(*graph, *alg, Parameters::budget, 
					Parameters::output_file_name);
			}
			else
			{
				graph->build_distance_matrix();
				
				unsigned int budget_from_num_partitions = 
					SliceTree::budget(Parameters::num_partitions, 
					*graph);
				alg = new Wavelets(*graph);
				GraphCompression::compress(*graph, *alg, budget_from_num_partitions, 
					Parameters::output_file_name);
			}
		}

		if(Parameters::compression_algorithm != "")
		{
			sse = GraphCompression::sse();
			sse_reduction = GraphCompression::sse_reduction();
			compression_rate = GraphCompression::compression_rate();
			compression_time = GraphCompression::compression_time();
			num_partitions = GraphCompression::num_partitions();
			budget = GraphCompression::budget();
		
			/*Statistics printed as output*/
			std::cout << "budget = " << budget << std::endl;
			std::cout << "sse = " << sse << std::endl;
			std::cout << "sse_reduction = " <<  sse_reduction << std::endl;
			std::cout << "compression_rate = " << compression_rate << std::endl;
			std::cout << "compression_time = " << compression_time << std::endl;
			std::cout << "num_partitions = " << num_partitions << std::endl;
			
			delete alg;
		}

		delete graph;
	}

	delete exec_time_distance_str;

	return 0;
}


