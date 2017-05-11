#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <ctype.h>






int isHex(char *va){
    int r = 0;
    for(int i =0;i <strlen(va);i++){
        if(!isdigit(va[i]))
                r =1;//it is hex
    }
    return r;
}

char* itoa(int val, int base){
    static char buf[32] = {0};
    int i = 30;
    for(; val && i ; --i, val /= base)
        buf[i] = "0123456789abcdef"[val % base];

    return strdup(&buf[i+1]);

}


int pow2(int n){
    int r = 1;
    int pow = 0;
    while(r <n){
        r*=2;
        pow++;
    }
    return pow;
}

int main(int argc, char **argv)
{
    // read in header of input
    FILE *fd = fopen(argv[1],"r");
    int x;
    // read in page table lines
    int *v = malloc(4 * 100);
    int count =0;

    while(1){
        int n;
        if(fscanf(fd,"%d",&n) !=1)
            break;
        v[count++] = n;
    }
    int va_bits = v[0];
    int pa_bits = v[1];
    int num_bytes = v[2];

    //printf("%i %i %i\n",va_bits,pa_bits,num_bytes);
    printf("va_bits: %i pa_bits: %i page_size: %i\n",va_bits,pa_bits,num_bytes);
    fclose(fd);

    int **table = malloc(sizeof(int*) * count);
    int j =0;
    for(int i =3;i<count;i+=4){
        int *r = malloc(sizeof(int) * 4);
        r[0] = v[i];
        r[1] = v[i+1];
        r[2] = v[i+2];
        r[3] = v[i+3];
        printf(" %d %d %d %d\n",r[0],r[1],r[2],r[3]);
        table[j] = r;
        j++;
    }


    // read va.txt
   


    // clock algorithm stuff
    int cidx = 0;
    int ridx = -1;//replacement index
    int found =0;
    while(1){
        char *va = malloc(sizeof(char) * va_bits);
        if(fscanf(stdin,"%s ",va) !=1)
                break;
        printf("virtual address: %s\n",va);
        //printf("res: %i\n",isHex(va));

        // get decimal value
        int v;
        int bHex = isHex(va);
        v = (bHex)  ? strtol(va,NULL,16) : strtol(va,NULL,10);
        //printf("VALUE: %i\n",v);


        // get binary rep
        char *b = itoa(v,2);
        if(strlen(b) < va_bits){
            char *n = malloc(sizeof(char) * va_bits);
            for(int i =0;i<va_bits-strlen(b);i++)
                n[i] = '0';
            strcpy(&n[(va_bits-strlen(b))],b);
            //printf("n: %s\n",n);
            free(b);
            b = n;
        }
        //printf("binary rep: %s\n",b);

                        // check out page table
                      


        int offset = pow2(num_bytes);
        char *vpt_bits = malloc(sizeof(char) * (va_bits -offset));
        char *offset_bits = malloc(sizeof(char)*offset);

        //save offset bits
        strcpy(offset_bits,&b[va_bits -offset]);
        
        // copy vpt index bits
        for(int i =0;i<va_bits-offset;i++)
            vpt_bits[(va_bits-offset) -(i+1)] = b[va_bits-offset- (i+1) ];

        //printf("vpt bits: %s\n",vpt_bits);
        //printf("offset bits: %s\n",offset_bits);
        int vpt_idx = strtol(vpt_bits,NULL,2);

        
        // go to page table
        int num_entries =j;

        if(table[vpt_idx][0] ==0){
            
            if(table[vpt_idx][1] ==0)
                puts("SEGFAULT");
            #ifdef PROB1
            else{
                puts("DISK");
                continue;
            }
            #else 
            else{

                found = 0;
                while(found==0){
                    if(table[cidx][0] !=0){// if valid page
                        if(table[cidx][3] >0)// reset used bit if 1
                            table[cidx][3] =0;
                        else{// else this is the replacement
                            printf("REPLACING IDX %i\n",cidx);
                            table[cidx][0] = 0; // turn this page offf
                            table[vpt_idx][2] = table[cidx][2]; // use this physical page
                            table[vpt_idx][3] = 1; //use bit =1
                            table[vpt_idx][0] = 1;//turn this page on
                            //
                            found =1;

                            // print out table
                            for(int i =0;i<num_entries;i++){
                                printf("%i %i %i %i\n",table[i][0],table[i][1],
                                        table[i][2],table[i][3]);
                            }

                        }
                    }
                    cidx++;
                    if(cidx ==num_entries)
                        cidx=0;

                
                }


            }
            #endif
        }

        // else theres a physical address
            table[vpt_idx][3] = 1;
            char *pa_pgnum = itoa(table[vpt_idx][2],2);
            strcat(pa_pgnum,offset_bits);
            // append 0's if necessary
            if (strlen(pa_pgnum) < pa_bits){
                char *n = malloc(sizeof(char) * pa_bits);
                for(int i =0;i<pa_bits-strlen(pa_pgnum);i++)
                    n[i] = '0';
                strcpy(&n[(pa_bits-strlen(pa_pgnum))],pa_pgnum);
                free(pa_pgnum);
                pa_pgnum = n;
            }

            int x = strtol(pa_pgnum,NULL,2);
            printf("physical address: %s      DECIMAL(%i)\n",pa_pgnum,x);
            printf("\n\n");


    }









}
