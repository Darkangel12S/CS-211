#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]) 
{

    FILE* fp = fopen(argv[1], "r");
    if (!fp) 
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // first, read the number
    signed short int input;
    fscanf(fp, "%hd", &input);

    // store the bit in an array[16]
    int array[16]; 
    for (int i = 15; i >= 0; i--) 
    {
        bool char_val = 0b1 & input>>i; // shift and mask
        array[i] = char_val;
    }

    // splits array into series of 4 bits for ez conversion
    for (int i = 15; i >= 0; i-= 3)
    {
        // store the 4 bits in a temp array
        int hex[4];
        for (int j = 0; j < 4; j++)
        { hex[j] = array[i-j]; } 
        // convert the 4 bits into a hex int print the hex representation
        int value = 0, k = 3; 
        for (int j = 0; j < 4; j++)
        {   
            value += (int) (hex[j] * (int) pow(2,k)); 
            k--; 
        }
        if (value < 10) { printf("%d", value); }
        else
        {
            if (value == 10) { printf("A"); }
            if (value == 11) { printf("B"); }
            if (value == 12) { printf("C"); }
            if (value == 13) { printf("D"); }
            if (value == 14) { printf("E"); }
            if (value == 15) { printf("F"); }
        }
        // goes to the the beginning of the next 4 bits
        i--;
    }
    fclose(fp);
    return EXIT_SUCCESS;

}
