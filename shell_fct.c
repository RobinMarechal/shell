#include "shell_fct.h"

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include "helpers.h"

int exec_command(cmd* c)
{
    char ** path = NULL;

    pid_t * pid = NULL;
    int ** tube = NULL;
    int * status = NULL;
    unsigned int i;

    char buffer[20] = {0};

    // Initialiation des pipes.

    if ((tube = (int **) malloc(sizeof(int *) * c->nb_cmd_members)) == NULL)
        fatalError("Error in main.c !\n");

    for (i = 0; i < c->nb_cmd_members; i++)
    {
        if ((tube[i] = (int *) malloc(sizeof(int) * 2)) == NULL)
            fatalError("Error in main.c !\n");
    }

    if ((status = (int *) malloc(sizeof(int) * c->nb_cmd_members)) == NULL)
        fatalError("Error in main.c !\n");

    // Initialisation des pid.

    if ((pid = (pid_t *) malloc(sizeof(pid_t) * c->nb_cmd_members)) == NULL)
        fatalError("Error in main.c !\n");

    if ((path = (char  **) malloc(sizeof(char *) * c->nb_cmd_members)) == NULL)
        fatalError("Error in main.c !\n");

    for (i = 0; i < c->nb_cmd_members; i++)
    {
        pipe(tube[i]);

        pid[i] = fork();

        if(pid[i] == 0)
        {
            if (i > 1)
            {
                close(tube[i - 2][0]);
                close(tube[i - 2][1]);
            }

            if (i > 0)
            {
                close(tube[i - 1][1]);
                dup2(tube[i - 1][0], 0);
                close(tube[i - 1][0]);
            }

            //Redirect output
            close(tube[i][0]);
            dup2(tube[i][1], 1);
            close(tube[i][1]);

            if ((path[i] = (char *) malloc(sizeof(char) * (strlen(c->cmd_members_args[i][0]) + 1))) == NULL)
                fatalError("Error in main.c !\n");

            sprintf(path[i], "/bin/%s", c->cmd_members_args[i][0]);

            /*c->cmd_members_args[i] = realloc(c->cmd_members_args[i], c->nb_members_args[i] + 1);

            c->nb_members_args[i]++;

            c->cmd_members_args[i][c->nb_members_args[i]] = NULL;*/

            /*    int j;
            for (j = 0; j < c->nb_members_args[i]; j++)
                printf("\n%s\n\n", c->cmd_members_args[i][j]);*/

            if (execvp(path[i], c->cmd_members_args[i]) == -1)
            {
                perror("execlp");
                exit(errno);
            }
        }
    }

    for (i = 0; i < c->nb_cmd_members - 1; i++)
    {
        close(tube[i][0]);
        close(tube[i][1]);
    }

    close(tube[c->nb_cmd_members - 1][1]);
    dup2(tube[c->nb_cmd_members - 1][0], 0);
    close(tube[c->nb_cmd_members - 1][0]);

    for (i = 0; i < c->nb_cmd_members; i++)
        waitpid(pid[i], &status[i], 0);

    while(fgets(buffer, 20, stdin) != NULL)
        printf("%s", buffer);

    for (i = 0; i < c->nb_cmd_members; i++)
        free(path[i]);

    free(path);

    free(pid);

    for (i = 0; i < c->nb_cmd_members; i++)
        free(tube[i]);

    free(tube);

    free(status);

    return 0; //your return code goes here
}
