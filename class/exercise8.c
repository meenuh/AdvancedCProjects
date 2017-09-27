/***************************************************
    Exercise 1.8
    A. This program does not compile. 
       Explain why, and fix it.
       It does not compile because you are trying to
       copy directly into the strings. You must use 
       strcpy in order for it to work.
    B. Then rewrite the swap function using 
       good style.
    
    Name: Mina Yi
    Date: April 14, 2011
***************************************************/ 
#include <stdio.h>
struct book{
	char ISBN[15];
	char *author;
	int  year;
    int  nopages;
};

int main(void)
{
    void printList(struct book list[]);
    void swap (struct book *a, struct book *b);
    struct book list[10] = 
    {
        {"0170088219", "Ron House", 1994, 568},
        {"0534951406", "John W. Perry", 1998, 304},
        {"0131103628", "Dennis M.Ritchie", 1988, 272},
        {"", NULL, 0, 0},
    };
    
    printList(list);
    swap(&list[0], &list[1]);
    printList(list);

    return 0;
}

/***************************************************
    Prints a list of book structures
*/
void printList(struct book list[])
{
    int i;

    putchar('\n');
    for (i = 0; list[i].author != NULL; i++)
    {
        printf( "%10s %-20s %d %d\n", (*(list + i)).ISBN, 
                                      (*(list + i)).author, 
                                      (*(list + i)).year, 
                                      (*(list + i)).nopages);
    }
    return;
}

/***************************************************
    swap two book structures
*/
void swap ( struct book *a, struct book *b )
{
     struct book temp;

	 temp = *a;
     *a = *b;
     *b = temp;

	return;
}
