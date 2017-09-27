/***************************************************
    Exercise 1.3
    Rewrite the following program using good style

    Name: Mina Yi
    Date: 4/12/11   
***************************************************/ 
#include <stdio.h>

int main (void)
{
    //int findMin(int a, int b, int c, int d);
    //int a = 10, b = 20, c = 5, d = 7;
    int findMin(int num1, int num2, int num3, int num4);
    int a = 10, b = 2, c = 5, d = 7;
    
    //findMin(a, b, c, d);

    printf("%d %d %d %d\n", a, b, c, d);
    printf("The smallest value is %d\n", findMin(a, b, c, d));
    //printf("The smallest value is %d.\n", min);

    return 0;
}
/***************************************************
    Find smallest
*/
int findMin(int num1, int num2, int num3, int num4)
{    
    int min = num1;

    if(num2 < min)
        min = num2;
    else if(num3 < min)
        min = num3;
    else if(num4 < min)
        min = num4;

/*
    if (a <= b && a <= c && a <= d )
        min = a;
    if (b <= a && b <= c && b <= d )
        min = b;
    if (c <= a && c <= b && c <= d )
        min = c;
    if (d <= a && d <= b && d <= c )
        min = d;
    return min;
*/
}


