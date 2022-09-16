#include "../graphutils.h" // header for functions to load and free adjacencyList

// A program to print the edge list of a graph given the adjacency matrix
int main (int argc, char* argv[]) 
{

    // FIRST, READ THE ADJACENCY MATRIX FILE
    AdjacencyListNode* adjacencyList = NULL; 
    unsigned long int graphSize = adjMatrixToList(argv[1], &adjacencyList); 

    // NEXT, TRAVERSE THE ADJACENCY LIST AND PRINT EACH EDGE, REPRESENTED AS A PAIR OF NODES
    // Example of traversing the adjacency list is in the freeAdjList() function in graphutils.h
    for (size_t source = 0; source < graphSize; source++) 
    {
        AdjacencyListNode* dest = adjacencyList[source].next; 
        while (dest) 
        {
            // prints source node plus linked node 
            printf("%lu %lu\n ", source, dest->graphNode); 
            dest = dest->next;
        }
    }

    // NOW, BE SURE TO FREE THE ADJACENCY LIST
    freeAdjList(graphSize, adjacencyList); 
    return EXIT_SUCCESS;
}
