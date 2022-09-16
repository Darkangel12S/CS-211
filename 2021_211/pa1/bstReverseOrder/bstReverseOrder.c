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
bstNode* insert(struct bstNode* root, int key)
{
    bstNode* tempNode = NULL;
    if (root == NULL)
    {
        tempNode = malloc(sizeof(bstNode));
        tempNode->l_child = NULL;
        tempNode->r_child = NULL;
        tempNode->val = key;
        root = tempNode; 
        return tempNode; 
    }
    if      (key < root->val) { root->l_child = insert(root->l_child, key); }
    else if (key > root->val) { root->r_child = insert(root->r_child, key); }
    else                      {                                             } 
    return root; 
}
void delete (bstNode* root) 
{
    // recursive to delete the root and all its children
    if (root->r_child != NULL) { delete(root->r_child); }
    if (root->l_child != NULL) { delete(root->l_child); }
    free(root);
}
void recurse(bstNode* root) 
{
  if (root != NULL) // if root is not null 
  {
    recurse(root->r_child); // recurse to right
    printf("%d ", root->val);
    recurse(root->l_child); // recurse to left
  }
}
void freeBST(struct bstNode* root) 
{
  if (root == NULL) { return; }
  else 
  {
      freeBST(root->l_child);
      freeBST(root->r_child);
      free(root); 
  }
}

int main(int argc, char* argv[])
{
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    bstNode* root = NULL; 

    int key; 
    while (fscanf(fp, "%d", &key) != EOF) 
    {
        root = insert(root,key); 
    }
    fclose(fp);
    recurse(root); 
    freeBST(root); 
    printf("\n"); 

    return 0;
}
