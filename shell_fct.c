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

pid_t currentChild;


void alarmHandler(int signum)
{
    printf("Too long: killing child process !\n");
    kill(currentChild, SIGKILL);
}


void exec_redirection_in(cmd * c, int i)
{
    if (c->redirection_type[i][STDIN] == RFILE)
    {
        int fd = open(c->redirection[i][STDIN], O_RDONLY);
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
}


void exec_redirection_out(cmd * c, int i)
{
    if (c->redirection_type[i][STDOUT] == APPEND)
    {
        // S_IRUSR : permission de lecture.
        // S_IWUSR : permission d'écriture.

        int fd = open(c->redirection[i][STDOUT], O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);

        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    // Redirection de type OVERRIDE.

    else if (c->redirection_type[i][STDOUT] == OVERRIDE)
    {
        // S_IRUSR : permission de lecture.
        // S_IWUSR : permission d'écriture.

        int fd = creat(c->redirection[i][STDOUT], O_RDWR | S_IRUSR | S_IWUSR);
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}


int exec_command(cmd * c)
{
    pid_t * pid = NULL;
    int ** tube = NULL;
    int * status = NULL;
    unsigned int i;

    char buffer[1] = {0};

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

        currentChild = pid[i];

        if(pid[i] == 0)
        {
            signal(SIGALRM, alarmHandler);
            alarm(5);

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

            // Exécution des redirections.

            exec_redirection_out(c, i);

            exec_redirection_in(c, i);

            // Exécution des commandes.

            if (execvp(c->cmd_members_args[i][0], c->cmd_members_args[i]) == -1)
            {
                perror("execlp");
                exit(errno);
            }
        }
    }

    if (i > 1)
    {
        close(tube[c->nb_cmd_members - 2][0]);
        close(tube[c->nb_cmd_members - 2][1]);
    }

    close(tube[c->nb_cmd_members - 1][1]);

    for (i = 0; i < c->nb_cmd_members; i++)
        waitpid(pid[i], &status[i], 0);

    while (read(tube[c->nb_cmd_members - 1][0], buffer, 1) != 0)
        printf("%s", buffer);

    close(tube[c->nb_cmd_members - 1][0]);

    free(pid);

    for (i = 0; i < c->nb_cmd_members; i++)
        free(tube[i]);

    free(tube);

    free(status);

    return 0; //your return code goes here
}
