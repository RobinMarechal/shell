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
    //your implementation comes here
}

//Remplit les champs initial_cmd, membres_cmd et nb_membres
void parse_members(char *s, cmd *c)
{
    char * pipe = NULL;

    // Le cas où il y a plusieurs membres.
    while ((pipe = strchr(s, '|')) != NULL)
    {
        char * member = subString(s, pipe);

        c->nb_cmd_members++;

        if ((c->cmd_members = (char **) realloc(c->cmd_members, c->nb_cmd_members * sizeof(char *))) == NULL)
        {
            printf("Error !");
            exit(-1);
        }

        if ((c->cmd_members[c->nb_cmd_members - 1] = malloc(sizeof(char) * (strlen(member) + 1))) == NULL)
        {
            printf("Error !");
            exit(-1);
        }

        strcpy(c->cmd_members[c->nb_cmd_members - 1], member);

        s = pipe + 1;
    }

    // Le cas où il n'y a qu'un membre ou s'il s'agit du dernier.
    c->nb_cmd_members++;

    if ((c->cmd_members = (char **) realloc(c->cmd_members, c->nb_cmd_members * sizeof(char *))) == NULL)
    {
        printf("Error !");
        exit(-1);
    }

    if ((c->cmd_members[c->nb_cmd_members - 1] = (char *) malloc(sizeof(char) * (strlen(s) + 1))) == NULL)
    {
        printf("Error !");
        exit(-1);
    }

    strcpy(c->cmd_members[c->nb_cmd_members - 1], s);
}

//Remplit les champs redir et type_redir
void parse_redirection(unsigned int i, cmd *c)
{
    //your implementation comes here
}

char * subString(const char * start, const char * end) {
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
