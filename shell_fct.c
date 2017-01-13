#include "shell_fct.h"

int exec_command(cmd* my_cmd)
{
    int ** tube, i;

    if ((tube = malloc(sizeof(int *) * my_cmd->nb_cmd_members)) == NULL)
        fatalError("Error in main.c !\n");

    for (i = 0; i < my_cmd->nb_cmd_members; i++)
    {
        if ((tube[i] = malloc(sizeof(int) * 2)) == NULL)
            fatalError("Error in main.c !\n");
    }

    return 0; //your return code goes here
}
