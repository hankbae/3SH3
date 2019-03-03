#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

// Question 1: ps -e shows processes of all users and ps -l shows process uids, ppids, pgids, and winpids

// Question 2: deleted /usr/bin/bash

// Question 3: twice

// Question 4: pstree shows the current precesses as a tree

int main(void){
    int * test;
    int test2 = 2;
    test = &test2;
    printf("check\n");
    printf("%d\n",test);
    printf("%d\n",*test);
}