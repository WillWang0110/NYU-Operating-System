#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SIZE 1024

int main(void)
{
    key_t key=ftok("shmfile", 65);
    int id;
    int num;
    int infinite_loop = 1;
    char line[BUFSIZ];
    char *shm, *s;
    FILE * secrets;

    secrets = fopen("secrets.out","w");

    id = shmget(key,SIZE, 0600);

    if (id ==-1) {
        perror("shmget failed: shared memory creation failed");
        exit(EXIT_FAILURE);
    }

    shm = shmat(id, NULL, 0);

    if (shm == (char *) -1) {
        perror("shmat failed");
        exit(1);
    }

    while (infinite_loop) {

        if (*shm != (int) NULL)
        {
            if (*shm != '@')
            {
                num = 0;
                memset(line, 0, sizeof(line));

                for (s = shm; *s != '$'; s++)
                {
                    line[num] = *s;
                    num++;
                }
                if (strncmp(line, "quit", 4) == 0)
                {
                    infinite_loop = 0;
                }

                *shm = '@';
                secrets = fopen ("secrets.out","a");

                if (secrets != NULL)
                {
                    fprintf(secrets, "%d: ", num-1);
                    fputs(line, secrets);
                    fclose(secrets);
                }
            }
        }
    }

    if (shmdt(shm) == -1) {
        perror("shmdt failed");
        exit(1);
    }
}