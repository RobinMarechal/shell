#include "cmd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//Your includes come here

//Prints the contents of members_args to the console
void print_members_args(cmd *c)
{
    unsigned int i, j, nb;
    char ** member_args;

    printf("------- ARGS ------- \n");

    for (i = 0; i < c->nb_cmd_members; i++)
    {
        member_args = c->cmd_members_args[i];
        nb = c->nb_members_args[i];
        printf("Member %d:\n", i);
        for (j = 0; j < nb; j++)
        {
            printf("%s ", member_args[j]);
        }
        printf("\n");
    }

    printf("\n");
}

//Frees the memory allocated to store member arguments
void free_members_args(cmd *c)
{
    // unsigned int i, j;
    //
    // for(i = 0; i < c->nb_cmd_members; i++)
    // {
    //     for(j = 0; j < c->nb_members_args[i]; i++)
    //         free(c->cmd_members_args[i][j]);
    //
    //     free(c->cmd_members_args[i]);
    // }
    //
    // free(c->cmd_members_args);
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
    unsigned i;

    free(c->init_cmd);

    for(i = 0; i < c->nb_cmd_members; i++)
        free(c->cmd_members[i]);

    free(c->cmd_members);
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
    unsigned int i, j;

    for(i = 0; i < c->nb_cmd_members; i++)
    {
        for(j = 0; j < 3; j++)
        {
            free(c->redirection[i][j]);
        }

        free(c->redirection[i]);
        free(c->redirection_type[i]);
    }

    free(c->redirection);
    free(c->redirection_type);
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
        char * tmp = NULL;
        int j = 0;
        int nb_args;

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
            member_without_flux = trim(strdup(c->cmd_members[i]));
        }

        nb_args = 1;
        tmp = member_without_flux;
        while((tmp = strchr(tmp+1, ' ')) != NULL)
        {
            nb_args++;
        }

        c->nb_members_args[i] = nb_args;

        c->cmd_members_args[i] = (char **) malloc((nb_args + 1) * sizeof(char *));
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
        char * member = trim(subString(s, pipe));

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

    strcpy(c->cmd_members[c->nb_cmd_members - 1], trim(s));
}

//Remplit les champs redir et type_redir
void parse_redirection(unsigned int i, cmd *c)
{
    char * flux = NULL;
    int numberOfChevron = 0;

    size_t sizeOfMember = strlen(c->cmd_members[i]);

    if ((c->redirection[i] = (char **) malloc(3 * sizeof(char *))) == NULL)
    {
        printf("Error !");
        exit(-1);
    }

    if ((c->redirection_type[i] = (int *) malloc(2 * sizeof(int))) == NULL)
    {
        printf("Error !");
        exit(-1);
    }

    // Initialisation des types de flux.

    c->redirection_type[i][APPEND] = 0;
    c->redirection_type[i][OVERRIDE] = 0;

    // Recherche des flux.

    // STDIN.

    if ((flux = strchr(c->cmd_members[i], '<')) != NULL)
    {
        flux = subString(flux + 1, flux + strlen(flux));

        // Il peut y avoir un ou plusieurs chevrons.
        while (strchr(flux, '<') != NULL)
            flux = subString(flux + 1, flux + strlen(flux));

        if ((c->redirection[i][0] = (char *) malloc(sizeOfMember * sizeof(char))) == NULL)
        {
            printf("Error !");
            exit(-1);
        }

        strcpy(c->redirection[i][0], flux);

        // STDOUT et STDERR mis à NULL;
        c->redirection[i][1] = NULL;
        c->redirection[i][2] = NULL;
    }

    // STDOUT.
    else if ((flux = strchr(c->cmd_members[i], '>')) != NULL)
    {
        numberOfChevron++;

        flux = subString(flux + 1, flux + strlen(flux));

        // Il peut y avoir un ou plusieurs chevrons.
         while (strchr(flux, '>') != NULL)
         {
            flux = subString(flux + 1, flux + strlen(flux));

            numberOfChevron++;
         }

        if ((c->redirection[i][1] = (char *) malloc(sizeOfMember * sizeof(char))) == NULL)
        {
            printf("Error !");
            exit(-1);
        }

        strcpy(c->redirection[i][1], flux);

        // STDOUT et STDERR mis à NULL;
        c->redirection[i][0] = NULL;
        c->redirection[i][2] = NULL;

        // S'il y a plusieurs chevrons, la redirection est de type "APPEND".
        // Sinon, elle est de type "OVERRIDE".

        if (numberOfChevron > 1)
        {
            c->redirection_type[i][APPEND] = 1;
            c->redirection_type[i][OVERRIDE] = 0;
        }

        else
        {
            c->redirection_type[i][APPEND] = 0;
            c->redirection_type[i][OVERRIDE] = 1;
        }
    }
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

    while(*str == ' ')
    {
        str++;
    }

    end = strchr(str, '\0');
    while(*(end-1) == ' ')
        end--;

    return subString(str, end);
}
