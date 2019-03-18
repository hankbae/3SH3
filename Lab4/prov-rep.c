#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>


char * mem;
struct stat buffer;
int newResource;
size_t size;
pid_t childpid;


int mutex;
key_t semKey;
struct sembuf wait[1];
struct sembuf sig[1];

void readfile(const char * filename)
{
    int file;
    file = open("res.txt",O_RDWR);
    if(fstat(file,&buffer)==-1)
    {
        printf("fstat error\n");
        exit(0);
    }
    size = buffer.st_size;
    mem = mmap(NULL, size, PROT_EXEC|PROT_READ|PROT_WRITE, MAP_SHARED,file, 0);
    close(file);
}

void sem_init(void)
{
    semKey = ftok("/tmp",'a');
    mutex = semget(semKey,1, 0666 | IPC_CREAT);
    semctl(mutex,0,SETVAL,1);

    
    wait[0].sem_num = 0;
    wait[0].sem_op = -1;
    wait[0].sem_flg = 0;
    sig[0].sem_num = 0;
    sig[0].sem_op = 1;
    sig[0].sem_flg = 0;
}

void sem_wait(void)
{
    if(semop(mutex,wait,1)==-1){
        printf("semop error");
        exit(-1);
    }
}

void sem_signal(void)
{
    if(semop(mutex,sig,1)==-1){
        printf("semop error");
        exit(-1);
    }
}

int main(void)
{
    

    sem_init();
    readfile("res.txt");


    if((childpid = fork()) == 0) // CHILD
    {
        int vecSize = (size+getpagesize()-1)/getpagesize();
        unsigned char vec[vecSize];
        int i;
        while(1)
        {
            sem_wait();
            
            printf("\npage size: %d\n",getpagesize());
            printf("\nresources:\n");
            for(i = 0; i<size;i++)printf("%c",mem[i]);

            mincore(mem,size,vec);
            printf("\nmincore:\n");
            for(i = 0;i<vecSize;i++)printf("%d\n",vec[i]);

            sem_signal();
            sleep(10);
        }
    }
    else    // PARENT
    {
        int type,val,tmp;
        while(1)
        {
            printf("new resource?:\n");
            scanf("%d",&newResource);
            if(newResource != 0)
            {
                sem_wait();

                printf("which resource to add:");
                scanf("%d",&type);
                if (type > (mem[size-4]-'0')) {printf("resource type does not exits\n");}
                else
                {
                    printf("how many:");
                    scanf("%d",&val);

                    tmp = ((mem[type*4+2]-'0') + val);
                    printf("tmp = %d\n",tmp);

                    if( tmp > 9) mem[type*4+2] = 9+'0'; // check mem val if over 9
                    else mem[type*4+2] += val;

                    printf("mem= %d\n",mem[type*4+2]-'0');
                    msync(mem,size,MS_SYNC);
                }
                newResource = 0;

                sem_signal();
            }
        }
    }
    return(0);
}
