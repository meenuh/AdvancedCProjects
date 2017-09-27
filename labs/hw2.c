/*
 * This program takes a file and enters the stock ticker symbol into a
 * circularly doubly linked list which contains one sentinel. It pushes the
 * prices into a stack. It allows the user to enter a stock ticker symbol and
 * the number of latest prices the user wants to see and prints it until the
 * user enters the sentinel to skip. It then asks the user for 2 stock ticker
 * symbols and prints all the results to and from the first to second along
 * with the most recent price. It stops when the user enters the sentinel.
 *
 * Mina Yi
 * Homework 2
 * May 3, 2011
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "perry.h"

#define FLUSH while(getchar() != '\n')
#define MAXLINE 6
#define SENTINEL '\177'

typedef struct snode S_NODE;
typedef struct lnode L_NODE;
    
struct snode
{
    float price;
    S_NODE *next;
};

struct lnode
{
    char *stockSym;
    S_NODE *stack;                      
    L_NODE *forw;
    L_NODE *back;
};

void checkAndGetArgs(int argc, char **argv, FILE **fpin);
L_NODE *initList();
void createList(FILE *fpin, L_NODE *list);
L_NODE *insert(L_NODE *list, char *symbol);
S_NODE *pushPrice(S_NODE *stack, float price);
L_NODE *find(char *symbol, L_NODE *list);
void upper(char *symbol);
void freeMemory(L_NODE *list);

int main(int argc, char **argv)
{
    FILE *fpin;
    L_NODE *list, *listItr, *startNode, *endNode;
    S_NODE *stackItr;
    char userInput[MAXLINE];
    int count, i;

    float price; 
    char stockSym[MAXLINE];

    checkAndGetArgs(argc, argv, &fpin);
    list = initList();
    createList(fpin, list); 
   
    while(printf("Enter symbol or * to skip:  \n"),
            scanf("%5s", userInput), strcmp(userInput, "*") != 0)
    {
        FLUSH;
        
        upper(userInput);

        listItr = find(userInput, list);

        if(strcmp(userInput, listItr->stockSym) == 0)
        {
            stackItr = listItr->stack;
            count = -1;
            while(count < 0)
            {
                printf("Enter number of quotes: \n");
                scanf("%d", &count);
                FLUSH;
            }

            for (i=0; i < count; i++)
            {   
                if (stackItr->next == NULL)
                {
                    printf("%d requested. Only %d found.\n", count, i);
                    break;
                }
                printf("%.2f\n", stackItr->price);
                stackItr = stackItr->next;
            }
        } else printf("Not found.\n");
    }

    do {
        while(printf("Enter first ticker symbol or * to quit: \n"), 
                scanf("%5s", userInput))
        {
            FLUSH;

            if (strcmp(userInput, "*") == 0)
                break;

            upper(userInput);

            startNode = find(userInput, list);

            if(strcmp(userInput, startNode->stockSym) != 0)
                printf("Not found.\n");
            else break;
        }

        if (*userInput != '*') 
        {
            while(printf("Enter second ticker symbol or * to quit: \n"), 
                    scanf("%5s", userInput))
            {
                FLUSH;

                if (strcmp(userInput, "*") == 0)
                    break;

                upper(userInput);

                endNode = find(userInput, list);

                if (strcmp(userInput, endNode->stockSym) != 0)
                    printf("Not found.\n");
                else break;
            }
        }

        if(*userInput != '*')
        {
            if(strcmp(startNode->stockSym, endNode->stockSym) < 0)
            {
                stackItr = startNode->stack;
                while(startNode != endNode->forw)
                {
                    printf("%s: %.2f\n", startNode->stockSym, stackItr->price );
                    startNode = startNode->forw;
                    stackItr = startNode->stack;;
                }

            } else {
                stackItr = endNode->stack;
                while(startNode != endNode->back)
                {
                    printf("%s: %.2f\n", startNode->stockSym, stackItr->price);
                    startNode = startNode->back;
                    stackItr = startNode->stack;
                }
            }
        }
    } while (*userInput != '*');
   
    freeMemory(list);
    return 0;
}

/*
 * checAndGetArgs takes in argc, argv, and the address of fpin. It opens the
 * file for reading via command line argument. This function does not return
 * anything.
 */
void checkAndGetArgs(int argc, char **argv, FILE **fpin)
{
    if(argc != 2)
    {
        printf("Incorrect number of command line arguments!\n");
        exit(1);
    }
    *fpin = Fopen(argv[1], "r");

    return;
}

/*
 * initList does not take any arguments. It initializes the first node in
 * the circularly doubly linked list to the sentinel. It sets the forw and back
 * pointers to point to itself. It returns list.
 */
L_NODE *initList()
{
    L_NODE *list;

    list = (L_NODE *)Malloc(sizeof(L_NODE));
    list->stockSym = (char *)Malloc(2);

    *list->stockSym = SENTINEL;

    list->forw = list;
    list->back = list;

    return list;
}

/*
 * createList takes in the file pointer fpin and the circularly doubly linked
 * list called list. It scans in the name of the ticker symbol and the price
 * and calls insert to insert the new node into the circularly doubly linked
 * list. After it has been inserted or not inserted because it is a duplicate,
 * it uses the stack pointer to push the price into stack.
 */
void createList(FILE *fpin, L_NODE *list)
{
    char sBuffer[MAXLINE];
    L_NODE *new;
    float price;

    while(fscanf(fpin, "%s %f", sBuffer, &price) != EOF)
    {
        new = insert(list, sBuffer);
        new->stack = pushPrice(new->stack, price);
    }
    Fclose(fpin);
    return;
}

/*
 * insert takes in the list and the ticker symbol. It calls the find function
 * to find the position the node should be inserted to and assigns that postion
 * to current. It compares the stockSym and the symbol, if it compares to 0,
 * that means it already exists and so it assigns the position of current to
 * new. If it does not compare to 0, it creates a new node, copys the string to
 * the node, reassigns the pointers to link it in, and it initiliazes the new
 * stack since it did not exist before. It returns new.
 */
L_NODE *insert(L_NODE *list, char *symbol)
{
    L_NODE *new, *current;

    current = find(symbol, list);

    if(strcmp(symbol, current->stockSym) == 0)
        new = current;
    else
    {
        new = (L_NODE *)Malloc(sizeof(L_NODE));
        new->stockSym = (char *)Malloc(strlen(symbol) + 1);
        strcpy(new->stockSym, symbol);

        new->forw = current;
        new->back = current->back;
        current->back->forw = new;
        current->back = new;

        new->stack = (S_NODE *)Malloc(sizeof(S_NODE));
        new->stack->next = NULL;
    }
    return new;
}

/*
 * pushPrice takes in the stack pointer and the price. It creates a new node
 * and puts the price in and reassigns the pointer. It then returns the stack
 * pointer.
 */
S_NODE *pushPrice(S_NODE *stack, float price)
{
    S_NODE *new;

    new = (S_NODE *)Malloc(sizeof(S_NODE));
    new->price = price;

    new->next = stack;
    stack = new;

    return stack;
}

/*
 * The find function traverses the circularly doubly linked list to find the
 * desired stock symbol and also finds the position it should be in when
 * inserting. It returns a pointer (mover) to the position.
 */
L_NODE *find(char *symbol, L_NODE *list)
{
    L_NODE *mover = list->forw;

    while(strcmp(symbol, mover->stockSym) > 0)
        mover = mover->forw;

    return mover;
}

/*
 * freeMemory takes in the circularly doubly linked list. It traverses it,
 * frees the stack, then the string and the linked list node itself. This
 * function does not return anything.  
 */
void freeMemory(L_NODE *list)
{
    L_NODE *lmover = list->forw;
    S_NODE *sprev = lmover->stack;
    S_NODE *scur = sprev->next;

    while(*lmover->stockSym != SENTINEL)
    {
        while(sprev != NULL)
        {
            free(sprev);
            sprev = scur;
            if(scur)
                scur = scur->next;
        }
        lmover = lmover->forw;
        free(lmover->back->stockSym);
        free(lmover->back);
    }  

    free(lmover->stockSym);
    free(lmover);
} 

/*
 * upper takes the string from the user and uppers it in order to ensure user
 * validation. This function does not return anything.
 */
void upper(char *symbol)
{
    char *mover;

    for(mover = symbol; *mover; mover++)
        *mover = toupper(*mover);
}
