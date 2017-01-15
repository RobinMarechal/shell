#include "shell_fct.h"

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

int exec_command(cmd* my_cmd)
{
    char ** path = NULL;

    pid_t * pid = NULL;
    int ** tube = NULL, i;
    int * status = NULL;

    char buffer[20] = {0};

    // Initialiation des pipes.

    if ((tube = malloc(sizeof(int *) * my_cmd->nb_cmd_members)) == NULL)
        fatalError("Error in main.c !\n");

    for (i = 0; i < my_cmd->nb_cmd_members; i++)
    {
        if ((tube[i] = malloc(sizeof(int) * 2)) == NULL)
            fatalError("Error in main.c !\n");
    }

    if ((status = malloc(sizeof(int) * my_cmd->nb_cmd_members)) == NULL)
        fatalError("Error in main.c !\n");

    // Initialisation des pid.

    if ((pid = malloc(sizeof(pid_t) * my_cmd->nb_cmd_members)) == NULL)
        fatalError("Error in main.c !\n");

    if ((path = malloc(sizeof(char *) * my_cmd->nb_cmd_members)) == NULL)
        fatalError("Error in main.c !\n");

    for (i = 0; i < my_cmd->nb_cmd_members; i++)
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

            if ((path[i] = malloc(sizeof(char) * (strlen(my_cmd->cmd_members_args[i][0]) + 1))) == NULL)
                fatalError("Error in main.c !\n");

            sprintf(path[i], "/bin/%s", my_cmd->cmd_members_args[i][0]);

            /*my_cmd->cmd_members_args[i] = realloc(my_cmd->cmd_members_args[i], my_cmd->nb_members_args[i] + 1);

            my_cmd->nb_members_args[i]++;

            my_cmd->cmd_members_args[i][my_cmd->nb_members_args[i]] = NULL;*/

            /*    int j;
            for (j = 0; j < my_cmd->nb_members_args[i]; j++)
                printf("\n%s\n\n", my_cmd->cmd_members_args[i][j]);*/

            if (execvp(path[i], my_cmd->cmd_members_args[i]) == -1)
            {
                perror("execlp");
                exit(errno);
            }
        }
    }

    for (i = 0; i < my_cmd->nb_cmd_members - 1; i++)
    {
        close(tube[i][0]);
        close(tube[i][1]);
    }

    close(tube[my_cmd->nb_cmd_members - 1][1]);
    dup2(tube[my_cmd->nb_cmd_members - 1][0], 0);
    close(tube[my_cmd->nb_cmd_members - 1][0]);

    for (i = 0; i < my_cmd->nb_cmd_members; i++)
        waitpid(pid[i], &status[i], 0);

    while(fgets(buffer, 20, stdin) != NULL)
        printf("%s", buffer);

    for (i = 0; i < my_cmd->nb_cmd_members; i++)
        free(path[i]);

    free(path);

    free(pid);

    for (i = 0; i < my_cmd->nb_cmd_members; i++)
        free(tube[i]);

    free(tube);

    for (i = 0; i < my_cmd->nb_cmd_members; i++)
        free(status[i]);

    free(status);

    return 0; //your return code goes here
}
