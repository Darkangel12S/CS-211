#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) 
{
    FILE* fp = fopen(argv[1], "r");
    if (!fp) 
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // first, read the minuend (number to be subtracted from)
    char buff; 
    bool minuend[8]; 
    for (int i = 7; i >= 0; i--) 
    { 
        fscanf(fp, "%c", &buff);
        minuend[i] = buff - '0';
    }

    // read the \n
    fscanf(fp, "%c", &buff); 

    // second, read the subtrahend (number to subtract)
    bool subtrahend[8];
    for (int i = 7; i >= 0; i--) 
    { 
        fscanf(fp, "%c", &buff);
        subtrahend[i] = buff - '0';
    }

    // Negate the subtrahend
    for (int i = 7; i >= 0; i--) { subtrahend[i] = !subtrahend[i]; }

    // add one
    bool carry = true;
    for (int i = 0; i < 8; i++) 
    { 
        if (subtrahend[i] && carry) // if bit is 1
        {
            subtrahend[i] = !subtrahend[i]; 
            carry = true; 
        }
        else // if bit is 0
        {
            if (carry) { subtrahend[i] = !subtrahend[i]; }
            carry = false; 
        }
    } 

    // add minuend + subtrhend
    bool difference[8];
    carry = false;
    for (int i = 0; i < 8; i++)
    {
        if (carry) // if there is an extra 1
        {
            if      ((minuend[i] && !subtrahend[i]) || 
                    (!minuend[i] && subtrahend[i])     ) { difference[i] = 0; carry = true;  } // 10 or 01 + carry-> 0
            else if (!minuend[i] && !subtrahend[i]     ) { difference[i] = 1; carry = false; } // 00 + carry -> 0
            else                                         { difference[i] = 1; carry = true;  } // 11 -> 0 + carry 
        }
        else // no extra 1
        {
            if      ((minuend[i] && !subtrahend[i]) || 
                    (!minuend[i] && subtrahend[i])     ) { difference[i] = 1;               } // 10 or 01 -> 1
            else if (!minuend[i] && !subtrahend[i]     ) { difference[i] = 0;               } // 00 -> 0
            else                                         { difference[i] = 0; carry = true; } // 11 -> 0 + carry 
        }
    }

    // print the difference bit string
    for (int i = 7; i >= 0; i--) { printf("%d",difference[i]); }
    fclose(fp);
    return EXIT_SUCCESS;

}
