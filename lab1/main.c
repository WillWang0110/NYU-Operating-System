//
// Created by Christina Hu on 17/2/2018.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 500

//1.read input

char* readline()
{
    char* line=(char *)malloc(MAX_BUFFER_SIZE*sizeof(char));
    size_t size=MAX_BUFFER_SIZE;
    getline(&line,&size,stdin);
    return line;
}


//2.split input to token

char **splitline(char *input)
{
    char **command= malloc(8 * sizeof(char*));
    char *separator=" \n";
    char *parsed;
    int index=0;
    parsed=strtok(input,separator);
    if(!parsed) {
        perror("input read failed");
        exit(1);
    }

    while(parsed){
        command[index]=parsed;
        index++;
        parsed=strtok(NULL,separator);
    }
    return command;
}

//3.design fork

void execute(char **argv) {
    pid_t pid;
    int status;
    pid = fork();
    if (pid < 0) {
        printf("ERROR: forking child process failed\n");
        exit(-1);
    } else if (pid == 0)
    {
        if (execvp(argv[0], argv) < 0)
        {
            printf("Sorry, Please Try Again");
        }
        exit(0);
    }
    else
    {
        while (!wait(&status)) {
            sleep(1);
        }
        printf("%s", "> ");
    }
}

   /*if(pid==0)
    {
        execvp(argv[0],argv);
        printf("Unknown command\n");
        exit(0);
    }
    else
    {
        pid_t tpid=wait(&status);
        if(tpid!=pid)
        {
            printf("%s", "> ");
            _Exit(0);
        }
    }
}
*/

//4.cd input

void cd(char *path) {
    chdir(path);
    printf("%s","> ");
}

//5.execute main

int main()
{
    printf("%s","> ");
    while(1)
    {
        char *line = readline();
        char **token = splitline(line);
        /*test
        int id=0;
        while(token[id]!=NULL){
            printf("%s\n",token[id]);
            id++;
        }
        printf("%d\n",id);
        */
        if(strcmp(token[0],"exit")==0) exit(0);//clarrify the input command
        if(strcmp(token[0],"cd")==0)
        {
            cd(token[1]);
            continue;
        }
        if(token[0]!=NULL) execute(token);
        free(line);
        free(token);
    }
}
