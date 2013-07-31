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
	graph_diameter = 0;
	distance_matrix = NULL;
	num_edges = 0;
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
	graph_diameter = 0;
	distance_matrix = NULL;
	num_edges = 0;
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

		if(vertex_ids[vertex_one] > vertex_ids[vertex_two])
		{
			num_edges++;
		}
		
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
		
		if(vertex_ids[vertex_one] > vertex_ids[vertex_two])
		{
			num_edges++;
		}
		
		std::getline(input_graph_file, line_str);
	}

	input_graph_file.close();
}

/**
 * Partitions the graph by removing edges until no partitions has size
 * larger than max_size_partition.
 * @param max_size_partition max size partition
 * @return
 * @throws
**/
void Graph::partition_graph(const unsigned int max_size_partition)
{
	std::vector<edge_t*> candidate_edges;
	candidate_edges.reserve(num_edges);
	edge_t* edge;
	unsigned int u;

	for(unsigned int v = 0; v < adjacency_list.size(); v++)
	{
		for (std::list<unsigned int>::iterator it = adjacency_list[v]->begin(); 
			it != adjacency_list[v]->end(); ++it)
		{
			u = *it;

			if(v > u)
			{
				edge = new edge_t;
				edge->v_one = v;
				edge->v_two = u;

				edge->difference = fabs(value(v) - value(u));

				candidate_edges.push_back(edge);
			}
		}
	}

	std::sort(candidate_edges.begin(), candidate_edges.end(), CompareEdges());
	unsigned size_largest = size();

	for(unsigned int e = 0; e < candidate_edges.size(); e++)
	{
		remove_edge(candidate_edges.at(e));
		
		if(e % (size_largest-max_size_partition) == 0)
		{
			size_largest = size_largest_connected_component();
		}
		
		std::cout << "size_largest = " << size_largest << std::endl;

		if(size_largest <= max_size_partition)
		{
			break;
		}
	}
}

void Graph::remove_edge(const edge_t* edge)
{
	unsigned int u;

	for (std::list<unsigned int>::iterator it = adjacency_list[edge->v_one]->begin(); 
		it != adjacency_list[edge->v_one]->end(); ++it)
	{
		u = *it;

		if(u == edge->v_two)
		{
			adjacency_list[edge->v_one]->erase(it);
			break;
		}
	}
	
	for (std::list<unsigned int>::iterator it = adjacency_list[edge->v_two]->begin(); 
		it != adjacency_list[edge->v_two]->end(); ++it)
	{
		u = *it;

		if(u == edge->v_one)
		{
			adjacency_list[edge->v_two]->erase(it);
			break;
		}
	}
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
	unsigned int max_distance = 0;
	       
	for(unsigned int v = 0; v < num_vertices; v++)
	{
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
			else
			{
				std::cerr << "graph_compression: The graph is not connected. Slice tree works only for connected graphs." << std::endl;
				exit(1);
			}
		}
	} 
}

/**
 * Builds a distance structure for slice tree using a set of sample 
 * vertices, this strcutre efficiently returns the list of vertices 
 * at a given distance from a certain vertex
 * @param 
 * @return 
 * @throws 
**/
void Graph::build_distance_str_slice_tree_sample()
{
	unsigned int num_vertices = size();
	distance_str.reserve(num_vertices);
	    
	std::vector<unsigned int> distances;
	distances.reserve(num_vertices);
	
	for(unsigned int v = 0; v < num_vertices; v++)
	{
		distances.push_back(UINT_MAX);
		distance_str.push_back(new std::vector< std::list<unsigned int >* >);
	}

	std::queue<unsigned int> queue;
	unsigned int u;
	unsigned int z;
	unsigned int max_distance = 0;
	       
	for (std::list<unsigned int>::iterator v = samples.begin(); 
		v != samples.end(); ++v)
	{
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

		if(max_distance > graph_diameter)
		{
			graph_diameter = max_distance;
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
			else
			{
				std::cerr << "graph_compression: The graph is not connected. Slice tree works only for connected graphs." << std::endl;
				exit(1);
			}
		}
		
		/*When sampling is applied we do not obtain the graph
		* diameter, thus we set it to a max value*/
		if(graph_diameter < MAX_GRAPH_DIAMETER)
		{
			graph_diameter = MAX_GRAPH_DIAMETER;
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

const unsigned int Graph::bfs(const unsigned root, unsigned int& num_visited, std::vector<bool>& visited)
{
	unsigned int size_component = 1;
	std::queue<unsigned int> queue;
	unsigned int u;
	unsigned int z;
	
	queue.push(root);
	visited.at(root) = true;

	while(! queue.empty())
	{
		u = queue.front();
		queue.pop();
			
		for (std::list<unsigned int>::iterator it = adjacency_list[u]->begin(); 
			it != adjacency_list[u]->end(); ++it)
		{
			z = *it;

			if(! visited.at(z))
			{
				queue.push(z);
				visited.at(z) = true;
				size_component++;
			}
		}
	}
	
	num_visited = num_visited + size_component;

	return size_component;
}

const unsigned int Graph::size_largest_connected_component()
{
	std::vector<bool> visited;
	visited.reserve(size());
	unsigned int num_visited = 0;
	unsigned int size_component;
	unsigned int size_largest = 0;

	for(unsigned int v = 0; v < size(); v++)
	{
		visited.push_back(false);
	}

	while(num_visited < size())
	{
		for(unsigned int v = 0; v < size(); v++)
		{
			if(! visited.at(v))
			{
				size_component = bfs(v, num_visited, visited);

				if(size_component > size_largest)
				{
					size_largest = size_component;
				}
			}
		}
	}

	return size_largest;
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
 * Selects a set of vertices as random samples from the graph
 * @param num_samples number of samples
 * @return
 * @throws 
**/
void Graph::set_sample(const unsigned int num_samples)
{
	srand (time(NULL));
	bitmap_sample.reserve(size());
	unsigned int sample;

	for(unsigned int v = 0; v < size(); v++)
	{
		bitmap_sample.push_back(false);
	}

	while(samples.size() < num_samples && samples.size() < size())
	{
		sample = random_int(size());

		if(! bitmap_sample.at(sample))
		{
			bitmap_sample.at(sample) = true;
			samples.push_back(sample);
		}
	}
}

