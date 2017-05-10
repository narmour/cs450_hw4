#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(int argc, char **argv){
    if(argc < 2){
        puts("need filename");
        return 0;
    }
    // opens some existing file for reading
    FILE *fd = fopen(argv[1],"rw");

    //reads a value from the file
    char c;
    fscanf(fd,"%c",&c);
    printf("read in %c\n",c);
    //fclose(fd);

    //forks a child process that calls unlink() to delete the file
    if(fork() ==0){
        unlink(argv[1]);
    }
    wait(NULL);

    //tries to read the new -deleted file
    //fd = fopen(argv[1],"rw");
    char d;
    int res;
    if(res = fscanf(fd,"%c",&d)!=-1){

        printf("after delete, read in %c\n",d);
    }










}
