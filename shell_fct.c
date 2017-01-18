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

void exec_redirection_in(cmd * c, int i)
{
    if (c->redirection_type[i][STDIN] == RFILE)
    {
        int fd0 = open(c->redirection[i][STDIN], O_RDONLY);
        dup2(fd0, STDIN_FILENO);
        close(fd0);
    }

    /*else if (c->redirection_type[i][STDIN] == KEYBOARD)
    {
        int fd0 = open(c->redirection[i][STDIN], O_WRONLY | S_IWUSR);
        dup2(fd0, STDIN_FILENO);
        close(fd0);
    }*/
}


void exec_redirection_out(cmd * c, int i)
{
    if (c->redirection_type[i][STDOUT] == APPEND)
    {
        // S_IRUSR : permission de lecture.
        // S_IWUSR : permission d'écriture.

        int fd0 = open(c->redirection[i][STDOUT], O_RDWR | O_APPEND | S_IRUSR | S_IWUSR);
        dup2(fd0, STDOUT_FILENO);
        close(fd0);
    }

    // Redirection de type OVERRIDE.

    else if (c->redirection_type[i][STDOUT] == OVERRIDE)
    {
        // S_IRUSR : permission de lecture.
        // S_IWUSR : permission d'écriture.

        int fd0 = creat(c->redirection[i][STDOUT], O_RDWR | S_IRUSR | S_IWUSR);
        dup2(fd0, STDOUT_FILENO);
        close(fd0);
    }
}


int exec_command(cmd * c)
{
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

            c->cmd_members_args[i] = realloc(c->cmd_members_args[i], c->nb_members_args[i] + 1);

            c->nb_members_args[i]++;

            c->cmd_members_args[i][c->nb_members_args[i]] = NULL;

            // Exécution des redirections.

            exec_redirection_out(c, i);

            exec_redirection_in(c, i);

            if (execvp(c->cmd_members_args[i][0], c->cmd_members_args[i]) == -1)
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

    free(pid);

    for (i = 0; i < c->nb_cmd_members; i++)
        free(tube[i]);

    free(tube);

    free(status);

    return 0; //your return code goes here
}
