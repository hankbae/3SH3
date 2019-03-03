#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX 20      // maximum size of text file matrix is 20x20

int readMatrix(int result[MAX][MAX], const char * filename){
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
    return (j);
}

void printMatrix(int matrix[MAX][MAX],int n){
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

void main(void){
    char filename[8] = "test.txt";
    int matrix[MAX][MAX];
    int i=0, j=0,n;

    n = readMatrix(matrix,filename);
    printMatrix(matrix,n);

    

}