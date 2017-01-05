#include <pwd.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "shell_fct.h"

//To complete
int main(int argc, char** argv)
{
	//..........
	int ret = MYSHELL_CMD_OK;
	char* readlineptr;
	struct passwd* infos;
	char str[1024];
	char hostname[256];
	char workingdirectory[256];

	//..........
	while(ret != MYSHELL_FCT_EXIT)
	{
	    int i;

		//Get your session info
        infos=getpwuid(getuid());
		gethostname(hostname, 256);
		getcwd(workingdirectory, 256);
        //Print it to the console
		sprintf(str, "\n{myshell}%s@%s:%s$ ", infos->pw_name, hostname, workingdirectory);
		readlineptr = readline(str);

        //Your code goes here.......

        //Parse the comand

        // Initialisation of the command.
        struct command * cmd;

        cmd->init_cmd = readlineptr;
        cmd->nb_cmd_members = 0;
        cmd->cmd_members = NULL;
        cmd->cmd_members_args = NULL;
        cmd->nb_members_args = NULL;
        cmd->redirection = NULL;
        cmd->redirection_type = NULL;

        parse_members(cmd->init_cmd, cmd);

        cmd->redirection = malloc(cmd->nb_cmd_members * sizeof(char **));
        cmd->redirection_type = malloc(cmd->nb_cmd_members * sizeof(int));

        for (i = 0; i < cmd->nb_cmd_members; i++)
            parse_redirection(i, cmd);

		parse_members_args(cmd);

        //print_members(&cmd);

        //Execute the comand
        //Clean the house
        //..........

	}
	//..........
	return 0;
}
