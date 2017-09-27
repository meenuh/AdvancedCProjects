/*
 * This program takes an input file name from the user and hashes to disk. It
 * allows the user to pick from a menu. They may add, look up or quit.
 * 
 * Mina Yi
 * Homework 3
 * May 19, 2011
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "perry.h"

#define MAXLINE 512
#define BUCKETS 4
#define BUCKETSIZE 240
#define RECORDSIZE 60
#define CITYMAX 49
#define RECS_PER_BUCKET 4
#define BLANK -1
#define BADLINE -2
#define GOODADD -3
#define BADFILE -4
#define NOTFOUND -5
#define FOUND -6
#define ADD 1
#define LOOKUP 2
#define QUIT 3

FILE *openWeatherFile();
int menu();
int addRecordsToDB(FILE *fp);
void insert(char *key, int hash, FILE *fp);
int hashkey(char *cityState);
int lookupRecord(FILE *fp);
int find(FILE *fp, char *search, int hash);
void upperinput(char *userinput);

int main()
{
    FILE *fp;
    int result, option;

    fp = openWeatherFile();

    while((option = menu()) != QUIT)
    {
        switch(option)
        {
            case BLANK: continue;
            case BADLINE: continue;
            case BADFILE: continue;
            case ADD:
                while((result = addRecordsToDB(fp)) == BADLINE || result == BADLINE);
                break;
            case LOOKUP:
                while((result = lookupRecord(fp)) == BADLINE || result == NOTFOUND);
                break;
        }
    }
    Fclose(fp);
    return 0;
}

/*
 * The openWeather function does not take any arguments. It opens a file for
 * reading and writing. It returns a FILE *.
 */
FILE *openWeatherFile()
{
    FILE *fp;

    fp = fopen("hw3out.txt", "r+");
    return fp;
}

/*
 * The menu function does not take any arguments. It prints the menu and allows
 * the user to choose from the menu. It validates for valid numbers and input.
 * It returns BADLINE if the user's input has invalid characters or if the menu
 * selection is invalid. It returns BLANK if the user does not enter anything.
 * If the user's selection is valid it returns their menu choice.
 */
int menu()
{
    char userInput[CITYMAX], *delim;
    int selection;

    printf("1. Add records to database.\n");
    printf("2. Look up a record.\n");
    printf("3. Quit.\n\n");

    printf("Enter selection: \n");
    fgets(userInput, CITYMAX, stdin);

    if(*userInput == '\n')
        return BLANK;

    delim = strtok(userInput, "\040\t\n");
    if(strtok(NULL, "\040\t\n") != NULL)
    {
        printf("Legal entry has one selection only.\n");
        return BADLINE;
    }

    strtol(userInput, &delim, 10);
    if(*delim)
    {
        printf("Invalid character in selection.\n");
        return BADLINE;
    }

    sscanf(userInput, "%d", &selection);
    if(selection < 0 || selection > 3)
    {
        printf("Invalid menu selection.\n");
        return BADLINE;
    }

    return selection;
}

/*
 * the addRecordsToDB function takes in the FILE * of the file that is to be
 * hashed it. It takes an input file name from the user and adds the records
 * from the file to the hash file. This function calls hashkey to generate the
 * hashkey and insert to insert to the hash file. It closes the file when it is
 * done and returns GOODADD when the add is successful or BADFILE if the file
 * the user enters can not be opened.
 */
int addRecordsToDB(FILE *fp)
{
    char buffer[MAXLINE], cityState[CITYMAX]; 
    char *city, *state, *temp;
    FILE *batchFp;
    int hashval;

    printf("Enter file name.\n");
    fgets(buffer, MAXLINE, stdin);
    *strchr(buffer, '\n') = '\0';

    if((batchFp = fopen(buffer, "r")) == NULL)
    {
        printf("Batch insertion file could not be opened.\n");
        return BADFILE;
    }
        
    while(fgets(buffer, MAXLINE, batchFp))
    {
        city = strtok(buffer, ",");
        state = strtok(NULL, ",");
        temp = strtok(NULL, "\n");
       
        sprintf(cityState, "%s,%s", city, state);

        hashval = hashkey(cityState);
        sprintf(buffer, "%s,%s", cityState, temp);
        insert(buffer, hashval, fp);
    }
    Fclose(batchFp);
    return GOODADD;
}

/*
 * The insert function takes in the city/state (key), the number this
 * particular key hashes to and the FILE * for the hash file. It seeks to the
 * position given by the hash and searches through the bucket for an empty
 * spot. If the bucket is full it prints a message. This function does not
 * return anything.
 */
void insert(char *key, int hash, FILE *fp)
{
    char temp[RECORDSIZE + 1];
    int i;

    fseek(fp, hash * BUCKETSIZE, SEEK_SET);
    for(i = 0; i < RECS_PER_BUCKET; i++)
    {
        fgets(temp, RECORDSIZE + 1, fp);
        if(strncmp(key, temp, strlen(key)) == 0)
        {
            printf("Duplicate entry: %s.\n", key);
            return;
        }
        if(*temp == ' ')
        {
            fseek(fp, -1L * RECORDSIZE, SEEK_CUR);
            fputs(key, fp);
            return;
        }
    }
    printf("Bucket overflow! Record for %s rejected.\n", key);
}

/*
 * The hashkey function takes the city/state key given by the user and
 * generates a key from it. It returns the hash key.
 */
int hashkey(char *cityState)
{
    long hash = 0;
    char *mover;

    for(mover = cityState; *mover; mover++)
        hash += (*mover) * (*mover) * (*mover);
  
    return hash % 100;
}

/*
 * The lookupRecord function takes in the FILE * to the hash file. The user
 * enters the city,state they would like to search. It validates user input and
 * returns BLANK or BADLINE accordingly. This function calls hashkey to
 * generate the key and find to find the user's search city,state. It returns
 * BLANK if the user input is blank, BADLINE if the input is invalid or FOUND
 * or NOTFOUND that is returned by the find function.
 */
int lookupRecord(FILE *fp)
{
    char userInput[CITYMAX], accept[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ,\040\n";
    char *city, *state, *city_pre, *city_post, *state_post;
    int hashval, len;

    printf("Enter city,state.\n");
    fgets(userInput, CITYMAX, stdin);

    if(*userInput == '\0')
        return BLANK;

    upperinput(userInput);
    
    len = strspn(userInput, accept);
    if(strlen(userInput) != len)
    {
        printf("Illegal characters.\n");
        return BADLINE;
    }
    
    city = strtok(userInput, ",");
    while(*city == '\040')
        city++;
   
    if(*city == '\0')
    {
        printf("Missing city.\n");
        return BADLINE;
    }

    state = strtok(NULL, "\n\040");
    while(state != NULL && *state == '\040')
        state++;
    
    if(state == NULL || *state == '\0')
    {
        printf("Missing state.\n");
        return BADLINE;
    }
    
    if(strchr(state, ',') != NULL)
    {
        printf("Illegal extra token.\n");
        return BADLINE;
    }
    
    state_post = strtok(NULL, "\n\040");
    city_pre = strtok(city, "\040");
    city_post = strtok(NULL, "\040");

    if(city_post && state_post)
        sprintf(userInput, "%s %s,%s %s", city_pre, city_post, state, state_post);
    else if(city_post && !state_post)    
        sprintf(userInput, "%s %s,%s", city_pre, city_post, state);
    else if(!city_post && state_post)
        sprintf(userInput, "%s,%s %s", city_pre, state, state_post);
    else
        sprintf(userInput,"%s,%s", city_pre, state);

    hashval = hashkey(userInput);
    return find(fp, userInput, hashval);
}

/*
 * The find function takes in the FILE * to the hash file, the user input to
 * search for and the hash key. This function returns FOUND or NOTFOUND in
 * accordance to whether the city,state the user is searching for exists or
 * not.
 */
int find(FILE *fp, char *search, int hash)
{
    char temp[RECORDSIZE + 1];
    int i;

    fseek(fp, hash * BUCKETSIZE, SEEK_SET);

    for(i = 0; i < RECS_PER_BUCKET; i++)
    {
        fgets(temp, RECORDSIZE, fp);
        if(strncmp(temp, search, strlen(search)) == 0)
        {
            fseek(fp, -1L * RECORDSIZE, SEEK_CUR);
            printf("Found: %s\n", temp);
            return FOUND;
        }
    }

    printf("Not found.\n");
    return NOTFOUND;
}

/*
 * This function takes in the userInput and uppercases it to ensure the best
 * possibility for their search to be found.
 */
void upperinput(char *userinput)
{
    char *mover;

    for(mover = userinput; *mover; mover++)
        *mover = toupper(*mover);
}


