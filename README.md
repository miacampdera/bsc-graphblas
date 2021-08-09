# SSSP ALGORITHMS IN GRAPHBLAS 
Mia Campdera-Pulido

## Description
This project consists of several single-source shortest path (SSSP) algorithms: unweighted SSSP, weighted SSSP, and batch SSSP. The algorithms are solved using linear algebra techniques implemented in the GraphBLAS C API specification and support weighted and unweighted graphs in the form of adjacency matrices.
The project aims to use the algorithms defined to solve board games and find their remoteness. Non-loopy board games, such as tic tac toe, can be defined as a tree and loopy board games, such as chess, as a graph. By putting board game graphs in the form of adjacency matrices we can run the SSSP algorithms and find the desired information.

## Algorithms
The project supports three SSSP algorithms: unweighted SSSP, weighted SSSP, and batch SSSP. 

##
**Unweighted SSSP**
The unweighted SSSP algorithm calculates the distances between a specified source node to every other node in the graph, for an unweighted graph. Observing the fact that calculating the distances between a source node and every other node where there is no weight between the edges of the nodes (the weight can be interpreted as constant or equivalent to 1) is the same as performing breadth first search, BFS, the unweighted SSSP algorithm was designed as a BFS algorithm. The BFS algorithm calculates the levels of a graph; the source node corresponds to level 0, those nodes directly connected to it (neighbors) correspond to level 1, those nodes connected to the neighbours are on level 2, and so on until all nodes have been visited.

##
**Weighted SSSP**
The weighted SSSP algorithm calculates the distances between a specified source node to every other node in the graph, for a weighted graph. Taking as inspiration Dijkstra’s algorithm implementation with a minimum priority queue (see annex), the weighted SSSP algorithm was designed with a min-plus semiring which is further discussed in the implementation section. 

##
**Batch SSSP**
The batch SSSP algorithm calculates the distances of every node to every other node. The design of the algorithm uses the unweighted SSSP algorithm to perform calculations, however, by just changing a single line of code, the weighted SSSP algorithm can be used. 

## Installation

1. Clone this repo
2. Clone GraphBLAS repo: https://github.com/DrTimothyAldenDavis/GraphBLAS
