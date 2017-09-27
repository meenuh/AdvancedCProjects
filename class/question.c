#include <stdio.h>

int main (void)
{
    int strlen(char *s);
    char word[31] = "";
    char word[31] = "demonstrate";
    char *ps = "The insides of the ANSI C strlen function.";

    printf("\"%s\" has %d characters\n", word, strlen(word));
    printf("\"%s\" has %d characters\n", ps, strlen(ps));
    return 0;
}

/***************************************************
  Returns the length of a string
  */
int strlen(char *s)
{
    char *ptr = s;

    //while (*ptr != '\0') ptr++;
    //while (*ptr) ptr++;
    while (*ptr++);     // ???
    return ptr - s;
}
