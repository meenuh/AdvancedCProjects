/*
 * Mina Yi
 * May 6, 2011
 */

#include <stdio.h>
#include <stdlib.h>  // exit
#include <string.h>
#include <ctype.h>
#define FILENAME "io.dat"

int main(int argc, char *argv[])
//int main(void)
{
    FILE *fp;
    char table[5][64] = {"One", "Two", "Three", "Four", "Five"};
    char temp[5][64], word[64], *p;
    long pos;
    char new[7][64];
    int r, k, n = 3;

    if ((fp = fopen(argv[1], "w+b")) == NULL)
        if ((fp = fopen(FILENAME, "w+b")) == NULL)
        {
            printf("Could not open %s.\n", argv[1]);
            printf("Could not open %s.\n", FILENAME);
            exit(1);
        }

    fwrite(table, 64, 5, fp);
    rewind(fp);
    fread(temp, 64,5,fp);

    for(k = 0; k < 5; k++)
            printf("%s\n", temp[k]);

    rewind(fp);
   
    /* move to beginning of the last record in the file*/
    fseek(fp, -1 * 64, SEEK_END);
    fread(temp, 64,1,fp);
    printf("%s", temp);    

    /*move to the beginning of record n*/
    fseek(fp, (n - 1) * 64, SEEK_SET);
    fread(temp, 64, 1, fp);
    printf("%s\n", temp);
    
    /*move to the beginning of the first record, then forward one record*/
    rewind(fp);
    fseek(fp, 1 * 64, SEEK_SET);
    fread(temp, 64, 1, fp);
    printf("%s\n", temp);
    
    /*
     * move to the beginning of the last record in the file, then move backwards
     * two records
     */
    fseek(fp, -1 * 64, SEEK_END);
    fseek(fp, -2 * 64, SEEK_CUR);
    fread(temp, 64, 1, fp);
    printf("%s\n", temp);
    
    /*replace the nth element with "DONE"*/
    fwrite("DONE", 64, 1, fp = (fp + fseek(fp, (n-1) * 64, SEEK_SET)));
    fread(temp, 64, 1, fp = (fp + fseek(fp, -1 * 64, SEEK_CUR)));
    printf("%s\n", temp);
    
    /*change the last letter in the last word to uppercase*/
    fread(word, 64, 1, fp = (fp + fseek(fp, -1 * 64, SEEK_END)));
    r = strlen(word) - 1;
    *(word + r)= toupper(*(word + r));
    fwrite(word, 64, 1, fp = (fp + fseek(fp, -1 * 64, SEEK_CUR)));
    fread(word, 64, 1, fp = (fp + fseek(fp, -1 * 64, SEEK_CUR)));
    printf("%s\n", word);
    
    /*change the last letter in the last word to uppercase*/
    fwrite("DONE", 64, 1, fp = (fp + fseek(fp, 0, SEEK_END)));
    fwrite("DONE", 64, 1, fp = (fp + fseek(fp, 0, SEEK_END)));
      
    /*calculate the number of records in the file*/  
    fseek(fp, 0, SEEK_END);  
    printf("%d\n", ftell(fp)/64);
    rewind(fp);
    
    fread(new, 64, 7, fp);

    for(k = 0; k < 7; k++)
        printf("%s\n", new[k]);

    fclose(fp);
    return 0;
}
