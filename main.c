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
#include <time.h>

#define NUMPARAMS 2

int *m1,*m2,*m3;
int m1r, m1c, m2r,m2c;              /* # of rows and columns for both matrices*/
int m3r, m3c;
int numThreads;
int theChosenOne;


double diagonalSum = 0;
int Multiply(void *arg);

struct condition {
    pthread_mutex_t key;                          /* Synchronization variable */
    pthread_cond_t done;       /* Signal that thread is done with calculations*/
} cond;

int main(int argc, const char * argv[]) {
    
    FILE   *fp;                                        /* Pointer to the file */
    
   
    int code;                                /* Return code from the function */
    
    if (argc < NUMPARAMS){
        printf("Need a file with the test data\n");
        printf("Abnormal termination\n");
        exit (EXIT_FAILURE);
    } else {
        fp = fopen (argv[1],"r");             /* Open file for read operation */
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
            printf("Matrix 1 rows   : %d\nMatrix 1 columns: %d\n", m1r, m1c);
            printf("Matrix 2 rows   : %d\nMatrix 2 columns: %d\n", m2r, m2c);
#endif
                       /* Check if matrixes are compatible for multiplication */
            if (m1c != m2r) {
                printf("Matrixes can't be multiplied...");
                exit(EXIT_FAILURE);
            }
            m3r = m1r;
            m3c = m2c;
            numThreads = m1r;
            
            
            srand(time(NULL));
            
            theChosenOne = rand();
#ifdef DEBUG
            printf("Elected thread: %d\n", theChosenOne);
#endif
            
            /* Populate matrixes */
            m1 = malloc(sizeof(int)*m1r*m1c);
            m2 = malloc(sizeof(int)*m2r*m2c);
            m3 = malloc(sizeof(int)*m3r*m3c);
            InitDiagonal(&m3, m3r, m3c);
            
#ifdef DEBUG
            printf("\nResulting Matrix initialized: \n");
            PrintMatrix(m3, m3r, m3c);
#endif
            for (int i = 0; i < m1r; i++) {
                for (int j = 0; j < m1c; j++) {
                    *(m1+i*m1c+j) =GetInt(fp);
                }
            }
            for (int i = 0; i < m2r; i++) {
                for (int j = 0; j < m2c; j++) {
                    *(m2+i*m2c+j) =GetInt(fp);
                }
            }
            fclose(fp);
#ifdef DEBUG
            printf("\nMatrix A: \n");
            PrintMatrix(m1, m1r, m1c);
            printf("\nMatrix B: \n");
            PrintMatrix(m2, m2r, m2c);
#endif
            
            /* Init syncronization variables */
            pthread_mutex_init(&cond.key, NULL);
            pthread_cond_init(&cond.done, NULL);
            
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
                code = pthread_join(tid[i], NULL);    /* Wait for thread cont */
                if (code != 0)
                    printf("Failure joining thread a %d\n", i);
            }
      
            printf("Result: \n");
            PrintMatrix(m3, m3r, m3c);
            printf("The sum of the diagonal is: %f\n",diagonalSum);
            
            pthread_cond_destroy(&cond.done);
            free(m1);
            free(m2);
            free(m3);
        }
    }
#ifdef DEBUG
    printf("Done. Goodbye. \n");
#endif
    return EXIT_SUCCESS;
}

int Multiply(void *arg){
    /* Initialize Thread Conditional variables */
    
    int *p = (int *) arg;
    int threadNum = *p;
    Multiplication(m1, m2 ,m1r, m1c, m2c, &m3, threadNum,
                   &cond.key, &cond.done);
#ifdef DEBUG
    printf("Thread %d finished multiplication\n",threadNum);
#endif
    if(threadNum == theChosenOne){
        pthread_mutex_lock(&cond.key);
#ifdef DEBUG
        printf("Thread %d waiting for diagonal values!\n",threadNum);
#endif
        int select = m3c;
        if (m3r < m3c) select = m3r;
        for (int i = 0; i < select; i++) {
            while(*(m3+(i*select)+i) == -1){
#ifdef DEBUG
                printf("Blocked\n");
#endif
                pthread_cond_wait(&cond.done, &cond.key);
            }
            printf("Adding: %d\n",*(m3+(i*select)+i));
            diagonalSum += *(m3+(i*select)+i);
        }

       
#ifdef DEBUG
        printf("Diagonal sum = %f\n",diagonalSum);
#endif
        pthread_mutex_unlock(&cond.key);
    }
    
#ifdef DEBUG
    printf("Thread: %d returning...\n",threadNum);
#endif
    return EXIT_SUCCESS;
}
