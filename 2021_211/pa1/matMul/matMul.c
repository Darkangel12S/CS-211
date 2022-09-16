#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    // read first matrix
    FILE* matrix_a_fp = fopen(argv[1], "r");
    if (!matrix_a_fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }
    // changed char, unsigned char to int, unsigned int
    char buff[256];
    // find L in an LxM matrix
    fscanf(matrix_a_fp, "%s", buff);
    int length_l = atoi(buff);
    int** matrix_a = malloc(length_l * sizeof(int*));
    // find M in an LxM matrix
    fscanf(matrix_a_fp, "%s", buff);
    int length_m = atoi(buff);
    for (unsigned int i = 0; i < length_l; i++ ) 
    {
        matrix_a[i] = malloc(length_m * sizeof(int));
    }
    // set 2d matrix A
    // read all the items in the matrix and store it in 2d array
    int x = 0, i = 0, j = 0; 
    char ch;
    // https://stackoverflow.com/questions/20721245/how-fscanf-know-this-is-the-end-of-the-line
    while (fscanf(matrix_a_fp, "%s%c", buff, &ch) == 2)
    {
        if (i == length_m) { i = 0; j += 1; } // cheese way of finding if there is new line
        x = atoi(buff); 
        matrix_a[j][i] = x; 
        i += 1;  
    }

    // read second matrix
    FILE* matrix_b_fp = fopen(argv[2], "r");
    if (!matrix_b_fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }
    // find M in an MxN matrix
    fscanf(matrix_b_fp, "%s", buff);
    length_m = atoi(buff);
    int** matrix_b = malloc(length_m * sizeof(int*));
    // find N in an LxM matrix
    fscanf(matrix_b_fp, "%s", buff);
    int length_n = atoi(buff);
    for (unsigned int i = 0; i < length_m; i++ ) 
    {
        matrix_b[i] = malloc(length_n * sizeof(int));
    }
    // set 2d matrix B
    // store values in a 2d array for both matrix
    x = 0; i = 0; j = 0;
    while (fscanf(matrix_b_fp, "%s%c", buff, &ch) == 2)
    {
        if (i == length_n) { i = 0; j += 1; }
        x = atoi(buff);
        matrix_b[j][i] = x; 
        i += 1;
    }

    // malloc for the 3rd matrix LxN
    int** matrix_c = malloc(length_l * sizeof(int*));
    for (unsigned int i = 0; i < length_l; i++) 
    {
        matrix_c[i] = malloc(length_n * sizeof(int));
    }
    for (int i = 0; i < length_l; i++) // set all to 0
    {
        for (int j = 0; j < length_n; j++)
        {
            matrix_c[i][j] = 0;
        }
    }
    // alg to multiply both matrix and store it in matrix_c
    for (int i = 0; i < length_l; i++)
    {
        for (int j = 0; j < length_n; j++)
        {
            for (int k = 0; k < length_m; k++)
            {
                matrix_c[i][j] += (matrix_a[i][k] * matrix_b[k][j]);
            }
        }
    }

    // print 2d matrix
    for (int i = 0; i < length_l; i++) 
    {
        for (int j = 0; j < length_n; j++) 
        {
            printf("%d ", matrix_c[i][j]);
        } printf("\n");
    }

    // close both files and free all malloc
    fclose(matrix_a_fp);
    for (unsigned char i = 0; i < length_l; i++) { free( matrix_a[i]); }
    free( matrix_a );
    fclose(matrix_b_fp);
    for (unsigned char i = 0; i < length_m; i++) { free( matrix_b[i]); }
    free( matrix_b);
    for (unsigned char i = 0; i < length_l; i++) { free( matrix_c[i]); }
    free( matrix_c);
    return 0;

}
