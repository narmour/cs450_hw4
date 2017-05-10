#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>



char* itoa(int val, int base){
    static char buf[32] = {0};
    int i = 30;
    for(; val && i ; --i, val /= base)
        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i+1];
}

int main(){
    int i =0;
    while(i<2){
        int x =200;
        if(i ==1)
            x = 5;
        char *b = itoa(x,2);
        printf("%s\n\n",b);
        i++;
    }
    return 0;
}
