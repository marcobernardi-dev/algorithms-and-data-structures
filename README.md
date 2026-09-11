# Algorithms and Data Structures
A collection of algorithms and data structures implemented in C as part of my Computer Science studies at the University of Turin.
The project focuses on fundamental data structures, algorithms, generic programming, unit testing, and complexity analysis.

## Sorting algorithms
Implementation of a hybrid sorting algorithm combining QuickSort and SelectionSort.
The algorithm switches between the two sorting strategies according to a configurable threshold k. The implementation is generic and supports different data types through a comparison function.

Main concepts:
-QuickSort
-SelectionSort
-Hybrid sorting
-Generic programming in C

The project also includes support for sorting structured records according to different fields and experimental measurements for different values of k.

## Hash table
Implementation of a generic hash table using separate chaining for collision handling.
The data structure supports generic keys and values through user-provided comparison and hash functions.

Supported operations:
-Insert / update (put)
-Search (get)
-Key lookup (contains_key)
-Removal (remove)
-Size
-Key set
-Memory deallocation

The implementation includes dynamic resizing and rehashing to maintain a suitable load factor.
Expected amortized complexity for the main operations is O(1), while retrieving the complete key set requires O(n) time.
The hash table is also reused by the priority queue implementation in the priority-queue-heap project and by the graph implementation in the graphs-and-bfs project.

## Priority queue - ternary heap
Implementation of a generic priority queue based on a ternary heap.
The heap is represented using an array and uses a comparison function to remain independent of the type of the stored elements.

Main operations:
-push
-pop
-top
-size

Complexity:
push --> O(log n)
pop --> O(log n)
top --> O(1)
size --> O(1)

Memory: O(n)

The implementation was tested on a dataset containing one million tasks, producing one million output records in approximately 1.17 seconds in the recorded experiment.

## Graphs and Breadth-First Search
Implementation of a generic graph data structure optimized for sparse graphs, together with a Breadth-First Search (BFS) traversal algorithm.
The graph implementation uses the hash table developed in the previous project to associate nodes with their adjacency information.

The graph supports:
-Directed and undirected graphs
-Labelled and unlabelled edges
-Generic node types
-Generic edge labels
-Node insertion, removal and lookup
-Edge insertion, removal and lookup
-Neighbour retrieval
-Graph size queries

The BFS implementation uses a queue and a visited-node structure to ensure that each reachable node is processed at most once.
For sparse graphs and efficient hash table operations, the expected complexity of BFS is: O(N + M) where N is the number of nodes and M is the number of edges.
The implementation was tested on an Italian geographical graph. Starting from Turin, the BFS visited 18,640 locations in approximately 6.46 seconds in the recorded experiment.

## Testing
Each project includes dedicated unit tests covering the main operations and relevant edge cases.
The tests include, depending on the data structure:
-Basic operations
-Empty structures
-Boundary conditions
-Duplicate elements
-Different input configurations
-Invalid or missing elements
-Graph connectivity cases
-BFS with missing starting nodes

Each exercise provides a Makefile to compile both the main program and its test executable.

Running the tests:
From each project directory:
"make"
Then run the corresponding test executable:
"./bin/test_ex1"
"./bin/test_ex2"
"./bin/test_ex3"
"./bin/test_ex4"
All tests pass in the current version of the project.

# Academic Context
This project was developed as part of the Algorithms and Data Structures course within the Computer Science degree program at the University of Turin.
The project provided practical experience with algorithm design, data structures, generic programming in C, complexity analysis, testing, memory management, and performance evaluation.