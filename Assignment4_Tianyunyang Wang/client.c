#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include "rpc.h"


int main(int argc, char **argv)
{

    if(argc !=2)
    {
        fprintf(stderr, "Usage : %s <host> \n",argv[0]);
        exit(1);
    }

    CLIENT *cl;
    int *result;
    char user_input[1024];
    char* message;

    while(1){

        memset(user_input,0, BUFSIZ);

        //get input
        printf("Enter an alpha numeric string:");
        fgets(user_input,BUFSIZ,stdin);
        message=user_input;

        //create connect
        cl=clnt_create(argv[1],osprgm,osprgm_vs,"udp");
        if (cl == NULL) {
            clnt_pcreateerror(argv[1]);
            exit(1);
        }

        //call remote procedure
        result=osrpc_1(&message,cl);
        if (result == NULL) {
            clnt_perror(cl,argv[1]);
            exit(1);
        }
        if (*result == 0) {
            printf("U HAVE TO ENTER THE SECRET CODE\n");
        }
        else{
            printf("Message delivered successfully to %s\n",argv[1]);
        }
    }
}