#include<stdio.h>

int main()
{
    FILE *fp;
    char record[61];
    int i;

    fp = fopen("hw3out.txt", "w");
    for(i = 0; i < 59; i++) record[i] = ' ';
    record[59] = '\n';
    record[60] =  '\0';

    for(i = 0; i < 400; i++)
        fprintf(fp, "%s", record);

    return 0;
}
