#include <stdio.h>
#include <string.h>
#include "perry.h"

void func(char *string);

int main()
{
    char *string[5];

    //func(string);
    printf("%d`");
    return 0;
}

void func(char **string)
{
    int elem = 0;

    elem = sizeof(string)/sizeof(char *);
    printf("%d\n", elem);

}
