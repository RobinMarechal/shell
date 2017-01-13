#include "cmd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//Your includes come here

//Prints the contents of members to the console
void print_members(cmd *c)
{
    unsigned int i = 0;
    printf("------- MEMBERS ------- \n");
    for (i = 0; i < c->nb_cmd_members; i++)
    {
        printf("-%s-\n", c->cmd_members[i]);
    }
    printf("\n");
}

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
        printf("\nMember %d:\n", i);
        for (j = 0; j < nb; j++)
        {
            printf("  %d => %s\n", j, member_args[j]);
        }
        printf("\n");
    }

    printf("\n");
}

//Prints the redirectio information for member i
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

    if (c->redirection_type[i][STDOUT] == APPEND)
        printf("La redirection du membre %d est de type APPEND.\n", i + 1);

    else if (c->redirection_type[i][STDOUT] == OVERRIDE)
        printf("La redirection du membre %d est de type OVERRIDE.\n", i + 1);
}

//Frees the memory allocated to store member information
void free_members(cmd *c)
{
    unsigned int i;
    free(c->init_cmd);
    for (i = 0; i < c->nb_cmd_members; i++)
    {
        free(c->cmd_members[i]);
    }
}

//Frees the memory allocated to store member arguments
void free_members_args(cmd *c)
{
    unsigned int i, j;
    for(i = 0; i < c->nb_cmd_members; i++)
    {
        for(j = 0; j < c->nb_members_args[i]; j++)
        {
            free(c->cmd_members_args[i][j]);
        }

        free(c->cmd_members_args[i]);
    }

    free(c->cmd_members_args);
    free(c->nb_members_args);
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
    // Initialisation
    char member[256] = {0};
    unsigned int i, j, k;
    char is_space = 0, was_space = 0;
    char * tok;
    char space[2] = " ";
    char * duplicate;


    // Allocate c->cmd_members_args && c->nb_members_args
    if((c->cmd_members_args = (char ***) calloc(c->nb_cmd_members, sizeof(char **))) == NULL)
        fatalError("Error: calloc() failed");


    if((c->nb_members_args = (unsigned int *) calloc(c->nb_cmd_members, sizeof(unsigned int))) == NULL)
        fatalError("Error: calloc() failed");


    // For each member
    for(i = 0; i < c->nb_cmd_members; i++)
    {
        strcpy(member, c->cmd_members[i]);
        c->nb_members_args[i] = 1;

        // get the nb of args (without flux)
        k = 0;
        // while ther char is not the end OR the beginig of a flux (< || >)
        while(member[k] != '\0')
        {
            was_space = (member[k] == ' ');
            k++;
            is_space = (member[k] == ' ');

            if(is_space == 1 && was_space == 0)
                c->nb_members_args[i]++;

            if(member[k] == '<' || member[k] == '>')
            {
                c->nb_members_args[i]--;
                break;
            }
        }


        // allocate c->cmd_members_args[i]
        // malloc enough memory to insert an aditionnal NULL at the end of the array
        if((c->cmd_members_args[i] = (char **) calloc(c->nb_members_args[i] + 1, sizeof(char *))) == NULL)
            fatalError("Error: calloc() failed");


        tok = strtok(member, space);
        j = 0;

        // For each member args
        while(j < c->nb_members_args[i])
        {
            // add arg to c->cmd_members_args[i][j]
            duplicate = strdup(tok);
            printf("\nDUPLICATE: -%s-", duplicate);
            c->cmd_members_args[i][j] = trim(duplicate);
            free(duplicate);

            // get next arg
            tok = strtok(NULL, space);
            j++;
        }
    }
}

//Remplit les champs initial_cmd, membres_cmd et nb_membres
void parse_members(char *s, cmd *c)
{
    // Initialisation
    char str[256] = {0};
    char * tok = NULL;
    char * duplicate = NULL;
    char delimiter[2] = "|";
    unsigned int i = 0;

    strcpy(str, c->init_cmd);

    // get the nb of members
    while(str[i] != '\0')
    {
        if(str[i] == *delimiter)
            c->nb_cmd_members++;

        i++;
    }

    c->nb_cmd_members++;


    // allocate data
    // malloc enough memory to insert an aditionnal NULL at the end of the array
    if((c->cmd_members = (char **) calloc(c->nb_cmd_members + 1, sizeof(char *))) == NULL)
        fatalError("Error: calloc() failed");

    printf("\nSIZE: %d\n", c->nb_cmd_members);


    // loop
    tok = strtok(str, delimiter);
    i = 0;

    while(i < c->nb_cmd_members)
    {
        // add to c->cmd_members[i]
        duplicate = strdup(tok);
        c->cmd_members[i] = trim(duplicate);
        printf("\nMEMBER: -%s-", c->cmd_members[i]);
        free(duplicate);

        // get next member
        tok = strtok(NULL, delimiter);
        i++;
    }
}

//Remplit les champs redir et type_redir
void parse_redirection(unsigned int i, cmd *c)
{
    int numberOfChevron = 0;
    char * flux = NULL, * sub = NULL;
    char member[256] = {0};

    size_t sizeOfMember = strlen(c->cmd_members[i]);
    strcpy(member, c->cmd_members[i]);

    if ((c->redirection[i] = (char **) calloc(3, sizeof(char *))) == NULL)
    {
        printf("Error !");
        exit(-1);
    }

    if ((c->redirection_type[i] = (int *) calloc(2, sizeof(int))) == NULL)
    {
        printf("Error !");
        exit(-1);
    }

    // Initialisation des types de flux.

    c->redirection_type[i][STDIN] = -1;
    c->redirection_type[i][STDOUT] = -1;
    c->redirection_type[i][STDERR] = -1;

    // Recherche des flux.

    // STDIN.

    printf("\nMEMBER(2): -%s-\n", c->cmd_members[i]);

    if ((flux = strchr(member, '<')) != NULL)
    {
        sub = subString(flux + 1, flux + strlen(flux));

        strcpy(flux, sub);
        free(sub);

        // Il peut y avoir un ou plusieurs chevrons.
        if (strchr(flux, '<') != NULL)
        {
            sub = subString(flux + 1, flux + strlen(flux));

            strcpy(flux, sub);
            free(sub);
        }

        if (strchr(flux, '<') != NULL)
        {
            printf("Error in the redirection type !\n");
            exit(-1);
        }

        sub = trim(flux);

        strcpy(flux, sub);
        free(sub);

        if ((c->redirection[i][0] = (char *) malloc(sizeOfMember * sizeof(char))) == NULL)
        {
            printf("Error !");
            exit(-1);
        }

        strcpy(c->redirection[i][0], flux);
            printf("\nMEMBER(3): -%s-\n", c->cmd_members[i]);
    }

    // STDOUT.
    else if ((flux = strchr(member, '>')) != NULL)
    {
        printf("\nMEMBER(4.0): -%s-\n", c->cmd_members[i]);
        numberOfChevron++;

        sub = subString(flux + 1, flux + strlen(flux));

        strcpy(flux, sub);
        free(sub);
            printf("\nMEMBER(4.1): -%s-\n", c->cmd_members[i]);

        // Il peut y avoir un ou plusieurs chevrons.
        if (strchr(flux, '>') != NULL)
        {
            sub = subString(flux + 1, flux + strlen(flux));

            strcpy(flux, sub);
            free(sub);

            numberOfChevron++;
        }
            printf("\nMEMBER(4.2): -%s-\n", c->cmd_members[i]);

        if (strchr(flux, '>') != NULL)
        {
            printf("Error in the redirection type !\n");
            exit(-1);
        }
            printf("\nMEMBER(4.3): -%s-\n", c->cmd_members[i]);

        sub = trim(flux);

        strcpy(flux, sub);
        free(sub);

        if ((c->redirection[i][1] = (char *) malloc(sizeOfMember * sizeof(char))) == NULL)
        {
            printf("Error !");
            exit(-1);
        }
            printf("\nMEMBER(4.4): -%s-\n", c->cmd_members[i]);

        strcpy(c->redirection[i][1], flux);
        c->redirection_type[i][STDOUT] = 1;

        // S'il y a plusieurs chevrons, la redirection est de type "APPEND".
        // Sinon, elle est de type "OVERRIDE".
            printf("\nMEMBER(4.5): -%s-\n", c->cmd_members[i]);

        if (numberOfChevron > 1)
        {
            c->redirection_type[i][STDOUT] = APPEND;
        }
        else
        {
            c->redirection_type[i][STDOUT] = OVERRIDE;
        }
            printf("\nMEMBER(4): -%s-\n", c->cmd_members[i]);
    }
    else
    {
        c->redirection_type[i][STDERR] = 1;
    }
        printf("\nMEMBER(2): -%s-\n", c->cmd_members[i]);
}

char * subString(const char * start, const char * end)
{
    char * str = (char *) malloc(( end - start + 1 ) * sizeof(char));

    if(str == NULL)
        fatalError("Error: malloc()\n");

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

    return subString(str+i, end);
}

void fatalError(const char * msg)
{
    printf("\n%s\n", msg);
    exit(-1);
}
