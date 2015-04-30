//
//  main.c
//  matrices
//
//  Created by Jose Carranza on 29/04/15.
//  Copyright (c) 2015 Jose Carranza. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "FileIO.h"
#include "matrix.h"

#define NUMPARAMS 2

int *m1,*m2,*m3;
int m1r, m1c, m2r,m2c;              /* # of rows and columns for both matrices*/
int m3r, m3c;
int numThreads;                                           /* Number of thread */

int Multiply(void *arg);

int main(int argc, const char * argv[]) {
    FILE   *fp;                                        /* Pointer to the file */
    
   
    int code;                                /* Return code from the function */
    
    if (argc < NUMPARAMS){
        printf("Need a file with the test data\n");
        printf("Abnormal termination\n");
        exit (EXIT_FAILURE);
    } else {
        fp = fopen (argv[2],"r");             /* Open file for read operation */
        if (!fp) {                                       /* There is an error */
            printf ("The filename: %s does not exist or is corrupted\n",
                    argv[1]);
            printf ("Abnormal termination\n");
            exit (EXIT_FAILURE);                     /* Terminate the program */
        } else {
                                   /* Read rows and columns for both matrixes */
            m1r = GetInt(fp);
            m1c = GetInt(fp);
            m2r = GetInt(fp);
            m2c = GetInt(fp);
            
#ifdef DEBUG
            printf("Matrix 1 rows   : %d\nMatrix 1 columns: %d\n",m1r,m1c);
            printf("Matrix 2 rows   : %d\nMatrix 2 columns: %d\n",m2r,m2c);
#endif
                       /* Check if matrixes are compatible for multiplication */
            if (m1c != m2r) {
                printf("Matrixes can't be multiplied...");
                exit(EXIT_FAILURE);
            }
            m3r = m1r;
            m3c = m2c;
            numThreads = m1r;
            
            /* Populate matrixes */
            m1 = malloc(sizeof(int)*m1r*m1c);
            m2 = malloc(sizeof(int)*m2r*m2c);
            m3 = malloc(sizeof(int)*m3r*m3c);
            
            for (int i = 0; i < m1r; i++) {
                for (int j = 0; j < m1c; j++) {
                    *(m1+i*m1c+j) =GetInt(fp);
                    int x = *(m1+i*m1c+j);
                }
            }
            for (int i = 0; i < m2r; i++) {
                for (int j = 0; j < m2c; j++) {
                    *(m2+i*m2c+j) =GetInt(fp);
                }
            }
#ifdef DEBUG
            PrintMatrix(m1, m1r, m1c);
            PrintMatrix(m2, m2r, m2c);
#endif
            /* Create threads */
            int tNum[m3r];
            pthread_t tid[m3r];

            for(int i=0; i< m3r; i++) {
                tNum[i] = i;
                code = pthread_create(&tid[i], NULL,(void *) Multiply,
                                      &tNum[i]);
                if (code != 0)
                    printf("Failure when creating thread a %d\n", code);
            }
            for(int i=0; i < numThreads; i++)
            {
                code = pthread_join(tid[i], NULL);   /* Wait for thread cont */
                if (code != 0)
                    printf("Failure joining thread a %d\n", i);
            }
#ifdef DEBUG
            PrintMatrix(m3, m3r, m3c);
#endif
        }
    }
    printf("Done. Goodbye. \n");
    return EXIT_SUCCESS;
}

int Multiply(void *arg){
    int *p = (int *) arg;
    int threadNum = *p;
    Multiplication(m1, m2 ,m1r, m1c, m2c, &m3, threadNum);
    return EXIT_SUCCESS;
}
