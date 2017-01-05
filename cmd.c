#include "cmd.h"
#include <string.h>
//Your includes come here

//Prints the contents of members_args to the console
void print_members_args(cmd *c){
    int i, j;
    char ** member_args;
    int nb;
    for (i = 0; i < c->nb_cmd_members; i++)
    {
        member_args = c->cmd_members_args[i];
        nb = c->nb_members_args[i];
        for (j = 0; j < nb; j++)
        {
            printf("%s ", member_args[j]);
        }
    }

void print_members_args(cmd *c)
{
    //your implementation comes here
}

//Frees the memory allocated to store member arguments
void free_members_args(cmd *c)
{
    //your implementation comes here
}

//Prints the contents of members to the console
void print_members(cmd *c)
{

}

//Frees the memory allocated to store member information
void free_members(cmd *c)
{
    //your implementation comes here
}

//Prints the redirection information for member i
void print_redirection(cmd *c, int i)
{
    //your implementation comes here
}

//Frees the memory allocated to store redirection info
void free_redirection(cmd *c)
{
    //your implementation comes here
}

void parse_members_args(cmd *c)
{
    int i;
    char * start, * end;
    char * arg;
    char sep[2] = " ";

    c->cmd_members_args = malloc(c->nb_cmd_members * sizeof(char **));
    if(c->cmd_members_args == NULL)
    {
        printf("ERROR: malloc()\n");
        exit(EXIT_ERROR);
    }

    start = strdup(c->init_cmd);
    end = start;

    for(i = 0; i < c->nb_cmd_members; i++)
    {

        char * member_without_flux = strdup(c->cmd_members[i]);
        int offset = 0;
        int j = 1;

        if(member_without_flux[0] != '\0' || member_without_flux[0] != '<' || member_without_flux[0] != '>')
        {
            while(member_without_flux[offset + 1] != '\0' || member_without_flux[offset + 1] != '<' || member_without_flux[offset + 1] != '>')
            {
                offset++;
            }
        }

        member_without_flux[offset] = '\0';


        c->cmd_members_args[i] = malloc(2 * sizeof(char *));
        if(c->cmd_members_args == NULL)
        {
            printf("ERROR: malloc()\n");
            exit(EXIT_ERROR);
        }

        c->cmd_members_args[i][0] = trim(strtok(member_without_flux, sep));

        while(c->cmd_members_args[i][j] != NULL)
        {
            c->cmd_members_args[i][j+1] = trim(strtok(NULL, sep));
            j++;
        }
    }

    // TEST
    int c1; c2;
    for(c1 = 0; c1 < c->nb_cmd_members; c1++)
    {
        printf("\n");
        for(c2 = 0; c2 < c->nb_members_args[c1]; c2++)
        {
            printf("%s\n", c->cmd_members_args[c1][c2]);
        }
    }
}

//Remplit les champs initial_cmd, membres_cmd et nb_membres
void parse_members(char *s, cmd *c)
{
    int i = 0;
}

//Remplit les champs redir et type_redir
void parse_redirection(unsigned int i, cmd *c)
{
    //your implementation comes here
}

char * IMPLEMENT(subString)(const char * start, const char * end) {
    char * str = malloc( (size_t) ( end - start + 1 ) );
    int i = 0;
    while(start+i != end && start[i] != '\0')
    {
        str[i] = start[i];
        i++;
    }
    str[i] = '\0';

    return str;
}

char * trim(char * str)
{
    char * end;
    char * result;

    while(str == ' ')
    {
        str++;
    }
    end = str;

    if((end = strchr(str, ' ')) == NULL)
    {
        result = subString(str, strchr(str, '\0'));
    }

    result = subString(str, end);
    free(str);
    return result;
}
