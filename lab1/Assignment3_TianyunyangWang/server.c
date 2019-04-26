#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5000
#define BUFSIZE 1024

int digitsum=0;
int linesum=0;

int calculate_digit(char * buf){
    int acc=0;
    while(*buf!='\n') {
        if (*buf >= '0' && *buf <= '9')
            acc++;
        buf++;
    }
    return acc;
}


void cc_handler(int x){
    printf("\nSummary:\n Total number of lines:%d\n Total number of digits:%d\n",linesum,digitsum);
}

int main() {

    //Ctrl-C
    signal(SIGINT,cc_handler);

    //server info
    struct sockaddr_in serv_addr,client_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;   //ipv4
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  //all available interfaces
    serv_addr.sin_port = htons(PORT);

    //create socket
    int sock=socket(AF_INET,SOCK_STREAM,0);  //ipv4
    if(sock==-1) {
        perror("Cannot create socket\n");
        exit(-1);
    }

    //bind socket to server addr
    if(bind(sock,(struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
        perror("Bind failed\n");
        exit(-1);
    }

    //listen, allow 1 pending connections
    if(listen(sock,1)==-1){
        perror("Listen failed\n");
        exit(-1);
    }

    //accept, get client socket
    socklen_t addrlen=sizeof(client_addr);
    int csock=accept(sock,(struct sockaddr*)&client_addr,&addrlen);
    if(csock==-1){
        perror("Accept failed\n");
        exit(-1);
    }

    //open output file
    FILE * secrets=fopen("secrets.out","w");
    if(secrets==NULL){
        perror("Cannot open \"secrets.out\"\n");
        exit(-1);
    }
    fclose(secrets);

    //body
    char* buffer=(char*)malloc(sizeof(char)*BUFSIZE);
    while(1){

        //receive content
        if((recv(csock,buffer,BUFSIZE,0))==-1){
            perror("Cannot receive from client\n");
            exit(-1);
        }

        //quit
        if(strncmp(buffer, "quit", 4) == 0){
            break;
        }

        //write to file
        linesum++;
        secrets=fopen("secrets.out","a");
        int count=calculate_digit(buffer);  //calculate digits in received buffer
        digitsum+=count;
        fprintf(secrets,"%d: ",count);
        fputs(buffer,secrets);
        fclose(secrets);

    }

    close(csock);
    close(sock);

    return 0;
}