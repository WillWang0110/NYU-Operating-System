#include <stdio.h>
#include "rpc.h"
#include <string.h>
#include <stdbool.h>

int calculate_digit(char * buf){
    int acc=0;
    while(*buf!='\n') {
        if (*buf >= '0' && *buf <= '9')
            acc++;
        buf++;
    }
    return acc;
}

int *osrpc_1_svc(char **msg, struct svc_req *req){

    static int result;

    //return 0 -> no output file
    FILE* f = fopen("secrets.out", "a");
    if (f == (FILE *) NULL) {
        result = 0;
        return (&result);
    }


    bool flag=false;
    if(strstr(*msg,"C00L")!=NULL)  flag=true;
    if (!flag) {
        printf("Illegal string! Do not have secret word\n");
        result=0;
        fclose(f);
        return(&result);
    }
    else printf("Message delivered successfully to server :-\n");


    int count=calculate_digit(*msg);

    fprintf(f, "%d :%s\n", count, *msg);
    fclose(f);
    result = 1;

    return(&result);
}

