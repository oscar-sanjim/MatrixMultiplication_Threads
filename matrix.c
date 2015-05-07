/*
 * File name: matrix.c
 *
 * Authors:  Oscar Sanchez
 *           Jose Carranza
 *           Jacob Rivera
 */

#include "matrix.h"

                                                         /* Print the matrix */
void PrintMatrix(int *mat,int r, int c)
{
    int i, j;
    int *matrix = (int*) mat;
    
    for(i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            printf("%4d ", *(matrix+(i*c)+j));
        }
        printf("\n");
    }
    printf("\n");
}
                                               /* Multiply a row of a matrix */
void Multiplication (int *m1, int *m2, int m1r, int m1c,
                     int m2c, int **m3, int threadNum,
                     pthread_mutex_t *key, pthread_cond_t *done) {
    int m2r = m1c;
                          /* First loop goes through the elements in the row */
    for(int i=0; i<m1r;i++)
    {
        int total = 0;
                                        /* Second loop makes the calculation */
        for(int j=0;j<m2r;j++)
        {
            total += *(m1+(m1c*threadNum)+j)*(*(m2+(i)+(j*m2c)));
        }
                       /* Lock the matrix, and assign the value to the space */
        pthread_mutex_lock(key);
        *(*m3+(threadNum*m1r)+i) = total;
        pthread_cond_signal(done);
        pthread_mutex_unlock(key);
    }
}

                                    /* Initialize the diagonal to contain -1 */
void InitDiagonal(int **mat, int r, int c) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if(i == j)
                *(*mat+(c*i)+j) = -1;
            else
                *(*mat+(c*i)+j) = 0;
        }
    }
}



