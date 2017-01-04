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
void free_members_args(cmd *c){
    //your implementation comes here
}

//Prints the contents of members to the console
void print_members(cmd *c){
    //your implementation comes here
}

//Frees the memory allocated to store member information
void free_members(cmd *c){
    //your implementation comes here
}

//Prints the redirection information for member i
void print_redirection(cmd *c, int i){
    //your implementation comes here
}

//Frees the memory allocated to store redirection info
void free_redirection(cmd *c){
    //your implementation comes here
}

//Remplit les champs cmd_args et nb_args_membres
void parse_members_args(cmd *c){

}

//Remplit les champs initial_cmd, membres_cmd et nb_membres
void parse_members(char *s,cmd *c){
    //your implementation comes here
}

//Remplit les champs redir et type_redir
void parse_redirection(unsigned int i, cmd *c){
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
