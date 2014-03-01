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
 *	Implementation of a graph
**/
/*std includes*/
#include <iostream>
#include <fstream>
#include <algorithm>
#include <climits>
#include <deque>
#include <list>
#include <queue>
#include <cmath>
#include <pthread.h>
#include <stdio.h>

/*my includes*/
#include "graph.h"

/**
 * Graph constructor
 * @param  graph_file_name input graph file name
 * @param  value_file_name input values file name
 * @return 
 * @throws 
**/
Graph::Graph(const std::string& graph_file_name, const std::string& values_file_name) throw (std::ios_base::failure)
{
	read_graph(graph_file_name, values_file_name);
	graph_diameter = USHRT_MAX;
	distance_matrix = NULL;
	biased_sampling = false;
	uniform_sampling = false;
	num_samples = 0;
	sum_values = 0;
	sum_weights = 0;
}

/**
 * Graph constructor
 * @param  graph_file_name input graph file name
 * @param  value_file_name input values file name
 * @return 
 * @throws 
**/
Graph::Graph(const std::string& graph_file_name) throw (std::ios_base::failure)
{
	read_graph(graph_file_name);
	graph_diameter = USHRT_MAX;
	distance_matrix = NULL;
	biased_sampling = false;
	uniform_sampling = false;
	num_samples = 0;
	sum_values = 0;
	sum_weights = 0;
}

/**
 * Splits a string using a delimiter
 * @param s string
 * @param delim delimiter
 * @return vector with substrings
 * @throws 
**/
const std::vector<std::string> split(const std::string &s, char delim)
{
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
      
	while(std::getline(ss, item, delim))
	{
        	elems.push_back(item);
	}
	   
	return elems;
}

/**
 * Reads the graph data
 * @param graph_file_name input file with edges
 * @param values_file_name input file with vertex values
 * @return
 * @throws
**/
void Graph::read_graph(const std::string& graph_file_name, const std::string& values_file_name) throw (std::ios_base::failure)
{
	std::ifstream input_values_file(values_file_name.c_str(), std::ios::in);
	std::string line_str;
	std::vector< std:: string > line_vec;
	std::map<std::string,unsigned int> vertex_ids;

	if(! input_values_file.is_open())
	{
		std::cerr << "Error: Could not open values file " << values_file_name << std::endl << std::endl;
		return;
	}
	
	/*Reading values from the graph*/
	std::getline(input_values_file, line_str);
	std::string vertex_name;
	double vertex_value;
	unsigned int ID = 0;
	
	while(! input_values_file.eof())
	{
		line_vec = split(line_str,',');
		vertex_name = line_vec[0];
		vertex_value = atof(line_vec[1].c_str());
		vertex_ids[vertex_name] = ID;
		vertex_values[ID] = vertex_value;
		vertex_names[ID] = vertex_name;

		std::getline(input_values_file, line_str);
		ID = ID + 1;
	}
	
	input_values_file.close();
	
	adjacency_list.reserve(vertex_ids.size());

	for(unsigned int v = 0; v < vertex_ids.size(); v++)
	{
		adjacency_list.push_back(new std::list<unsigned int>);
	}

	std::ifstream input_graph_file(graph_file_name.c_str(), std::ios::in);

	if(! input_graph_file.is_open())
	{
		std::cerr << "Error: Could not open graph file " << graph_file_name << std::endl << std::endl;
		return;
	}
	
	/*Reading edges from the graph*/
	std::getline(input_graph_file, line_str);
	std::string vertex_one;
	std::string vertex_two;

	while(! input_graph_file.eof())
	{
		line_vec = split(line_str,',');
		vertex_one = line_vec[0];
		vertex_two = line_vec[1];

		adjacency_list[vertex_ids[vertex_one]]->push_back(vertex_ids[vertex_two]);
		adjacency_list[vertex_ids[vertex_two]]->push_back(vertex_ids[vertex_one]);

		std::getline(input_graph_file, line_str);
	}
	
	input_graph_file.close();
}

/**
 * Reads the graph data
 * @param graph_file_name input file with edges
 * @return
 * @throws ios_base::failure in case can't read the input files
*/
void Graph::read_graph(const std::string& graph_file_name) throw (std::ios_base::failure)
{
	std::string line_str;
	std::vector< std:: string > line_vec;
	std::map<std::string,unsigned int> vertex_ids;

	std::ifstream input_graph_file(graph_file_name.c_str(), std::ios::in);

	if(! input_graph_file.is_open())
	{
		std::cerr << "Error: Could not open graph file " << graph_file_name << std::endl << std::endl;
		return;
	}
	
	/*Reading edges from the graph*/
	std::getline(input_graph_file, line_str);
	std::string vertex_one;
	std::string vertex_two;
	unsigned int ID = 0;

	while(! input_graph_file.eof())
	{
		line_vec = split(line_str,',');
		vertex_one = line_vec[0];
		vertex_two = line_vec[1];

		if(vertex_ids.find(vertex_one) == vertex_ids.end())
		{
			vertex_ids[vertex_one] = ID;
			vertex_names[ID] = vertex_one;
			ID = ID + 1;
		}

		if(vertex_ids.find(vertex_two) == vertex_ids.end())
		{
			vertex_ids[vertex_two] = ID;
			vertex_names[ID] = vertex_two;
		}
		
		std::getline(input_graph_file, line_str);
	}

	input_graph_file.close();

	adjacency_list.reserve(vertex_ids.size());

	for(unsigned int v = 0; v < vertex_ids.size(); v++)
	{
		adjacency_list.push_back(new std::list<unsigned int>);
	}


	input_graph_file.open(graph_file_name.c_str(), std::ios::in);

	while(! input_graph_file.eof())
	{
		line_vec = split(line_str,',');
		vertex_one = line_vec[0];
		vertex_two = line_vec[1];

		adjacency_list[vertex_ids[vertex_one]]->push_back(vertex_ids[vertex_two]);
		adjacency_list[vertex_ids[vertex_two]]->push_back(vertex_ids[vertex_one]);
		
		std::getline(input_graph_file, line_str);
	}

	input_graph_file.close();
}

/**
 * Graph destructor
 * @param 
 * @return 
 * @throws 
**/
Graph::~Graph()
{
	if(distance_matrix != NULL)
	{
		for(unsigned int v = 0; v < size(); v++)
		{
			free(distance_matrix[v]);
		}

		free(distance_matrix);
	}
	
	for(unsigned int v = 0; v < adjacency_list.size(); v++)
	{
		delete adjacency_list[v];
	}
	
	for(unsigned int v = 0; v < partition_sizes.size(); v++)
	{
		delete partition_sizes.at(v);
	}

	for(unsigned int v = 0; v < distance_str.size(); v++)
	{
		for(unsigned int d = 0; d < distance_str[v]->size(); d++)
		{
			delete distance_str[v]->at(d);
		}

		delete distance_str[v];
	}
}

/**
 * Counts the number of vertices in the input graph
 * @param values_file_name file with the vertex values
 * @return number of vertices
 * @throws
**/
unsigned int Graph::count_vertices(const std::string& values_file_name) const
{
	std::ifstream input_values_file(values_file_name.c_str());

	unsigned int num_vertices = std::count(
	std::istreambuf_iterator<char>(input_values_file),
	std::istreambuf_iterator<char>(), '\n');

	return num_vertices;
}

/**
 * Prints the slice tree distance structure
 * @param 
 * @return 
 * @throws 
**/
void Graph::print_distance_str_slice_tree()
{
	for(unsigned int v = 0; v < size(); v++)
	{
		std::cout << v;
		for(unsigned int d = 0; d < distance_str.at(v)->size(); d++)
		{
			for (std::list<unsigned int>::iterator it = 
				distance_str.at(v)->at(d)->begin(); 
				it != distance_str.at(v)->at(d)->end(); 
				++it)
			{
				std::cout << " " << *it << "(" << d << ")";
			}
		}
		std::cout << std::endl;
	}
}

/**
 * Builds a distance structure for the graph, this structure 
 * efficiently returns the list of vertices at a given distance
 * from a certain vertex
 * @param 
 * @return 
 * @throws 
**/
void Graph::build_distance_str_slice_tree()
{
	unsigned int num_vertices = size();
	distance_str.reserve(num_vertices);
	    
	std::vector<unsigned int> distances;
	distances.reserve(num_vertices);
	
	for(unsigned int v = 0; v < num_vertices; v++)
	{
		distances.push_back(UINT_MAX);
	}

	std::queue<unsigned int> queue;
	unsigned int u;
	unsigned int z;
	unsigned int max_distance;
	       
	for(unsigned int v = 0; v < num_vertices; v++)
	{
		max_distance = 0;

		for(u = 0; u < num_vertices; u++)
		{
			distances[u] = UINT_MAX;
		}

		/*Distances are computed using BFS*/
		distance_str.push_back(new std::vector< std::list<unsigned int >* >);
		distances[v] = 0;
		queue.push(v);
		
		while(! queue.empty())
		{
			u = queue.front();
			queue.pop();
			
			for (std::list<unsigned int>::iterator it = adjacency_list[u]->begin(); 
				it != adjacency_list[u]->end(); ++it)
			{
				z = *it;

				if(distances[z] > distances[u] + 1)
				{
					distances[z] = distances[u] + 1;
					
					if(distances[z] > max_distance)
					{
						max_distance = distances[z];
					}
					
					queue.push(z);
				}
			}
		}

		distance_str[v]->reserve(max_distance);

		if(max_distance > graph_diameter)
		{
			graph_diameter = max_distance;
		}

		for(unsigned int d = 0; d <= max_distance; d++)
		{
			distance_str[v]->push_back(new std::list<unsigned int>);
		}
		
		for(u = 0; u < num_vertices; u++)
		{
			if(distances[u] <= max_distance)
			{
				distance_str[v]->at(distances[u])->push_back(u);
			}
		}
	} 
}
/**
 * Builds the distance structure for a single center
 * @param center center
 * @param vertices_at_distance structure
 * @return
 * @throws
**/
void Graph::build_distance_str_slice_tree_vertex(unsigned int center, 
	std::vector<std::list<unsigned int>*>& 
	vertices_at_distance,
	const unsigned int max_radius) const
{
	std::vector<unsigned int> distances;
	distances.reserve(size());
	
	for(unsigned int v = 0; v < size(); v++)
	{
		distances.push_back(UINT_MAX);
	}

	std::queue<unsigned int> queue;
	unsigned int u;
	unsigned int z;
	unsigned int max_distance = 0;
	       
	distances[center] = 0;
	queue.push(center);
		
	while(! queue.empty())
	{
		u = queue.front();
		queue.pop();
			
		for (std::list<unsigned int>::iterator it = adjacency_list[u]->begin(); 
			it != adjacency_list[u]->end(); ++it)
		{
			z = *it;

			if(distances[z] > distances[u] + 1 && distances[u] + 1 <= max_radius)
			{
				distances[z] = distances[u] + 1;
					
				queue.push(z);
			}
		}
	}
	
	for(unsigned int v = 0; v < size(); v++)
	{
		if(distances[v] > max_distance && distances[v] <= max_radius)
		{
			max_distance = distances[v];
		}
	}

	vertices_at_distance.reserve(max_distance+1);
					
	for(unsigned int d = 0; d <= max_distance; d++)
	{
		vertices_at_distance.push_back(new std::list<unsigned int>);
	}

	for(u = 0; u < size(); u++)
	{
		if(distances[u] <= max_radius)
		{
			vertices_at_distance.at(distances[u])->push_back(u);
		}
	}
}

/**
 * Builds a distance structure for slice tree using a set of sample 
 * vertices, this strcutre efficiently returns the list of vertices 
 * at a given distance from a certain vertex
 * @param max_radius maximum radius for slice tree
 * @return 
 * @throws 
**/
void Graph::build_distance_str_slice_tree_sample(const unsigned max_radius)
{
	unsigned int num_vertices = size();
	distance_str.reserve(num_vertices);
	    
	std::vector<unsigned int> distances;
	distances.reserve(num_vertices);
	
	for(unsigned int v = 0; v < num_vertices; v++)
	{
		distances.push_back(max_radius+1);
		distance_str.push_back(new std::vector< std::list<unsigned int >* >);
	}

	std::queue<unsigned int> queue;
	unsigned int u;
	unsigned int z;
	unsigned int max_distance;
	
	for (std::list<unsigned int>::iterator v = samples.begin(); 
		v != samples.end(); ++v)
	{
		max_distance = 0;

		for(u = 0; u < num_vertices; u++)
		{
			distances[u] = UINT_MAX;
		}

		/*Distances are computed using BFS*/
		distances[*v] = 0;
		queue.push(*v);
		
		while(! queue.empty())
		{
			u = queue.front();
			queue.pop();
			
			for (std::list<unsigned int>::iterator it = adjacency_list[u]->begin(); 
				it != adjacency_list[u]->end(); ++it)
			{
				z = *it;

				if(distances[z] > distances[u] + 1 &&
					distances[u] + 1 <= max_radius)
				{
					distances[z] = distances[u] + 1;
					queue.push(z);
				}
			}
		}
		
		for(u = 0; u < num_vertices; u++)
		{
			if(distances[u] > max_distance &&
				distances[u] <= max_radius &&
				distances[u] < UINT_MAX)
			{
				max_distance = distances[u];
			}
		}

		for(u = 0; u < num_vertices; u++)
		{
			if(distances[u] <= max_distance)
			{
				while(distance_str[u]->size() <= distances[u])
				{
					distance_str[u]->push_back(new std::list<unsigned int>);
				}
	
				distance_str[u]->at(distances[u])->push_back(*v);
			}
		}
	}
}

/**
 * Builds a distance matrix for the graph
 * @param 
 * @return 
 * @throws 
**/
void Graph::build_distance_matrix()
{
	distance_matrix = (unsigned short int**) malloc (size() * sizeof(unsigned short int*));

	/*Allocating the distance matrix, some space is wasted here*/
	for(unsigned int v = 0; v < size(); v++)
	{
		distance_matrix[v] = (unsigned short int*) malloc (size() 
			* sizeof(unsigned short int));

		for(unsigned int u = 0; u < size(); u++)
		{
			distance_matrix[v][u] = USHRT_MAX;
		}
	}

	std::queue<unsigned int> queue;
	unsigned int u;
	unsigned int z;
	unsigned int max_distance = 0;
	       
	for(unsigned int v = 0; v < size(); v++)
	{
		queue.push(v);
		distance_matrix[v][v] = 0;
		
		while(! queue.empty())
		{
			u = queue.front();
			queue.pop();
			
			for (std::list<unsigned int>::iterator it = adjacency_list[u]->begin(); 
				it != adjacency_list[u]->end(); ++it)
			{
				z = *it;

				if(distance_matrix[v][z] > distance_matrix[v][u] + 1)
				{
					distance_matrix[v][z] = distance_matrix[v][u] + 1;
					
					if(distance_matrix[v][z] > max_distance)
					{
						max_distance = distance_matrix[v][z];
					}
					
					queue.push(z);
				}
			}
		}

		if(max_distance > graph_diameter)
		{
			graph_diameter = max_distance;
		}

		if(max_distance > USHRT_MAX)
		{
			std::cerr << "graph_compression: The diameter of the graph is larger than " << USHRT_MAX << std::endl;
			exit(1);
		}
	}
}

/**
 * Performs a BFS search from the center and bounded by radius distance
 * including in visited only the vertices in the bitmap
 * @param visited vector updated with the vertices visited in the search
 * @param center center
 * @param radius radius
 * @param bitmap
 * @return 
 * @throws 
**/
void Graph::bounded_bfs(std::vector<unsigned int>& visited, const unsigned int center,
	const unsigned int radius, const std::vector<bool>& bitmap) const
{
	
	std::vector<unsigned int> distances;
	distances.reserve(size());

	for(unsigned int v = 0; v < size(); v++)
	{
		distances.push_back(UINT_MAX);
	}

	std::queue<unsigned int> queue;
	unsigned int u;
	unsigned int z;
	
	distances[center] = 0;
	queue.push(center);
	
	if(bitmap[center])
	{
		visited.push_back(center);
	}
		
	while(! queue.empty())
	{
		u = queue.front();
		queue.pop();
			
		for (std::list<unsigned int>::iterator it = adjacency_list[u]->begin(); 
			it != adjacency_list[u]->end(); ++it)
		{
			z = *it;

			if(distances[z] > distances[u] + 1 && distances[u] + 1 <= radius)
			{
				distances[z] = distances[u] + 1;
				queue.push(z);
				
				if(bitmap[z])
				{
					visited.push_back(z);
				}
			}
		}
	}
}

/**
  * Updates some data structures used to compute upper bounds on 
  * the sizes of partitions generated by a slice.
  * @param center center
  * @param radius radius
  * @param dist_near_center distance to the center of the nearest
  * partition for each each vertex.
  * @param dist_center_part distance to the center of the partition
  * where the slice lies in for each vertex.
  * @param radius_near_center radius of the nearest partition
  * for each vertex
  * @param radius_part radius of the partition the slice lies in.
  * @throws
  * @return
**/
void Graph::update_partition_size_structs(
	const unsigned int center, 
	const unsigned int radius, 
	std::vector<unsigned int>& dist_near_center, 
	std::vector<unsigned int>& dist_center_part, 
	std::vector<unsigned int>& radius_near_center, 
	std::vector<unsigned int>& radius_part,
	const std::vector<bool>& bitmap) const
{
	std::vector<unsigned int> distances;
	distances.reserve(size());

	for(unsigned int v = 0; v < size(); v++)
	{
		distances.push_back(UINT_MAX);
	}

	std::queue<unsigned int> queue;
	unsigned int u;
	unsigned int z;
	
	distances[center] = 0;
	queue.push(center);
	
	while(! queue.empty())
	{
		u = queue.front();
		queue.pop();
			
		for (std::list<unsigned int>::iterator it = adjacency_list[u]->begin(); 
			it != adjacency_list[u]->end(); ++it)
		{
			z = *it;

			if(distances[z] > distances[u] + 1)
			{
				distances[z] = distances[u] + 1;
				queue.push(z);
			}
		}
	}

	for(unsigned int v = 0; v < distances.size(); v++)
	{
		if(bitmap.at(v))
		{
			if(distances.at(v) <= radius)
			{
				if(radius - distances.at(v) < 
					dist_center_part.at(v) - radius_part.at(v))
				{
					dist_center_part.at(v) = distances.at(v);
					radius_part.at(v) = radius;
				}
			}
			else
			{
				if(distances.at(v) - radius < 
					dist_near_center.at(v) - radius_near_center.at(v))
				{
					dist_near_center.at(v) = distances.at(v);
					radius_near_center.at(v) = radius;
				}
			}
		}
	}
}

/**
 * Builds a BFS-based sorted for the graph.
 * @param 
 * @return 
 * @throws 
**/
void Graph::build_bfs_vector()
{
	std::vector<unsigned int> distances;
	distances.reserve(size());

	for(unsigned int v = 0; v < size(); v++)
	{
		distances.push_back(UINT_MAX);
	}
	
	sorted_vector.reserve(size());

	std::queue<unsigned int> queue;
	unsigned int u;
	unsigned int z;

	/*The BFS starts from the vertex 0*/
	distances[0] = 0;
	queue.push(0);
	sorted_vector.push_back(0);

	while(! queue.empty())
	{
		u = queue.front();
		queue.pop();
			
		for (std::list<unsigned int>::iterator it = adjacency_list[u]->begin(); 
			it != adjacency_list[u]->end(); ++it)
		{
			z = *it;

			if(distances[z] > distances[u] + 1)
			{
				distances[z] = distances[u] + 1;
				queue.push(z);
				
				sorted_vector.push_back(z);
			}
		}
	}
}

/**
 * Builds a priority first-based sorted vector for the graph
 * @param 
 * @return 
 * @throws 
**/
void Graph::build_priority_first_vector()
{
	std::vector<bool> visited;
	std::vector<unsigned int> count;
	sorted_vector.reserve(size());
	visited.reserve(size());

	for(unsigned int v = 0; v < size(); v++)
	{
		visited.push_back(false);
		count.push_back(0);
	}

	/*The priority search starts from the vertex 0*/
	sorted_vector.push_back(0);
	visited.at(0) = true;
	
	for (std::list<unsigned int>::iterator it = adjacency_list[0]->begin(); 
		it != adjacency_list[0]->end(); ++it)
	{
		count[*it] = count[*it] + 1;
	}

	unsigned int vertex_max;
	unsigned int vertex_max_count;

	while(sorted_vector.size() < size())
	{
		vertex_max = 0;
		vertex_max_count = 0;

		for(unsigned int v = 0; v < size(); v++)
		{
			if(! visited.at(v))
			{
				if(count.at(v) >= vertex_max_count)
				{
					vertex_max = v;
					vertex_max_count = count.at(v);
				}
			}
		}

		visited.at(vertex_max) = true;
		sorted_vector.push_back(vertex_max);

		for (std::list<unsigned int>::iterator it = adjacency_list[vertex_max]->begin(); 
			it != adjacency_list[vertex_max]->end(); ++it)
		{
			count[*it] = count[*it] + 1;
		}
	}
}

/**
 * Generates a random int between the interval [0, limit]
 * @param limit limit
 * @return random int
 * @throws 
**/
unsigned int random_int(const unsigned int limit)
{
	return rand() % limit;
}

/**
 * Generates a random double between the interval [0, 1]
 * @param limit limit
 * @return random int
 * @throws
**/
double random_double()
{
	return (double) rand() / RAND_MAX;
}

/**
 * Selects a set of vertices as random samples 
 * (without replacement) from the graph
 * @param num_samples number of samples
 * @return
 * @throws 
**/
void Graph::set_uniform_sample(const unsigned int _num_samples)
{
	uniform_sampling = true;
	srand (time(NULL));
	num_samples = _num_samples;
	count_sample.reserve(num_samples);

	unsigned int sample;

	for(unsigned int v = 0; v < size(); v++)
	{
		count_sample.push_back(0);
	}

	while(samples.size() < num_samples && samples.size() < size())
	{
		sample = random_int(size());

		if(! count_sample.at(sample))
		{
			samples.push_back(sample);
		}
		
		sum_values += vertex_values.at(sample);
		count_sample.at(sample)++;
	}

	sum_weights = _num_samples;
}

/**
 * Selects a set of biased vertices from the graph according to |v-mu|
 * @param num_samples number of samples
 * @return
 * @throws
**/
void Graph::set_biased_sample(const unsigned int _num_samples)
{
	biased_sampling = true;
	srand (time(NULL));
	count_sample.reserve(size());
	num_samples = _num_samples;

	unsigned int sample;
	std::vector<float> selection_prob;
	mu = 0;
	lambda = 0;
	double rd;

	for(unsigned int v = 0; v < size(); v++)
	{
		count_sample.push_back(0);
		selection_prob.push_back(0);

		mu += vertex_values.at(v);
	}
	
	mu = (float) mu / size();

	for(unsigned int v = 0; v < size(); v++)
	{
		selection_prob.at(v) = fabs(vertex_values.at(v) - mu);
		lambda +=  fabs(vertex_values.at(v) - mu);
	}

	for(unsigned int v = 0; v < size(); v++)
	{
		selection_prob.at(v) /= lambda;

		if(v > 0)
		{
			selection_prob.at(v) += selection_prob.at(v-1);
		}
	}

	while(samples.size() < num_samples)
	{
		rd = random_double();
		sample = 0;
		
		while(sample < size() && selection_prob.at(sample) < rd) sample++;
		
		if(sample < size())
		{
			count_sample.at(sample) += 1;
			sum_values += vertex_values.at(sample);
			sum_weights += (double) lambda / fabs(vertex_values.at(sample) - mu);

			if(count_sample.at(sample) == 1) samples.push_back(sample);
		}
	}
/*
	for(unsigned int v = 0; v < size(); v++)
	{
		printf("v = %d, orig_value(v) = %lf, value(v) = %lf, weight(v) = %lf, count(v) = %d\n", 
			v, vertex_values.at(v), value(v), weight(v), count_sample.at(v));
	}

	printf("mu = %lf, lambda = %lf\n", mu, lambda);
*/
}

/**
 * Prints the graph (for debugging purposes)
 * @param
 * @return
 * @throws 
**/
void Graph::print()
{
	for(unsigned int v = 0; v < size(); v++)
	{
		std::cout << name(v) << "(" << value(v) << "):";
		
		for (std::list<unsigned int>::iterator it = adjacency_list[v]->begin(); 
			it != adjacency_list[v]->end(); ++it)
		{
			std::cout << " " << *it;
		}

		std::cout << std::endl;
	}
}

void build_distance_str(const unsigned int root, 
	 std::vector<std::vector<unsigned int>*>& partition_sizes, 
	 std::vector< std::list<unsigned int>* >& adjacency_list,
	 const unsigned int max_radius)
{
	std::queue<unsigned int> queue;
	unsigned int z;
	unsigned int max_distance = 0;
	std::vector<unsigned int> distances;
	distances.reserve(adjacency_list.size());

	for(unsigned int u = 0; u < adjacency_list.size(); u++)
	{
		distances.push_back(UINT_MAX);
	}

	/*Distances are computed using BFS*/
	distances[root] = 0;
	queue.push(root);
	unsigned int u;

	while(! queue.empty())
	{
		u = queue.front();
		queue.pop();
			
		for (std::list<unsigned int>::iterator it = adjacency_list[u]->begin(); 
			it != adjacency_list[u]->end(); ++it)
		{
			z = *it;

			if(distances[z] > distances[u] + 1 &&
				distances[u] + 1 <= max_radius)
			{
				distances[z] = distances[u] + 1;
				queue.push(z);
			}
		}
	}
	
	for(u = 0; u < adjacency_list.size(); u++)
	{
		if(distances[u] > max_distance
			&& distances[u] <= max_radius) 
		{
			max_distance = distances[u];
		}
	}
	
	partition_sizes.at(root)->reserve(max_distance+1);

	for(u = 0; u <= max_distance; u++)
	{
		partition_sizes.at(root)->push_back(0);
	}

	for(u = 0; u < adjacency_list.size(); u++)
	{
		if(distances[u] <= max_radius)
		{
			partition_sizes.at(root)->at(distances[u])++;
		}
	} 
	
	for(u = 1; u <= max_distance; u++)
	{
		partition_sizes.at(root)->at(u) += partition_sizes.at(root)->at(u-1);
	}
}

void run_thread(std::list<unsigned int>* pool, 
	std::vector< std::list<unsigned int>* >* adjacency_list, 
	pthread_mutex_t* mutex_pool, 
	std::vector<std::vector<unsigned int>*>* partition_sizes,
	const unsigned int max_radius)
{
	unsigned int vertex;
	
	while(true)
	{
		pthread_mutex_lock(mutex_pool);
		
		if(pool->empty())
		{
			pthread_mutex_unlock(mutex_pool);
			break;
		}
		else
		{
			vertex = pool->front();
			pool->pop_front();
			pthread_mutex_unlock(mutex_pool);
		}

		build_distance_str(vertex, *partition_sizes, *adjacency_list, max_radius);
	}
}

void* start_thread(void* v_parameter)
{
	PthreadParameters* parameter = (PthreadParameters*) v_parameter;
	run_thread(parameter->pool, parameter->adjacency_list, parameter->mutex_pool, 
		 parameter->partition_sizes, parameter->max_radius);
	pthread_exit(NULL);
}

/**
 * Pre-computes the partitions sizes for all centers and radius
 * and saves them in a file. Uses multiple threads to speed up
 * the computations.
 * @param num_threads number of threads available
 * @param output_file_name output file
 * @return
 * @throws
**/
void Graph::pre_compute_partition_sizes(const unsigned int num_threads, 
	const std::string& output_file_name,
	const unsigned int max_radius)
{
	std::vector<PthreadParameters*> parameters;
	parameters.reserve(num_threads);
	PthreadParameters* parameter;
	std::list<unsigned int> pool;
	partition_sizes.reserve(size());

	pthread_mutex_t* mutex_pool = new pthread_mutex_t;
	pthread_mutex_init(mutex_pool, NULL);

	for(unsigned int v = 0; v < size(); v++)
	{
		pool.push_back(v);
		partition_sizes.push_back(new std::vector<unsigned int>);
	}

	pthread_t* threads = (pthread_t*) malloc (num_threads * sizeof(pthread_t));

	for(unsigned int t = 0; t < num_threads; t++)
	{
		parameter = new PthreadParameters;
		
		parameter->adjacency_list = &adjacency_list;
		parameter->mutex_pool = mutex_pool;
		parameter->pool = &pool;
		parameter->partition_sizes = &partition_sizes;
		parameter->max_radius = max_radius;
		parameters.push_back(parameter);

		pthread_create(&threads[t], NULL, start_thread, parameter);
	}

 	for(unsigned int i = 0; i < num_threads; i++)
	{
 		pthread_join(threads[i], NULL);
	}
	
	for(unsigned int i = 0; i < num_threads; i++)
	{
		delete parameters[i];
	}

	free(threads);
	delete mutex_pool;

	std::ofstream output_file(output_file_name.c_str());
	
	for(unsigned int v = 0; v < size(); v++)
	{
		output_file << v;
		
		for(unsigned int d = 0; d <= max_radius; d++)
		{
			if(d < partition_sizes.at(v)->size())
			{
				output_file << "," << partition_sizes.at(v)->at(d);
			}
			else
			{
				output_file << ",0";
			}
		}
		
		output_file << "\n";
	}

	output_file.close();
}

/**
 * Reads the pre-computed partition sizes from a file.
 * @param input_file_name input file
 * @return
 * @throws
**/
void Graph::read_partition_sizes(const std::string& input_file_name)
{
	std::ifstream input_file(input_file_name.c_str());
	partition_sizes.reserve(size());
	unsigned int i = 0;
	std::vector< std:: string > line_vec;
	std::string line_str;
	
	for(unsigned int v = 0; v < size(); v++)
	{
		partition_sizes.push_back(new std::vector<unsigned int>);
	}
	
	std::getline(input_file, line_str);
	
	while(! input_file.eof())
	{
		line_vec = split(line_str,',');
		
		partition_sizes.at(i)->reserve(line_vec.size()-1);
		
		for(unsigned int j = 1; j < line_vec.size(); j++)
		{
			partition_sizes.at(i)->push_back(atoi(line_vec.at(j).c_str()));
		}

		std::getline(input_file, line_str);
		i++;
	}
	
	input_file.close();
}


