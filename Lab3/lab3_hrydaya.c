#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

sem_t s;
int data [4][4];
int counter=0;
int phase=0;

void hello (){
	printf("HELO \n");
}

void fwdsort(int r)
{	
	counter++;
	int i,j;
	for (i=0; i<4;i++){
		for (j=0;j<3;j++)
			if (data[r][j]>data[r][j+1])
			{
				int temp=data[r][j];
				data[r][j]=data[r][j+1];
				data[r][j+1]=temp;
			}
	}
	return;
}

void bkdsort(int r)
{
	counter++;
	int i,j;
	for (i=0; i<4;i++){
		for (j=0;j<3;j++)
			if (data[r][j]<data[r][j+1])
			{
				int temp=data[r][j];
				data[r][j]=data[r][j+1];
				data[r][j+1]=temp;
			}
	}
	return;
}

void colfwdsort(int r)
{
	counter++;
	int i,j;
	for (i=0; i<4;i++){
		for (j=0;j<3;j++)
			if (data[j][r]>data[j+1][r])
			{
				int temp=data[j][r];
				data[j][r]=data[j+1][r];
				data[j+1][r]=temp;
			}
	}
	return;
}

void printarr()
{
	int i,j,k;
	k=0;
	for (i=0;i<4;i++)
	{
		printf("\n");	
		for (j=0;j<4;j++)
		{
			k++;
			printf("%d \t", data[i][j]);
		}
	}
	printf("\n \n");
}

void todo(int r)
{
	printf("thread %d | phase %d",r,phase);
		printarr();		
		
		if (phase%2==0)
		{
			if (r%2==0)
				fwdsort(r);
			else
				bkdsort(r);
		}
		else
			colfwdsort(r);
		
		if (counter%4==0)
			phase++;
		
		
		printf("%d \n ",counter);
}

void *strtHere(void *arg){

	int r = (int) arg;	
	while (phase!=16)
	{
	sem_wait(&s);
		todo(r);
	sem_post(&s);
	}

}

int main(){
	int i,j;
	sem_init(&s,0,1);	
	
	FILE *fp;
	fp=fopen("input.txt","r");
	
	for (i=0;i<4;i++)
		for (j=0;j<4;j++)
			fscanf(fp,"%d",&data[i][j]);

	pthread_t a,b,c,d;
	
	
	pthread_create(&a, NULL, strtHere,(void *)0);
	pthread_create(&b, NULL, strtHere,(void *)1);
	pthread_create(&c, NULL, strtHere,(void *)2);
	pthread_create(&d, NULL, strtHere,(void *)3);
	pthread_join(a,NULL);
	pthread_join(b,NULL);
	pthread_join(c,NULL);
	pthread_join(d,NULL);	
	
	printf("\n \n");
}
