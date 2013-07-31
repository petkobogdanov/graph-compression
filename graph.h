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
 * Definitions of a class for graph manipulation
**/

#ifndef GRAPH_H
#define GRAPH_H

#define MAX_GRAPH_DIAMETER UCHAR_MAX

/*std includes*/
#include <string>
#include <exception>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <algorithm> 
#include <sstream>

typedef struct Edge
{
	unsigned int v_one;
	unsigned int v_two;
	double difference;
}edge_t;

class CompareEdges
{
	public:
		bool operator()(const edge_t* edge_one, 
			const edge_t* edge_two) const
		{
			return edge_one->difference < edge_two->difference;
		}
};

/**
 * Class for graph manipulation
**/
class Graph
{
	public:
		/**
		 * Graph constructor
		 * @param  graph_file_name input graph file name
		 * @param  value_file_name input values file name
		 * @return 
		 * @throws 
		 **/

		Graph(const std::string& graph_file_name, const std::string& values_file_name) throw (std::ios_base::failure);
		
		/**
		 * Graph constructor
		 * @param  graph_file_name input graph file name
		 * @return 
		 * @throws 
		 **/
		Graph(const std::string& graph_file_name) throw (std::ios_base::failure);
		
		/**
		 * Graph destructor
		 * @param 
		 * @return 
		 * @throws 
		**/
		virtual ~Graph();
		
		/**
		 * Partitions the graph by removing edges until no partitions has size
		 * larger than max_size_partition.
		 * @param max_size_partition max size partition
		 * @return
		 * @throws
		**/
		void partition_graph(const unsigned int max_size_partition);

		/**
		 * Builds a distance structure for slice tree, this structure 
		 * efficiently returns the list of vertices at a given distance
		 * from a certain vertex
		 * @param 
		 * @return 
		 * @throws 
		**/
		void build_distance_str_slice_tree();
		
		/**
		 * Builds a distance structure for slice tree using a set of sample 
		 * vertices, this strcutre efficiently returns the list of vertices 
		 * at a given distance from a certain vertex
		 * @param 
		 * @return 
		 * @throws 
		**/
		void build_distance_str_slice_tree_sample();

		/**
		 * Builds a distance matrix for the graph
		 * @param 
		 * @return 
		 * @throws 
		**/
		void build_distance_matrix();

		/**
		 * Builds a BFS-based sorted vector for the graph
		 * @param 
		 * @return 
		 * @throws 
		**/
		void build_bfs_vector();

		/**
		 * Builds a priority first-based sorted vector for the graph
		 * @param 
		 * @return 
		 * @throws 
		**/
		void build_priority_first_vector();
		
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
		void bounded_bfs(std::vector<unsigned int>& visited, const unsigned int center, const unsigned int radius, const std::vector<bool>& bitmap) const;

		/**
		 * Selects a set of vertices as random samples from the graph
		 * @param num_samples number of samples
		 * @return
		 * @throws 
		**/
		void set_sample(const unsigned int num_samples);

		/*Inline methods:*/
		
		/**
		 * Returns the size of the graph
		 * @param 
		 * @return size
		 * @throws 
		**/
		const inline unsigned int size() const
		{
			return adjacency_list.size();
		}

		/**
		 * Returns the diameter of the graph
		 * @param 
		 * @return diameter
		 * @throws 
		**/
		const inline unsigned int diameter() const
		{
			return graph_diameter;
		}

		/**
		 * Returns the number of vertices at a given distance 
		 * from a vertex given as parameter
		 * @param vertex vertex
		 * @param distance distance
		 * @return number of vertices
		 * @throws 
		**/
		inline std::list<unsigned int>* vertices_at_distance(const unsigned int vertex, 
			const unsigned int distance) const
		{
			return distance_str[vertex]->at(distance);
		}

		/**
		 * Returns the distance between two vertices
		 * @param v_one vertex
		 * @param v_two vertex
		 * @return distance
		 * @throws 
		**/
		const inline unsigned int distance(const unsigned int v_one, 
			const unsigned int v_two) const
		{
			return distance_matrix[v_one][v_two];
		}

		/**
		 * Returns the maximum distance from a given vertex 
		 * to any other vertex
		 * @param vertex
		 * @return maximum distance
		 * @throws 
		**/
		const inline unsigned int max_distance(const unsigned int vertex) const 
		{
			return distance_str[vertex]->size();
		}
		
		/**
		 * Returns the value of a vertex
		 * @param 
		 * @return diameter
		 * @throws 
		**/
		const inline double value(const unsigned int v) const
		{
			return vertex_values.at(v);
		}

		/**
		 * Returns the name of a vertex
		 * @param 
		 * @return diameter
		 * @throws 
		**/
		const inline std::string name(const unsigned int v) const
		{
			return vertex_names.at(v);
		}

		/**
		 * Sets a value to a vertex in the graph
		 * @param v vertex
		 * @param value value
		 * @return 
		 * @throws 
		**/
		inline void set_value(const unsigned int v, const double value)
		{
			vertex_values[v] = value;
		}
		
		/**
		 * Returns the id of the vertex at the pos position 
		 * of the sorted vector
		 * @param pos position
		 * @return vertex id
		 * @throws 
		**/
		const inline unsigned int at(const unsigned int pos) const
		{
			return sorted_vector.at(pos);
		}
	private:
		std::vector< std::list<unsigned int>* > adjacency_list;
		unsigned short int** distance_matrix;
		std::map<unsigned int, double> vertex_values;
		std::map<unsigned int, std::string> vertex_names;
		std::vector< std::vector< std::list<unsigned int >* >* > distance_str;
		std::vector<unsigned int> sorted_vector; 
		unsigned int graph_diameter;
		unsigned int num_edges;
		std::vector<bool> bitmap_sample;
		std::list<unsigned int> samples;

		const unsigned int size_largest_connected_component();
		const unsigned int bfs(const unsigned root, 
			unsigned int& num_visited, std::vector<bool>& visited);
		void remove_edge(const edge_t* edge);

		/**
		 * Reads the graph data
		 * @param graph_file_name input file with edges
		 * @param values_file_name input file with vertex values
		 * @return
		 * @throws ios_base::failure in case can't read the input files
		 **/
		void read_graph(const std::string& graph_file_name, const std::string& values_file_name) throw (std::ios_base::failure);

		/**
		 * Reads the graph data
		 * @param graph_file_name input file with edges
		 * @return
		 * @throws ios_base::failure in case can't read the input files
		 */
		void read_graph(const std::string& graph_file_name) throw (std::ios_base::failure);

		/**
		 * Counts the number of vertices in the input graph
		 * @param values_file_name file with the vertex values
		 * @return number of vertices
		 * @throws
		**/
		unsigned int count_vertices(const std::string& values_file_name) const;
};

#endif
