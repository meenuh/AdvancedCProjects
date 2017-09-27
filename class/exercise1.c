/***************************************************
    Exercise 1.1
    Rewrite the following program using good style

    Name: Mina Yi
    Date: April 14, 2011
***************************************************/ 
#include <stdio.h>

int main (void)
{

    int a = 10, c = 0;
/*    
    if (a > 0)
    {
        c++;
        printf("%d %d\n", a, c);
    }
    else
    {
        a = -a;
        printf("%d %d\n", a, c);
    }
*/

    if(a > 0)
        c++;
    printf("%d %d\n", a, c);

    return 0;
}
