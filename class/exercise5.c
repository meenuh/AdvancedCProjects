/***************************************************
    Exercise 1.5
    Rewrite the following program using good style

    Name: Mina Yi
    Date: April 14, 2011   
***************************************************/ 
#include <stdio.h>

int main (void)
{
    int checkLetter(char aChar);
    //char c = '*';
    char c = 'a';
    //char c = 'B';

    printf("%c is%s a letter!\n", c, checkLetter(c)?"":" not");
/*    
    printf("%c is ", c);
    if (checkLetter(c) == 0)
        printf("not ");
    printf(" a letter\n");
*/        
    return 0;
}
/***************************************************
    Checks if a character is a letter or not
*/
int checkLetter(char aChar)
{    
    int isLetter = 0;

    if((aChar >= 'a' && aChar <= 'z') ||
        (aChar >= 'A' && aChar <='Z'))
            isLetter = 1;
/*
    if (aChar >= 'a' && aChar <= 'z')
        isLetter = 1;
    else 
        if (aChar >= 'A' && aChar <= 'Z')
            isLetter = 1;
        else
            isLetter = 0;
*/
    return isLetter;
}
