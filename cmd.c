#include "cmd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//Your includes come here

//Prints the contents of members_args to the console
void print_members_args(cmd *c)
{
    // unsigned int i, j;
    // char ** member_args;
    // unsigned int nb;
    // for (i = 0; i < c->nb_cmd_members; i++)
    // {
    //     member_args = c->cmd_members_args[i];
    //     nb = c->nb_members_args[i];
    //     for (j = 0; j < nb; j++)
    //     {
    //         printf("%s ", member_args[j]);
    //     }
    // }
}

//Frees the memory allocated to store member arguments
void free_members_args(cmd *c)
{
    //your implementation comes here
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
    unsigned int i;
    char sep[2] = " ";

    c->cmd_members_args = (char ***) malloc(c->nb_cmd_members * sizeof(char **));
    c->nb_members_args = (unsigned int *) malloc(c->nb_cmd_members * sizeof(int));
    if(c->cmd_members_args == NULL || c->nb_members_args == NULL)
    {
        printf("ERROR: malloc()\n");
        exit(-1);
    }

    // for every member
    for(i = 0; i < c->nb_cmd_members; i++)
    {

        char * member_without_flux = NULL;
        char * flux_start = NULL;
        int j = 0;

        // checking if there is a flux
        // if there is one, we take the str before it
        flux_start = strchr(c->cmd_members[i], '>');
        if(flux_start == NULL)
        {
            flux_start = strchr(c->cmd_members[i], '<');
        }
        if(flux_start != NULL)
        {
            member_without_flux = trim(subString(c->cmd_members[i], flux_start));
        }
        else
        {
            member_without_flux = strdup(c->cmd_members[i]);
        }

        c->cmd_members_args[i] = (char **) malloc(2 * sizeof(char *));
        if(c->cmd_members_args == NULL)
        {
            printf("ERROR: malloc()\n");
            exit(-1);
        }

        // we split the args by cutting the str at spaces
        c->cmd_members_args[i][0] = strtok(member_without_flux, sep);

        while(c->cmd_members_args[i][j] != NULL)
        {
            c->cmd_members_args[i][j+1] = strtok(NULL, sep);
            j++;
        }
        c->nb_members_args[i] = j;
    }
}

//Remplit les champs initial_cmd, membres_cmd et nb_membres
void parse_members(char *s, cmd *c)
{
    char * pipe = NULL;

    // Le cas où il y a plusieurs membres.
    while ((pipe = strchr(s, '|')) != NULL)
    {
        // On saute l'espace de fin.
        char * member = subString(s, pipe - 1);

        c->nb_cmd_members++;

        if ((c->cmd_members = (char **) realloc(c->cmd_members, c->nb_cmd_members * sizeof(char *))) == NULL)
        {
            printf("Error !");
            exit(-1);
        }

        if ((c->cmd_members[c->nb_cmd_members - 1] = (char *) malloc(sizeof(char) * (strlen(member) + 1))) == NULL)
        {
            printf("Error !");
            exit(-1);
        }

        strcpy(c->cmd_members[c->nb_cmd_members - 1], member);

        // On saute l'espace de début.
        s = pipe + 2;
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

}

char * subString(const char * start, const char * end) {
    char * str = (char *) malloc( (size_t) ( end - start + 1 ) );
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
    if(str == NULL)
        return NULL;

    char * end;
    char * result;

    while(*str == ' ')
    {
        str++;
    }
    end = str;

    if((end = strchr(str, ' ')) == NULL)
    {
        result = subString(str, strchr(str, '\0'));
    }
    else
    {
        result = subString(str, end);
    }

    return result;
}
