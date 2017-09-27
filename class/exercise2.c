/***************************************************
    Exercise 1.2
    Rewrite the following program using good style

    Name: Mina Yi
    Date: April 14, 2011   
***************************************************/ 
#include <stdio.h>

int main (void)
{
    int countOdd(int a[], int n);
    int a[100] = {1, 2, 3, 4, 5, 4, 3, 2, 1, 9};
    int n = 10, count;

    count = countOdd(a, n);
/*
    if (countOdd(a, n) != 0)
        if(countOdd(a, n) == 1)
        printf("%d odd number\n", countOdd(a, n));
      else
        printf("%d odd numbers\n", countOdd(a, n));
    else
      printf("No odd numbers\n");
*/
    
    if(count)
        printf("%d odd number%c.\n", count, count > 1?'s':' ');
    else printf("No odd numbers.\n");
        
    return 0;
}
/***************************************************
    Counts the odd elements in an arrays of ints
*/

int countOdd(int a[], int n)
{
    int i = 0, count = 0;

    for (i = 0; i < n; i++)
        if(a[i] % 2 != 0)
            count++;

    return count;
}     

