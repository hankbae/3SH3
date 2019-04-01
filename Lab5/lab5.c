#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include <string.h>

#define ERR(error) perror(error); exit(0);

struct stat buffer;

void fileAttr(const char * filename)
{
    printf("\n\nFile %s Attributes\n",filename);
    printf("-------------------------------\n");

    // mode
    printf("mode:\t\t");
    printf("%d",(buffer.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR))>>6);
    printf("%d",(buffer.st_mode & (S_IRGRP|S_IWGRP|S_IXGRP))>>3);
    printf("%d",(buffer.st_mode & (S_IROTH|S_IWOTH|S_IXOTH)));
    printf("\n");

    // number of links
    printf("links:\t\t%d\n",buffer.st_nlink);

    // owner's name
    struct passwd * owner=getpwuid(buffer.st_uid);
    printf("owner:\t\t%s\n",owner->pw_name);

    // group name
    struct group * name = getgrgid(buffer.st_gid);
    printf("group:\t\t%s\n",name->gr_name);
    // size in bytes
    printf("size:\t\t%d\n",buffer.st_size);
    // size in blocks
    printf("blocks:\t\t%d\n",buffer.st_blksize);
    // last modification time
    char date[20];
    strftime(date, 20, "%x - %X", localtime(&(buffer.st_mtime)));
    printf("mod time:\t%s\n",date);
    // name
    printf("name:\t\t%s\n",filename);
    printf("-------------------------------\n\n\n");

}

void folderTraverse(const char * str)
{
    DIR * directory;
    struct dirent * dirp;
    if ((directory=opendir(str)) == NULL)
    {
        printf("fail\n");
        return;
    }
    while ((dirp = readdir(directory)) != NULL)
    {
        if(strcmp(dirp->d_name,".")!=0 && strcmp(dirp->d_name,"..")!=0)
        if(dirp->d_type == DT_DIR)
        {
            char dir[64];
            strcpy(dir,str);
            strcat(dir,"/");
            strcat(dir,dirp->d_name);
            strcat(dir,"/..");
            printf("%s\t%s\n",dirp->d_name,dir);
            chdir(dir);
            folderTraverse(dirp->d_name);
            chdir("..");
        }
        else
        {
            fileAttr(dirp->d_name);
        }
        
    }
    closedir(directory);
    return;
}

int main (void)
{
    char str[128];
    printf("enter directory:");
    scanf("%s",str);

    // reading file info
    if(stat(str,&buffer)!=0){ERR("stat() error");}

    // checking if dirp or file
    if(S_ISDIR(buffer.st_mode))
    {
        folderTraverse(str);
    }
    else 
    {
        fileAttr(str);
    }
    
    return 0;
}
