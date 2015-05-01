//
//  matrix.c
//  matrices
//
//  Created by Jose Carranza on 29/04/15.
//  Copyright (c) 2015 Jose Carranza. All rights reserved.
//

#include "matrix.h"
void PrintMatrix(int *mat,int r, int c)
{
    int i, j;
    int *matrix = (int*) mat;
    
    for(i=0; i<r;i++)
    {
        for (j = 0; j < c; j++)
        {
            printf("%3d ", *(matrix+(i*c)+j));
        }
        printf("\n");
    }
    printf("\n");
}
void Multiplication (int *m1, int *m2, int m1r, int m1c,
                     int m2c, int **m3, int threadNum){
    int m2r = m1c;

    for(int i=0; i<m1r;i++)
    {
        if(*(*m3+(threadNum*m1r)+i) != 0)
            *(*m3+(threadNum*m1r)+i) = 0;
        for(int j=0;j<m2r;j++)
        {
            *(*m3+(threadNum*m1r)+i) += *(m1+(m1c*threadNum)+j)*
                                                    (*(m2+(i)+(j*m2c)));
            
//            if(threadNum == 0)
//            printf("%d * % d\n",*(m1+(m1c*threadNum)+j),(*(m2+(i)+(j*m2c))));
        }
    }
}

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



