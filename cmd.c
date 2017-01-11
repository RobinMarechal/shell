#include "cmd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//Your includes come here

//Prints the contents of members_args to the console
void print_members_args(cmd *c)
{
}

//Frees the memory allocated to store member arguments
void free_members_args(cmd *c)
{
}

//Prints the contents of members to the console
void print_members(cmd *c)
{
    unsigned int i = 0;
    printf("------- MEMBERS ------- \n");
    for (i = 0; i < c->nb_cmd_members; i++)
    {
        printf("%s\n", c->cmd_members[i]);
    }
    printf("\n");
}

//Frees the memory allocated to store member information
void free_members(cmd *c)
{

}

//Prints the redirection information for member i
void print_redirection(cmd *c, int i)
{
    int j;

    printf("\n------- REDIRECTION ------- \n");

    for (j = 0; j < 3; j++)
    {
        if (c->redirection[i][j] != NULL)
        {
            printf("%s\n", c->redirection[i][j]);

            break;
        }
    }

    printf("\n------- TYPE OF REDIRECTION ------- \n");

    if (c->redirection_type[i][APPEND] == 1)
        printf("La redirection du membre %d est de type APPEND.\n", i + 1);

    else if (c->redirection_type[i][OVERRIDE] == 1)
        printf("La redirection du membre %d est de type OVERRIDE.\n", i + 1);
}

//Frees the memory allocated to store redirection info
void free_redirection(cmd *c)
{

}

void parse_members_args(cmd *c)
{

}

//Remplit les champs initial_cmd, membres_cmd et nb_membres
void parse_members(char *s, cmd *c)
{
    int nb_members = 0;
    char delim[2] = "|";
    char * str = strdup(c->init_cmd);

    while(strtok(str, delim) != NULL)
    {
        nb_members++;
    }

    c->cmd_members = (char **) malloc(sizeof(char **) * nb_members);






}

//Remplit les champs redir et type_redir
void parse_redirection(unsigned int i, cmd *c)
{

}

char * subString(const char * start, const char * end)
{
    char * str = (char *) malloc(( end - start + 1 ) * sizeof(char));

    if(str == NULL)
    {
        printf("Error: malloc()\n");
        exit(-1);
    }

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
    char * result;
    char * end;
    int i = 0;

    if(str == NULL)
        return NULL;

    while(*(str+i) == ' ')
    {
        i++;
    }

    end = strchr(str+i, '\0');
    while(*(end-1) == ' ')
        end--;

    result = subString(str+i, end);
    free(str);
    return result;
}
