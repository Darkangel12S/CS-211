#include "../graphutils.h"

// A program to find a cycle in a directed graph

// You may use DFS or BFS as needed
typedef struct QueueNode 
{
    graphNode_t graphNode;
    struct QueueNode* next;
} QueueNode;

struct Queue 
{
    struct QueueNode* front; // front (head) of the queue
    struct QueueNode* back; // back (tail) of the queue
};
typedef struct Queue Queue;

// Append a new QueueNode to the back of the Queue
struct Queue* enqueue(struct Queue* queue, graphNode_t graphNode) 
{
    QueueNode* queueNode = malloc(sizeof(QueueNode));
    queueNode->graphNode = graphNode; 
    queueNode->next = NULL; 
    if (queue->back == NULL && queue->front == NULL) // if q is empty, node is both the front and the back
    {
        queue->front = queueNode; 
        queue->back = queueNode; 
    }
    else // if q is not empty, append to the back
    {
        (queue->back)->next = queueNode;
        queue->back = queueNode; 
    }
    return queue;
}

// Remove a QueueNode from the front of the Queue
graphNode_t dequeue(Queue* queue) // dequeue is not freeing properly
{
    // returns -1 if nothing left in queue
    if (queue->front == NULL) { return -1; } 
    else
    {
        QueueNode* temp = queue->front; 
        if (queue->back == temp) { queue->back = NULL; }
        queue->front = temp->next; 

        graphNode_t value = temp->graphNode;
        free(temp); 
        return value;  
    }
}

int main (int argc, char* argv[]) 
{
    // READ INPUT FILE TO CREATE GRAPH ADJACENCY LIST
    AdjacencyListNode* adjacencyList = NULL; 
    unsigned long int graphSize = adjMatrixToList(argv[1], &adjacencyList); 

    // queue for bfs
    Queue* queue = (struct Queue*)malloc(sizeof(struct Queue)); 
    queue->front = queue->back = NULL; 

    bool isCyclic = false;
    size_t temp = 0, tempSource = 0;

    // An array that keeps track of who is the parent node of each graph node we visit
    size_t parents[graphSize]; 

    // loops through all nodes of graph
    for (unsigned source = 0; source < graphSize; source++) 
    {
        // clear parents array 
        for (size_t i = 0; i < graphSize; i++) 
        {
            parents[i] = -1; // -1 indicates that a nodes is not yet visited
        }
        // set the parents[source] to -2
        parents[source] = -2; 
        // empty the queue
        while (queue->front != NULL) 
        {
            dequeue(queue); 
        }

        // enqueue the source node
        queue = enqueue(queue, source);
        // do a BFS on the source node
        while(queue->front != NULL)
        { 
            graphNode_t current = dequeue(queue);
            AdjacencyListNode* neighbor = adjacencyList[current].next;
            while (neighbor)
            {
                if (parents[neighbor->graphNode] == -1) // if not visited
                {
                    queue = enqueue(queue, neighbor->graphNode);
                    parents[neighbor->graphNode] = current;
                }
                // if the current node is the source node -> cycle starts and ends on source
                if (neighbor->graphNode == source)
                {
                    // we found a cycle, note to TA- i used goto here b/c i feel like this is the 
                    //                               simplist way to terminate the nested loops 
                    //                               without using multiple break statements
                    temp = current; 
                    tempSource = source;
                    isCyclic = true; 
                    goto end; 
                }
                neighbor = neighbor->next;
            } 
        }
        // if we reach here, it means no cycle was found that starts and ends on source
    }

    end: 
    if (isCyclic)
    {
        // print the source first
        printf("%ld ", tempSource);
        // prints the parents in reverse order
        size_t arraySize = 0; 
        for (size_t i = temp; parents[i] != -2; i = parents[i]) { arraySize++; } 
        size_t reversedParent[arraySize], j = arraySize;
        for (size_t i = temp; parents[i] != -2; i = parents[i])
        {
            reversedParent[j-1] = i; 
            j--;
        }
        for (size_t i = 0; i < arraySize; i++)
        {
            printf("%ld ", reversedParent[i]);
        }
    }
    else { printf("DAG"); } 

    // free stuff
    while (queue->front != NULL) 
    {
        dequeue(queue); 
    }
    free(queue); 
    freeAdjList(graphSize, adjacencyList);
    return EXIT_SUCCESS;
}
