#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>


int main(void)
{
    int file, type,fileSize,check,val;
    char * mem;
    struct stat buffer;
    size_t size;
    file = open("res.txt",O_RDWR);
    if(fstat(file,&buffer)==-1)
    {
        printf("fstat error\n");
        exit(0);
    }
    size = buffer.st_size;
    mem = mmap(NULL, size, PROT_EXEC|PROT_READ|PROT_WRITE, MAP_SHARED,file, 0);
    close(file);
    // int i;
    // for(i = 0; i<size;i++){printf("%c",mem[i]);}

    while(1)
    {
        printf("which resource type:");
        scanf("%d",&type);
        if(type > (mem[size-4]-'0'))
        {
            printf("type does not exist\n");
        }
        else
        {
            printf("how many:");
            scanf("%d",&val);
            if (val > (mem[type*4+2] - '0')) {
                printf("not enough resources\n");
            }
            else
            {
                mem[type*4+2] -= val;
                printf("type: %d\n",type);
                printf("val: %c\n",mem[type*4+2]);
                msync(mem,size,MS_SYNC);
            }
        }
    }
    return(0);
}
