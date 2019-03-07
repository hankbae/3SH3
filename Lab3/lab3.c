#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#define MAX 20      // maximum size of text file matrix is 20x20

// global variable declaration
char filename[10] = "input.txt";
int matrix[MAX][MAX];
int n;
int thread_num = 0;
sem_t semaphore;
// pthread_cond_t condition;   // only go to next phase after all threads are complete same phase
// pthread_mutex_t mutex;




// function declarations
void readMatrix(int result[MAX][MAX], const char * filename);
void printMatrix(void);
void *shearSort(void * i);




// main function
int main(void){
    
    int rc,i;

    readMatrix(matrix,filename);
    printMatrix();
    sem_init(&semaphore,0,1);
    pthread_t thread_id[n];

    // pthread_cond_init(&condition,NULL);
    // pthread_mutex_init(&mutex,NULL);

    for(i = 0; i<n;i++){    // thread init
        rc = pthread_create(&thread_id[i],NULL,shearSort,(void*)i);
        if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(0);
       }
    }

    pthread_exit(NULL);
    // pthread_mutex_destroy(&mutex);
    return(0);
}

void *shearSort(void * arg){
    int index;
    index = (int) arg;
    int phase,i,j;

    
    
    for(phase = 1; phase<=ceil(log2(n^2))+2;phase++){        // even phases col sort, odd phases row sort
        //pthread_mutex_lock(&mutex);
        sem_wait(&semaphore);
        if(phase%2 == 1){  // odd => row sort
            if (index%2 == 0){
                //fwd bubble
                for(i = n-2; i>=0;i--){
                    for(j = 0; j<=i;j++){
                        if(matrix[index][j] > matrix[index][j+1]){ // comparison
                            int tmp = matrix[index][j+1];
                            matrix[index][j+1] = matrix[index][j];
                            matrix[index][j] = tmp;
                        }
                    }
                }

            }else{
                //back bubble
                for(i = n-2; i>=0;i--){
                    for(j = 0; j<=i;j++){
                        if(matrix[index][j] < matrix[index][j+1]){ // comparison
                            int tmp = matrix[index][j+1];
                            matrix[index][j+1] = matrix[index][j];
                            matrix[index][j] = tmp;
                            
                        }
                    }
                }
            }
        }else{              // even => col sort
            //down bubble
            for(i = n-2; i>=0;i--){
                    for(j = 0; j<=i;j++){
                        if(matrix[j][index] > matrix[j+1][index]){ // comparison
                            int tmp = matrix[j+1][index];
                            matrix[j+1][index] = matrix[j][index];
                            matrix[j][index] = tmp;
                        }
                    }
                }
        }
        // thread_num++;
        // if (thread_num >= n){
        //     thread_num = 0;
        //     pthread_cond_broadcast(&condition);
        // }else{
        //     pthread_cond_wait(&condition,&mutex);
        // }

        printf("phase %d | index %d\n",phase,index);
        printMatrix();
        // pthread_mutex_unlock(&mutex);
        sem_post(&semaphore);
    }
    pthread_exit(NULL);
}

void readMatrix(int result[MAX][MAX], const char * filename){
    // takes in an empty matrix and file name.
    // the values in the file are filled out into the empty matrix
    // the number of rows is returned 
    FILE * fp;
    char newLine;
    int i=0,j=0,val;

    fp = fopen(filename,"r");
    if (fp == NULL){
        printf("read error");
        exit(0);
    }
    
    while (1){

        fscanf(fp,"%d",&val);
        // printf("j=%d val=%d\n",j,val);
        result[i][j] = val;
        j++;

        newLine = fgetc(fp);
        if ((val == EOF)||(newLine == EOF)){
            break;
        }else if(newLine == '\n'){
            // printf("i=%d\n\n",i);
            i++;
            j=0;
        }
        
    }
    fclose(fp);
    n=j;
}

void printMatrix(void){
    // displays matrix
    int i=0, j=0;

    while(i!=n){
        while(j!=n){
            printf("%d ",matrix[i][j]);
            j++;
        }
        i++;
        j=0;
        printf("\n");
    }

    printf("\n\n");
} 