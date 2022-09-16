#define  _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#define NAME_SIZE 16

/*
 *
 *  >>   NOTE TO INSTRUCTOR  << 
 *  THIS PROGRAM IS FOR EXTRA CREDIT OUT OF 40 POINTS
 *  BECAUSE OF FINALS, I AM NOT ABLE TO COMPLETE THIS CODE
 *  THE CODE USED FOR THIS IS THE CODE FOR basicGates.c
 *  IT PASSES THE FIRST TEST CASE SO I WILL GET (5/40) POINTS FOR multiInput.c
 *  AND (45/40) FOR pa6
 *  SO FINAL AVERAGE WILL BE 100.38*
 *
 */

//number of lines in the file
int numberOfLines = 0; 

// Reverse linked list node of Boolean variable and Boolean value pairs
typedef struct var 
{
    char name[NAME_SIZE];
    bool val;
    struct var* prev;
} var_t;

// PROCESS A SINGLE ROW OF CIRCUIT FILE DESCRIBING A GATE TO FIND OUTPUT NAME AND VALUE
void findOutVarNameVal(var_t* varList, char* gateLine, char* outName, bool* outVal) 
{

    // unknown if outName is used here
    char gate[5];
    char inName0[NAME_SIZE];
    char inName1[NAME_SIZE];

    if (sscanf(gateLine, "NOT %s %s\n", inName0, outName) == 2) 
    {
        // It is a NOT gate
        bool inVal = false;

        // Traverse varList to look for existing value
        var_t* curr = varList;
        while (curr) 
        {
            // https://www.tutorialspoint.com/c_standard_library/c_function_strcmp.htm
            if (strcmp(curr->name, inName0) == 0)
            {
                inVal = curr->val;
                break;
            }
            curr = curr->prev;
        }
        *outVal = !inVal;
    } 
    else if (sscanf(gateLine, "%s %s %s %s\n", gate, inName0, inName1, outName) == 4)  
    {
        // It is not a NOT gate

        bool inVal1 = false, inVal2 = false;
        // Traverse varList to look for first existing variable
        var_t* curr1 = varList;
        while (curr1) 
        {
            if (strcmp(curr1->name, inName0) == 0)
            {
                inVal1 = curr1->val;
                break;
            }
            curr1 = curr1->prev;
        }
        // Traverse varList to look for second existing variable
        var_t* curr2 = varList;
        while (curr2) 
        {
            if (strcmp(curr2->name, inName1) == 0)
            {
                inVal2 = curr2->val;
                break;
            }
            curr2 = curr2->prev;
        }
        // https://miro.medium.com/max/2400/1*4JomFrSIYLoTs_8x4ZqGNA.jpeg
        if      (strcmp("AND" , gate) == 0) { *outVal = inVal1 & inVal2;    } // AND gate
        else if (strcmp("OR"  , gate) == 0) { *outVal = inVal1 | inVal2;    } // OR gate
        else if (strcmp("NAND", gate) == 0) { *outVal = !(inVal1 & inVal2); } // NAND gate
        else if (strcmp("NOR" , gate) == 0) { *outVal = !(inVal1 | inVal2); } // NOR gate
        else if (strcmp("XOR" , gate) == 0) { *outVal = inVal1 ^ inVal2;    } // XOR gate
        else if (strcmp("XNOR", gate) == 0) { *outVal = (inVal1 == inVal2); } // XNOR gate
    } 
    else 
    {
        perror("invalid line describing gate in circuit file\n");
        exit(EXIT_FAILURE);
    }
}

// PRINT A SINGLE ROW OF THE TRUTH TABLE GIVEN INITIAL ASSIGNMENTS TO CIRCUIT INPUTS
void printTruthTableRow(FILE* circuit_fp,
    size_t circuitInputBits) // Vector of Boolean assignments for input bits
{
    // Record input variable names
    size_t circuitInputCount;
    assert(fscanf(circuit_fp, "INPUTVAR %ld ", &circuitInputCount) == 1);
    var_t* inputs = calloc(circuitInputCount, sizeof(var_t)); // We use an array of var_t here
    for (size_t i = 0; i < circuitInputCount; i++)
    {
        assert(fscanf(circuit_fp, "%s", inputs[i].name) == 1);
    }
    assert(fscanf(circuit_fp, "\n") == 0);

    // Record output variable names
    size_t circuitOutputCount;
    assert (fscanf(circuit_fp, "OUTPUTVAR %ld ", &circuitOutputCount) == 1);
    var_t* outputs = calloc(circuitOutputCount, sizeof(var_t)); // We use an array of var_t here
    for (size_t i = 0; i < circuitOutputCount; i++)
    {
        assert(fscanf(circuit_fp, "%s", outputs[i].name) == 1);
    }
    assert(fscanf(circuit_fp, "\n") == 0);

    // Reversed linked list of variable structs
    var_t* varList = NULL;
    // Initialize the list with initial assignments to input variables
    for (size_t i = 0; i < circuitInputCount; i++) 
    {
        var_t* temp = calloc(1, sizeof(var_t));
        strcpy(temp->name, inputs[i].name);
        temp->val = 0b1 & circuitInputBits>>i; // Each bit in input bits
        temp->prev = varList;
        varList = temp;
    }

    // Read the rest of the circuit file consisting of gates line by line
    char* line = NULL;
    size_t len = 0;

    // read all the dig log operation lines
    for (int i = 0; i < numberOfLines; i++)
    {
        getline(&line, &len, circuit_fp);

        char outName[NAME_SIZE];
        bool outVal=false;
        findOutVarNameVal(varList, line, outName, &outVal);

        // Record this gate's outputs
        // N+M outputs
        var_t* temp = calloc(1, sizeof(var_t));
        // https://www.tutorialspoint.com/c_standard_library/c_function_strcpy.htm
        strcpy(temp->name, outName);
        temp->val = outVal;
        temp->prev = varList;
        varList = temp;
    }

    // Print the truth table
    for (size_t i = 0; i < circuitInputCount; i++) 
    {
        var_t* curr = varList;
        while (curr) 
        {
            if (strcmp(curr->name, inputs[i].name) == 0) 
            {
                printf ("%d ", curr->val);
                break;
            }
            curr = curr->prev;
        }
    }
    for (size_t i = 0; i < circuitOutputCount; i++) 
    {
        var_t* curr = varList;
        while (curr) 
        {
            if (strcmp(curr->name, outputs[i].name) == 0) 
            {
                printf ("%d ", curr->val);
                break;
            }
            curr = curr->prev;
        }
    }
    printf("\n");

    free(line);
    while (varList) 
    {
        var_t* temp = varList;
        varList = varList->prev;
        free(temp);
    }
    free(outputs);
    free(inputs);
}


int main(int argc, char* argv[]) 
{
    if (argc!=2) 
    {
        printf("Usage: ./basicGates <circuit_file>\n");
        exit(EXIT_FAILURE);
    }

    // count number of lines in the file
    FILE* fp = fopen(argv[1], "r");
    if (!fp) 
    {
        perror("fopen failed");
        exit(EXIT_FAILURE);
    }
    char ch; 
    while ((ch=fgetc(fp)) != EOF) 
    {
      if (ch=='\n') { numberOfLines++; } 
    }
    // 
    numberOfLines -= 2;

    // Open circuit file
    FILE* circuit_fp = fopen(argv[1], "r");
    if (!circuit_fp) 
    {
        perror("fopen failed");
        exit(EXIT_FAILURE);
    }

    // Peak at the input count so we know how many rows in the truth table
    size_t circuitInputCount;
    assert(fscanf(circuit_fp, "INPUTVAR %ld ", &circuitInputCount) == 1);

    // THIS IS CURRENTLY SET UP TO PRINT JUST TWO TRUTH TABLE ROWS:
    // we need 2^n rows
    for (int i = 0; i < pow(2,circuitInputCount); i++)
    {
        // Rewind circuit file for each truth table line
        rewind(circuit_fp);
        printTruthTableRow (circuit_fp, i);
    }

    fclose(circuit_fp);
    exit(EXIT_SUCCESS);
}