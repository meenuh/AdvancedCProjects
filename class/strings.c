/*
 * Mina Yi
 * Strings extra credit
 *
 */

#include <stdio.h>
#include <string.h>

struct words
{
    char **wrd;
    char **ant;
}WORDS;

int main()
{
    char *wrd[5] = {"good", "ugly", "sunny", "sad", NULL};
    char *ant[5] = {"bad", "nice", "cloudy", "happy", NULL};

    struct words *ptr = &WORDS;
    struct words *aptr = &WORDS;
    int i, j;

    WORDS.wrd = wrd;
    WORDS.ant = ant;

    printf("%s\n", *ptr->wrd);
    printf("%c\n", *(*(ptr->ant + 3) + strlen(*(ptr->ant + 3)) - 1));

    putchar('\n');

    for(i = 0; *(ptr->wrd + i); i++)
        printf("%s:%s\n", *(ptr->wrd + i), *(aptr->ant + i));

    return 0;
}
