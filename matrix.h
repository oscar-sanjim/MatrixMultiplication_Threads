/*
 * File name: main.c
 *
 * Authors:  Oscar Sanchez
 *           Jose Carranza
 *           Jacob Rivera
 */

#ifndef __matrices__matrix__
#define __matrices__matrix__

#include <stdio.h>
#include <pthread.h>


#endif 

void PrintMatrix(int *mat,int r, int c);
void Multiplication (int *m1, int *m2, int m1r, int m1c,
                     int m2c, int **m3, int threadNum,
                     pthread_mutex_t *key, pthread_cond_t *done);
void InitDiagonal(int **mat, int r, int c);


