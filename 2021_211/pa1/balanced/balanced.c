#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
//#include <curses.h>

struct element 
{
    char close;
    struct element* next;
};

struct element *topElement = NULL; // set stack to null  

// puts a new element in front of the ll
void push(int symbol)
{
   struct element *current;
   current = (struct element*)malloc(sizeof(struct element));
   current->close = symbol;
   if   (topElement == NULL) { current->next = NULL;       } // if there is nothing in the stack, make current element the first
   else                      { current->next = topElement; } // if there is stuff in the stack, make current the first
   topElement  = current;
}

char pop()
{
    if   (topElement == NULL) { return 'x'; }
    else
    {
        struct element *poppedElement = topElement;
        char symbol = poppedElement->close;
        topElement = poppedElement->next;
        free(poppedElement);
        return symbol; 
    } 
}

int main(int argc, char* argv[])
{

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    char buff;
    int length = 0; 
    // obtain the length of the given string
    while (fscanf(fp, "%c", &buff) == 1) 
    {
        length++; 
    } fclose(fp);

    // if the final number of elements is odd, print no and return 0
    if ((length / 2) == 1) { printf("no");  return 0; }

    // reopen the file now we know the length of the string to make
    FILE* fp2 = fopen(argv[1], "r");
    if (!fp2) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }
    // create an array with all the characters
    char expression[length];
    int i = 0; 
    while (fscanf(fp2, "%c", &buff) == 1) 
    {
        expression[i] = buff; 
        i++; 
    } 

    //loop
    char popped; 
    for (int i = 0; i < length; i++) // loops through every element of the array
    {
        // if it is an open bracket, push in stack
        if (expression[i] == '<' || expression[i] == '(' || expression[i] == '[' || expression[i] == '{')
        {
            push(expression[i]); 
        }
        else // if it is a closed bracket
        {
            popped = pop();
            // empty stack - more close brackets than open brackets - (()))
            if (popped == 'x') { printf("no"); return 0; }
            // if brackets are matching
            if      (popped == '<' && expression[i] == '>') { continue;               }
            else if (popped == '(' && expression[i] == ')') { continue;               }
            else if (popped == '[' && expression[i] == ']') { continue;               }
            else if (popped == '{' && expression[i] == '}') { continue;               }
            // if brackets aren't matching ([})
            else                                            { printf("no"); return 0; }
        }
    }
    
    // if theres nothing left in the stack, then its balanced
    if   (pop() == 'x') { printf("yes"); } 
    // if there is still stuff left in the stack - more open brackets than closed brackets - ((())
    else                { printf("no");  }
    fclose(fp2);
    return 0;
}
