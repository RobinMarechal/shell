#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "helpers.h"

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
