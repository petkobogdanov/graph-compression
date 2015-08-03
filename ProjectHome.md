Many real-world complex systems can be modeled
as dynamic networks with real-valued vertex/edge attributes.
Examples include users’ opinions in social networks and average
speeds in a road system. When managing these large
dynamic networks, compressing attribute values becomes a key
requirement, since it enables the answering of attribute-based
queries regarding a node/edge or network region based on a
compact representation of the data. To address this problem, we
introduce a lossy network compression scheme called Slice Tree
(ST), which partitions a network into smooth regions with respect
to node/edge values and compresses each value as the average
of its region. ST applies a compact representation for network
partitions, called slices, that are defined as a center node and
radius distance. We propose an importance sampling algorithm
to efficiently prune the search space of candidate slices in the ST
construction by biasing the sampling process towards the node
values that most affect the compression error. The effectiveness of
ST in terms of compression error, compression rate, and running
time is demonstrated using synthetic and real datasets. ST scales
to million-node instances and removes up to 87% of the error in
attribute values with a 103
compression ratio. We also illustrate
how ST captures relevant phenomena in real networks, such as
research collaboration patterns and traffic congestions.

# Publication: #

[Hierarchical In-Network Attribute Compression via Importance Sampling](http://www.cs.ucsb.edu/~arlei/pubs/icde15.pdf)
Arlei Silva, Petko Bogdanov, Ambuj K. Singh
IEEE International Conference on Data Engineering (ICDE), 2015