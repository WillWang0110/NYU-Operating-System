//
// Created by Christina Hu on 10/3/2018.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(void) {

    int id;
    int infinite_loop = 1;
    char *shm, *s, c;
    char *user_input = malloc(sizeof(char) * BUFSIZ);

    key_t key = ftok("shmfile", 65);
    id = shmget(key, BUFSIZ, IPC_CREAT | 0600);
    if (id == -1) {
        perror("shmget failed: shared memory creation failed");
        exit(1);
    }
    shm = shmat(id, NULL, 0);
    if(shm==(char*)-1){
        perror("shmat failed");
        exit(1);
    }


    while(infinite_loop)
        {
            printf("Enter an alpha numeric string: ");
            fgets(user_input, BUFSIZ, stdin);
            if (strncmp(user_input, "quit", 4) == 0)
                {
                    infinite_loop = 0;
                }
            char *p="COOL";
            if(strstr(user_input,p))
            {
                memcpy(shm, user_input, strlen(user_input));
                s = shm;
                s += strlen(user_input);
                *s = '$';

                while (*shm != '@')
                    sleep(1);
            }
        }
    if (shmdt(shm) == -1) {
        perror("shmdt failed");
        exit(1);
    }

    if (shmctl(id, IPC_RMID, 0) == -1) {
        perror("shmctl failed");
        exit(1);
    }
}



