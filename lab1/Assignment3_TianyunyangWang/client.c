#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5000
#define BUFSIZE 1024

int main(int argc,char* argv[]) {

    //get ip address of server from cmd
    if(argc!=2){
        printf("PLEASE ENTER IP ADDRESS OF SERVER\n");
        printf("FORMAT: X.X.X.X\n");
        printf("PORT CONFIGURED IS 5000\n");
        exit(-1);
    }

    //server info
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;   //ipv4
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(PORT);

    //create socket
    int sock=socket(AF_INET,SOCK_STREAM,0);  //ipv4
    if(sock==-1) {
        perror("Cannot create socket\n");
        exit(-1);
    }

    //connect server
    if(connect(sock,(struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
        perror("Cannot connect to server\n");
        exit(-1);
    }

    printf("You can enter \"quit\" to terminate\n");

    char * user_input=malloc(sizeof(char)*BUFSIZE);
    while(1){
        memset(user_input,0, BUFSIZE);

        //get input
        printf("Enter an alpha numeric string: ");
        fgets(user_input, BUFSIZE, stdin);

        //check if quit
        if (strncmp(user_input, "quit", 4) == 0) {
            //send quit to server to quit server
            if(send(sock,user_input,strlen(user_input),0)<0) {
                perror("Send to server failed\n");
                exit(-1);
            }
            break;
        }

        //check if satisfy secret code
        char *p="C00L";
        if(strstr(user_input,p))
        {
            //send to server
            if(send(sock,user_input,strlen(user_input),0)<0){
                perror("Send to server failed\n");
            }
        }
    }

    close(sock);

    return 0;
}