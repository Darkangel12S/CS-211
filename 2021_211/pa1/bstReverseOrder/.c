#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct bstNode bstNode;
struct bstNode 
{
    int val;
    struct bstNode* l_child;
    struct bstNode* r_child;
};
// node insert method
struct bstNode* insert(struct bstNode* root, int key)
{
    if (root == NULL)
    {
        root = malloc(sizeof(bstNode));
        root->l_child = NULL;
        root->r_child = NULL;
        root->val = key;
    }
    if      (key < root->val) { root->l_child = insert(root->l_child, key); }
    else if (key > root->val) { root->r_child = insert(root->r_child, key); }
    else                      {                                             } 
    return root; 
}
// node delete method
void delete (bstNode* root) 
{
    // recursive to delete the root and all its children
    if (root->r_child != NULL) { delete(root->r_child); }
    if (root->l_child != NULL) { delete(root->l_child); }
    free(root); 
}

// stack
struct element 
{
    struct bstNode* node;
    struct element* next;
};
struct element *topElement = NULL; // set stack to null  
// stack push
void push(bstNode* currNode)
{
   struct element *current;
   current = (struct element*)malloc(sizeof(struct element));
   current->node = currNode; 
   if   (topElement == NULL) { current->next = NULL;       } // if there is nothing in the stack, make current element the first
   else                      { current->next = topElement; } // if there is stuff in the stack, make current the first
   topElement = current;
} 
struct bstNode* pop() 
{              
    if   (topElement == NULL) { return NULL; } 
    else
    {
        struct element *poppedElement = topElement;
        topElement = poppedElement->next;
        struct bstNode* tempNode = poppedElement->node;
        free(poppedElement);
        return tempNode; 
        //ERROR
        // returning struct element instead of struct bst node
    } 
}

int main(int argc, char* argv[])
{
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    struct bstNode* root = NULL;

    char buff[256];
    while (fscanf(fp, "%s", buff) != EOF) 
    {
        root = insert(root, atoi(buff)); 
        // >>> TEST <<< 
        printf("%d ", atoi(buff));
    }

    // main alg
    // declare array of size length
    // NOTE length of tree != length of array because duplicates arent counted

    // while loop - conditions unknown
        // 1 if node has a right child, push home node in stack and continue right
        // 2 store rightmost node value in the array, and delete the node
        // 3 pop stack and go to left child, store the pop in the array
        // 4 if no left child, pop stack again, and store in array
        // 5 if there is a left child, go to the rightmost node of that child and store&delete
        // if node has no children, store node and delete 
        // repeat the process until there
    struct bstNode* currentBSTNode = root; 
    int currentVal, previousVal;
    do // do while or while
    {
        currentVal = currentBSTNode->val;
        // if there is right element
        if (currentBSTNode->r_child != NULL)
        {
            // push the visited node in the stack and go to right child
            push(currentBSTNode);
            previousVal = currentVal;
            currentBSTNode = currentBSTNode->r_child;
            continue; 
        }
        // if there are no right or left elements
        if (currentBSTNode->r_child == NULL && currentBSTNode->l_child == NULL)
        {
            // ONLY prints if the current value is less than the previous printed value
            if (currentVal < previousVal) 
            {
                printf("%d ", currentBSTNode->val); 
                previousVal = currentVal;
            }
            delete(currentBSTNode);
            currentBSTNode = pop(); 
        }
        // if only left element exists, and no right element
        if (currentBSTNode->l_child != NULL && currentBSTNode->r_child == NULL)
        {
            // ONLY prints if the current value is less than the previous printed value
            if (currentVal < previousVal)
            {
                printf("%d ", currentBSTNode->val);
                previousVal = currentVal; 
            }
            currentBSTNode = currentBSTNode->l_child;
        }
    } while (currentBSTNode != NULL); 

    fclose(fp);

    return 0;
}
