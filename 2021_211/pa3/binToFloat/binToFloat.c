#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define EXP_SZ 8
#define FRAC_SZ 23

int main(int argc, char *argv[]) 
{

    FILE* fp = fopen(argv[1], "r");
    if (!fp) 
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // https://www.h-schmidt.net/FloatConverter/IEEE754.html
    // https://www.youtube.com/watch?v=LXF-wcoeT0o
    // read the file and store it in array[32]
    char buff;
    int binary[32];
    for (int i = EXP_SZ+FRAC_SZ; i >= 0; i--) 
    {
        fscanf(fp, "%c", &buff);
        binary[i] = buff - '0';
    }

    // set the first element in the array to the sign
    bool sign = binary[31];

    // convert array[30] - array[23] to binary
    int e = 0, j = 7;
    for (int i = 30; i >= 23 ; i--)
    {
        e += binary[i] * ((int) pow(2, j));
        j--; 
    }
    // E = # - 127
    e -= 127; 

    // convert array[22] - array[0] to a decimal binary
    double m = 0; j = -1; 
    for (int i = 22; i >= 0; i--)
    {
        m += (double) binary[i] * pow(2, j);
        j--; 
    }
    // add 1 to the decimal
    m += 1; 

    // https://www.tutorialspoint.com/c_standard_library/c_function_ldexp.htm
    double number = ldexp (m, e);
    number = sign ? -number : number;
    printf("%e\n", number);
    fclose(fp);
    return EXIT_SUCCESS;
}
