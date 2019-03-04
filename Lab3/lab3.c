#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define MAX 20      // maximum size of text file matrix is 20x20

// global variable declaration
char filename[8] = "test.txt";
int matrix[MAX][MAX];
int n;
sem_t semaphore;


// function declarations
void readMatrix(int result[MAX][MAX], const char * filename);
void printMatrix(void);




// main function
void main(void){
    int i=0, j=0;
    
    readMatrix(matrix,filename);
    printMatrix();
    sem_init(semaphore,0,1);
    pthread_t thread_id[n];
    
    pthread_create(&thread_id[i],);      // INCOMPLETE

}

void *shearsort(int index){
    
    for(int phase = 1; phase<=ceil(log2(n+1));phase++){        // even phases col sort, odd phases row sort
        if(phase%2 == 1){  // odd => row sort
            if (index%2 == 0){
                //fwd bubble
            }else{
                //back bubble
            }
        }else{              // even => col sort
            //down bubble
        }
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
} 