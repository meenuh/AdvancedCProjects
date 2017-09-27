#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DELIMS "#\040%()|=+=*?:;~&.0123456789,"

int main()
{
    char *ptr, *word;
    //char line[] = "int num = 10;";
    char temp[100];
    char *line = "printf(\"hello world\", num);";
   // char *line = "char *name = /*this is a comment*/";
   for(ptr = line; word = strtok(ptr, DELIMS); ptr = NULL)
   {
        printf("%s\n", word);
   }

    return 0;
}
