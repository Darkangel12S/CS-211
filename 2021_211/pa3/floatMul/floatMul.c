#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define EXP_SZ 8
#define FRAC_SZ 23

// function that converts binary to float
float binToFloat(int binary[32])
{
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

    double number = ldexp(m,e);
    number = sign ? -number : number;
    return (float) number; 
}

void floatToBin(float number)
{
    // note: rewrite doubleToBin in this alg cos it seems to work
    
    // SIGN
    // see if the sign is positive or negative
    bool sign = number < 0.0;
    printf("%d", sign);  

    // make number positive
    if (sign) { number *= -1; }

    // E ---   x * 2^y = number AND 1 <= x < 2
    // trial division for positive 2^y
    int E = 0; 
    // if number >= 2, trial division go positive
    float tempValue = number; 
    if (number >= 2)
    {
        do
        {
            E++; 
            tempValue = number / (float) pow(2, E);
        } while (tempValue >= 2);
    }
    // if number < 1, trial division go negative
    if (number < 1 && number > 0)
    {
        do 
        {
            E--; 
            tempValue = number / (float) pow(2, E);
        } while (tempValue < 1);
    }
    // if number is zero, E must be zero
    if (number == 0)
    {
        E = -127;
    }
    printf("_");
    // once the conditions for x are met, we have found y
    // y += 127
    E += 127; 
    //convert that into binary with profs bit stuff
    for (int exp_index = EXP_SZ-1; 0 <= exp_index; exp_index--) 
    {
        bool exp_bit = 1&E>>exp_index;
        printf("%d", exp_bit);
    }

    // M
    // x -= 1
    printf("_");
    float M = tempValue - 1; 
    int j = -1; 
    for (int i = 22; i >= 0; i--) // finds the binary representation of the decimal with 23 bits max
    {
        if (M-pow(2,j) >= 0)
        {
            printf("1");
            M -= pow(2,j);
        }
        else
        {
            printf("0");
        }
        j--;
    }
}

int main(int argc, char *argv[]) 
{
    FILE* fp = fopen(argv[1], "r");
    if (!fp) 
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // first, read the binary number representation of multiplier
    char buff;
    int binaryFirst[32];
    for (int i = EXP_SZ+FRAC_SZ; i >= 0; i--) 
    {
        fscanf(fp, "%c", &buff);
        binaryFirst[i] = buff - '0';
    }

    // read the \n
    fscanf(fp, "%c", &buff); 
 
    // first, read the binary number representation of multiplcand
    int binarySecond[32];
    for (int i = EXP_SZ+FRAC_SZ; i >= 0; i--) 
    {
        fscanf(fp, "%c", &buff);
        binarySecond[i] = buff - '0';
    }

    float number1 = binToFloat(binaryFirst);
    //printf("float number 1: %f\n", number1); 
    float number2 = binToFloat(binarySecond);
    // multiply those numbers
    float product = number1 * number2;  
    // function to convert the product to binary
    floatToBin(product); 

    fclose(fp); 
    return(EXIT_SUCCESS);

}