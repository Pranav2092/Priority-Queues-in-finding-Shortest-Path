# Priority-Queues-in-finding-Shortest-Path

In this project we have implemented the priority queues using 3 different heaps:
  1. Binomial Heap
  2. Height-Balanced Leftist Heap
  3. Min-Max Heap

Using these different prioirtiy queues we are implementing the A* algorithm for pathfinding in the graph.Then we are comparing the performances of the different implementtions using the runtime of each implementation based on the following metrics:
  1. Number of nodes
  2. Density of graph
  3. Weight allocated to the heuristic

For the comapritive study we are generating a random dataset using our customized generator function.
We have implemented our project in C++ language.

The files include in this repository:
  1. `binomial heap implementaion.cpp` : Here we have implemented A* algorithm using prioirty queue which is implemented using binomial heap.
  2. `leftist heap implementaion.cpp` : Here we have implemented A* algorithm using prioirty queue which is implemented using leftist heap.
  3. `min-max heap implementaion.cpp` : Here we have implemented A* algorithm using prioirty queue which is implemented using min-max heap.
  4. `source.cpp` : It contains the implementation of A* algorithm using the min heap.This code acts the source code for the implementaion of the above 3 programs. 
  5. `graph_generator.cpp` : This provides the dataset by generating random weighted graph in the form of adjacency matrix.It also generates the coordinates of the nodes, which we use to calculated the heuristic value.This code is integrated in each of our above implemetations.
  6. `main.cpp` : Using this file we are comparing the runtime of each implementation by varying the number of nodes.
  7. `main2.cpp` : Using this file we are comparing the runtime of each implementation by varying the weight associated to the heuristic value.
  8. `main3.cpp` : Using this file we are comparing the runtime of each implementation by varying the density of graph(in terms of number of edges).
  9. `Report.pdf`: This contains the study of the findings of our project.
