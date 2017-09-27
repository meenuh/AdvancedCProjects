/***************************************************
    Exercise 1.4
    Rewrite the following program using good style

    Name: Mina Yi
    Date: April 14, 2011   
***************************************************/ 
#include <stdio.h>

int main (void)
{
    int checkTriangle(double a, double b, double c);
    double a = 10, b = 2, c = 3;
    //double a = 10, b = 20, c = 15;
    
    printf("%.1f %.1f %.1f are %sthe sides of a triangle.\n"
            , a, b, c, checkTriangle(a, b, c)?"":"not ");
        
    return 0;
}
/***************************************************
    Checks if a, b, and c are the sides of 
    a triangle
*/
int checkTriangle(double a, double b, double c)
{
    int check = 0;

    if(a > 0 && b > 0 && c > 0)
        if(a < b + c && b < a + c && c < a + b)
            check = 1;

/*
    if (a > 0)
        if (b > 0)
            if (c > 0)
                if (a < b + c)
                    if (b < a + c)
                        if (c < a + b)
                            check = 1;
                        else
                            check = 0;
                    else
                        check = 0;
                else
                    check = 0;
            else
                check = 0;
        else
            check = 0;
    else
        check = 0;
    return check;
*/
    return check;
}     

