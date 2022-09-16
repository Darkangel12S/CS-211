#include "../graphutils.h" // header for functions to load and free adjacencyList

// A program to find the minimum spanning tree of a weighted undirected graph using Prim's algorithm

int main ( int argc, char* argv[] ) {

    // READ INPUT FILE TO CREATE GRAPH ADJACENCY LIST
    AdjacencyListNode* adjacencyList = NULL; 
    unsigned long int graphSize = adjMatrixToList(argv[1], &adjacencyList);

    // An array that keeps track of who is the parent node of each graph node we visit
    // In Prim's algorithm, this parents array keeps track of what is the edge that connects a node to the MST.
    graphNode_t* parents = calloc( graphSize, sizeof(graphNode_t));
    for (size_t i = 0; i < graphSize; i++) 
    {
        parents[i] = -1; // -1 indicates that a nodes is not yet visited; i.e., node not yet connected to MST.
    }

    // root of the mst
    graphNode_t root = rand()%graphSize;
    parents[root] = root;

    // Prim's algorithm:
    // A greedy algorithm that builds the minimum spanning tree.
    // For a graph with N nodes, the minimum spanning tree will have N-1 edges spanning all nodes.
    // Prim's algorithm starts with all nodes unconnected.
    // At each iteration of Prim's algorithm, the minimum weight node that connects an unconnected node to the connected set of nodes is added to the MST.
    for (unsigned iter = 0; iter < graphSize-1; iter++) 
    {
        double minWeight = DBL_MAX; // If we find an edge with weight less than
                                    // this minWeight, and edge connects a new node to MST, 
                                    // then mark this as the minimum weight to beat.
        graphNode_t minDest = -1;
        graphNode_t minSource = -1;

        for (graphNode_t source = 0; source < graphSize; source++) // loop thru the adj list
        {
            if (parents[source] != -1) // if already visited
            {
                // go to neighbor
                AdjacencyListNode* neighbor = adjacencyList[source].next; 
                while (neighbor) // loops through all neighbors
                {
                    if (parents[neighbor->graphNode] == -1) // if neighbor is not visited
                    {
                        if ((neighbor->weight) < minWeight) // if the neighbor weight is less than minweight
                        {
                            minWeight = neighbor->weight; // set the minimum weight to the weight
                            minDest = neighbor->graphNode; // set the minimum destination to the current node
                            minSource = source; // set the minimum source to the current source we are on
                        }
                    }
                    neighbor = neighbor->next;
                } // endwhile
            } // endif 
            // if not visited, continue loop (no code needed here)
        } // endfor
        parents[minDest] = minSource; // we found the minimum weight, and set it as connected
    }

    for (size_t i = 0; i < graphSize; i++)
    {
        if (i != parents[i]) { printf("%ld %ld\n", parents[i], i); } 
    }

    free(parents);
    freeAdjList(graphSize, adjacencyList);

    return EXIT_SUCCESS;
}
