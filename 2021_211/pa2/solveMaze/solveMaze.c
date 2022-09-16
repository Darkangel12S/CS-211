#include "../graphutils.h" // header for functions to load and free adjacencyList

// A program to solve a maze that may contain cycles using BFS

// BFS requires using a queue data structure
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

int main ( int argc, char* argv[] ) 
{

    // READ INPUT FILE TO CREATE GRAPH ADJACENCY LIST
    AdjacencyListNode* adjacencyList = NULL; 
    unsigned long int graphSize = adjMatrixToList(argv[1], &adjacencyList);

    //read the query file to get the source and target nodes in the maze
    FILE* fp = fopen(argv[2], "r");
    if (!fp) 
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }
    // reads source
    graphNode_t source, target;
    fscanf(fp, "%ld", &source);
    // reads target
    fscanf(fp, "%ld", &target);

    Queue* queue = (struct Queue*)malloc(sizeof(struct Queue)); 
    queue->front = queue->back = NULL; 

    // An array that keeps track of who is the parent node of each graph node we visit
    size_t parents[graphSize]; 
    for (size_t i = 0; i < graphSize; i++) 
    {
        parents[i] = -1; // -1 indicates that a nodes is not yet visited
    }


    graphNode_t current = source; 
    parents[current] = -2; // source is visited, and set the parents to unique # of -2

    //https://www.youtube.com/watch?v=oDqjPvD54Ss
    
    // enqueue the source
    queue = enqueue(queue, current);

    // while queue is not empty
    while(queue->front != NULL)
    {
        current = dequeue(queue);
        AdjacencyListNode* neighbor = adjacencyList[current].next; 
        while (neighbor)
        {
            if (parents[neighbor->graphNode] == -1)
            {
                queue = enqueue(queue, neighbor->graphNode);
                parents[neighbor->graphNode] = current;
            }
            neighbor = neighbor->next;
        }
    }
    // Now that we've found the target graph node, use the parent array to print maze solution
    // Print the sequence of edges that takes us from the source to the target node
    for (size_t i = target; parents[i] != -2; i = parents[i])
    {  
        printf("%ld %ld\n", i, parents[i]); 
    }

    while (queue->front != NULL) 
    {
        dequeue(queue); 
    }
    free(queue); 
    //free (parents);
    freeAdjList(graphSize, adjacencyList);
    fclose(fp);
    return EXIT_SUCCESS;
}
