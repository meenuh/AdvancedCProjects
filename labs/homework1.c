/*
 * This program takes a file and parses and inserts the data into an array 
 * of structs called records. It sorts the data by city/state and then
 * averages the temperatures of matching city/states and writes it out to
 * a file.
 *
 * Mina Yi
 * HW 1
 * April 13, 2011 
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "perry.h"

#define BUCKET 5
#define MAXLINE 100

typedef struct
{
    char *stateCity;
    float temp;
}TEMPREC;

void checkAndGetArgs(int argc, char **argv, FILE **fpin, FILE **fpout);
TEMPREC *getRecords(FILE *fpin, int *count);
void sortByCityState(TEMPREC *records, TEMPREC *last);
void calcAverages(TEMPREC *records, TEMPREC *last, FILE *fpout);
void freeMemory(TEMPREC *last, TEMPREC *records);

int main(int argc, char **argv)
{
    FILE *fpin, *fpout;
    TEMPREC *records, *last;
    int count = 0;

    checkAndGetArgs(argc, argv, &fpin, &fpout);
    records = getRecords(fpin, &count);

    last = records + count;

    sortByCityState(records, last);
    calcAverages(records, last, fpout);
    freeMemory(last, records);

    return 0;
}

/*
 * checkAndGetArgs takes in argc, argv, the address of fpin and fpout.
 * It opens the files for writing or reading via command line argument.
 * This function does not return anything.
 */
void checkAndGetArgs(int argc, char **argv, FILE **fpin, FILE **fpout)
{
    if(argc != 3)
    {
        printf("Incorrect number of command line arguments!\n");
        exit(1);
    }

    *fpin = Fopen(argv[1], "r");
    *fpout = Fopen(argv[2], "w");

    return;
}

/*
 * getRecords takes in fpin (which has been opened for reading
 * by checkAndGetArgs) and address of count. It gets lines from the file,
 * parses it and assigns the data to the members in the structs. It uses 
 * the bucketing scheme to realloc space when necessary. It returns the
 * newly filled array of structs.
 */
TEMPREC *getRecords(FILE *fpin, int *count)
{
    TEMPREC *mover, *records = NULL;
    char line[MAXLINE], *colon;
    float temperature;

    while(fgets(line, MAXLINE, fpin))
    {
        if(*count % BUCKET == 0)
        {
            records  = (TEMPREC *)Realloc(records, (*count + BUCKET) * sizeof(TEMPREC));
            mover = records + *count;
        }

        colon = strrchr(line, ':');
        *colon = '\0';

        mover->stateCity = (char *)Malloc(strlen(line) + 1);
        sscanf(colon + 1, "%f", &temperature);

        strcpy(mover->stateCity, line);
        mover->temp = temperature;

        (*count)++;
        mover++;
    }

    records = (TEMPREC *)Realloc(records, (*count) * sizeof(TEMPREC));

    Fclose(fpin);
    return records;
}

/*
 * sortByCityState takes in last which points to the last element in the
 * array of structs and the array of structs called records itself. It 
 * uses the insertion sort to sort the array by city state. It does not 
 * return anything.
 */
void sortByCityState(TEMPREC *records, TEMPREC *last)
{
    TEMPREC temp, *mover, *current;
    int located;

    for(current = records + 1; current < last; current++)
    {
        located = 0;
        temp = *current;
        for(mover = current - 1; mover >= records && !located;)
        {
            if(strcmp(temp.stateCity, mover->stateCity) < 0)
            {
                *(mover + 1) = *mover;
                mover--;
            } else located = 1;     
        }
        *(mover + 1) = temp;
    }
    return;
}


/*
 * calcAverages calculates the averages of the city states' high temps.
 * It takes in the placement of the last element in the array of structs,
 * the array of structs(records) and fpout for the file it will be writing
 * out to. 
 *
 * match will be used to find all matches of the city states. Mover
 * will move through the array of structs to find where a different city 
 * state begins. It sums up and increases count for each mover city state 
 * matches the match city state. As soon as mover's city state does not 
 * return 0(which means it is a match), it will average the high temps for 
 * that state and write out to the file. It will then move match to 
 * mover's position and set average and count back to 0 for the new set 
 * of matching city states. This function does not return anything.
 */
void calcAverages(TEMPREC *records, TEMPREC *last, FILE *fpout)
{
    TEMPREC *mover = records, *match = mover;
    float average = 0; 
    int count = 0;

    while(mover < last)
    {
        while(mover < last && strcmp(mover->stateCity, match->stateCity) == 0)
        {
            average += mover->temp;
            count++;
            mover++;
        }
        average /= count;
        fprintf(fpout, "%s:%.2f\n", match->stateCity, average);

        match = mover;
        average = 0, count = 0;
    }

    Fclose(fpout);
    return;
}

/*
 * The freeMemory function takes in the position of the last element and 
 * passes recrods to this function. It then goes throug the array of structs
 * and frees all strings that were malloc'd. After it has done that, it frees
 * the entire array.
 */
void freeMemory(TEMPREC *last, TEMPREC *records)
{
    TEMPREC *mover;

    for(mover = records; mover < last; mover++)
        if(mover->stateCity)
            free(mover->stateCity);

    free(records);
    return;
}
