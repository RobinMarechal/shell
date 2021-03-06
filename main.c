#include <pwd.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <readline/history.h>
#include "shell_fct.h"
#include "helpers.h"

//To complete
int main(int argc, char** argv)
{
	//..........
	int ret = MYSHELL_CMD_OK;

	char* readlineptr;
	struct passwd* infos;
	char hostname[256];
	char workingdirectory[256];

	while(ret != MYSHELL_FCT_EXIT)
	{
	    unsigned int i;

		//Get your session info
        infos=getpwuid(getuid());
		gethostname(hostname, 256);
		getcwd(workingdirectory, 256);

        char str[1024];

        //Print it to the console
		sprintf(str, "\n{myshell}%s@%s:%s$ ", infos->pw_name, hostname, workingdirectory);

		readlineptr = readline(str);

        add_history(readlineptr);


        // If nothing is entered by the user, we skip the iteration.
        if (*readlineptr == NULL)
            continue;

        //Parse the comand

        // Initialisation of the command.
		struct _command cmd;

        cmd.init_cmd = strdup(readlineptr);
        cmd.nb_cmd_members = 0;
        cmd.cmd_members = NULL;
        cmd.cmd_members_args = NULL;
        cmd.nb_members_args = NULL;

		// PARSE FUNCTIONS
        parse_members(cmd.init_cmd, &cmd);
		parse_members_args(&cmd);

		cmd.redirection = (char ***) malloc(cmd.nb_cmd_members * sizeof(char **));
		cmd.redirection_type = (int **) malloc(cmd.nb_cmd_members * sizeof(int *));

		if( cmd.redirection == NULL || cmd.redirection_type == NULL)
			fatalError("Error: malloc()");

		for (i = 0; i < cmd.nb_cmd_members; i++)
		{
			parse_redirection(i, &cmd);
		}

		// PRINT FUNCTIONS
		//print_cmd(&cmd);

		exec_command(&cmd);

		// FREES
		free_members_args(&cmd);
		free_members(&cmd);
		free_redirection(&cmd);
		free(readlineptr);
	}

	return 0;
}
