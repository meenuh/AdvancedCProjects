/***************************************************
    Exercise 1.7
    Rewrite the following program using good style
    
    Name: Mina Yi
    Date: April 14, 2011
***************************************************/ 
#include <stdio.h>

int main(void)
{
    int multTable(int x, int y);
    int x, y;

    for (x = 10, y = 20; x < 15; x++, y += 10)
        printf("%d\t%d\t\t%d\n", x, y, multTable(x, y));

    return 0;
}

/***************************************************
    Guess what does this function calculate
*/
int multTable(int x, int y) 
{
    int sum = 0; 

    while(x)
    {
        if(x % 2)
            sum += y;
        x /= 2;
        y *= 2;
    }
    return sum;
}
