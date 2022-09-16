#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define EXP_SZ 11
#define FRAC_SZ 52

int main(int argc, char *argv[]) 
{
    // SETUP
    FILE* fp = fopen(argv[1], "r");
    if (!fp) 
    {
        perror("fopen failed");
        return 0;
    }

    // first, read the number
    double number;
    fscanf(fp, "%lf", &number);

    // https://www.youtube.com/watch?v=WME1IkIHewU
    // https://www.binaryconvert.com/convert_double.html
    bool sign = number < 0.0;
    printf("%d", sign); 

    if (sign) { number *= -1; }

    int E = 0; 
    // if number >= 2, trial division go positive
    double tempValue = number; 
    if (number >= 2)
    {
        do
        {
            E++; 
            tempValue = number / pow(2, E);
        } while (tempValue >= 2);
    }
    // if number < 1, trial division go negative
    bool denorm = false;
    if (number < 1 && number > 0)
    {
        do 
        {
            E--; 
            tempValue = number / pow(2, E);
            if (E == -1023) 
            {
                denorm = true; 
                break; 
            }
        } while (tempValue < 1);
    }
    // if number is zero, E must be zero
    if (number == 0)
    {
        E = -1023;
        denorm = false; 
    }
    printf("_");

    // E = 2^(11-1)-1 + power
    E += 1023;
    
    //convert that into binary with profs bit stuff
    for (int exp_index = EXP_SZ-1; 0 <= exp_index; exp_index--) 
    {
        bool exp_bit = 1&E>>exp_index;
        printf("%d", exp_bit);
    }
    printf("_");

    if (denorm) { tempValue /= 2; }

    // M
    // x -= 1
    double M = tempValue; 
    if (!denorm) // if denorm do the thing that the guy in piazza said to prevent bit shifting
    // https://piazza.com/class/kjw3smy31un5hy?cid=363
    { 
        M = tempValue - 1; 
    }
    int j = -1; 
    for (int i = 51; i >= 0; i--) // finds the binary representation of the decimal with 23 bits max
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

    fclose(fp); 
    return(EXIT_SUCCESS);
}
