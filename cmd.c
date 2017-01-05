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
        // On saute l'espace de fin.
        char * member = subString(s, pipe - 1);

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
    char * flux = NULL;
    int numberOfChevron = 0;

    size_t sizeOfMember = strlen(c->cmd_members[i]);

    if ((c->redirection[i] = malloc(3 * sizeof(char *))) == NULL)
    {
        printf("Error !");
        exit(-1);
    }

    // Recherche des flux.

    // UTILISER FONCTION ROBIN POUR SUPPRIMER ESPACE AU DEBUT ET A LA FIN !!

    // STDIN.
    if ((flux = strchr(c->cmd_members[i], '<')) != NULL)
    {
        flux = subString(flux + 1, flux + strlen(flux));

        // Il peut y avoir un ou plusieurs chevrons.
        while (strchr(flux, '<') != NULL)
            flux = subString(flux + 1, flux + strlen(flux));

        if ((c->redirection[i][0] = malloc(sizeOfMember * sizeof(char))) == NULL)
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

        // Il peut y avoir un ou plusieurs chevrons.
         while (strchr(flux, '>') != NULL)
         {
            flux = subString(flux + 1, flux + strlen(flux));

            numberOfChevron++;
         }

        if ((c->redirection[i][1] = malloc(sizeOfMember * sizeof(char))) == NULL)
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
            c->redirection[i] = APPEND;

        else
        c->redirection[i] = OVERRIDE;
    }

    /*if (c->redirection[i][0] != NULL)
            printf("%s\n", c->redirection[i][0]);

    if (c->redirection[i][1] != NULL)
        printf("%s\n", c->redirection[i][1]);*/
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
