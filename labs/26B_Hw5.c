/***************************************************************************/
/*                                                                         */
/*  This program reads a C program file as input, parses identifiers and   */
/*  places them in a binary tree.  Each tree node has an identifier,       */
/*  the usual "left" and "right" pointers to other tree nodes, and "front" */
/*  and "rear" pointers to a queue of line number nodes.  Line number      */
/*  nodes contain a line number where the identifier occurred and a, of    */
/*  course, a pointer to the next line number node.                        */
/*                                                                         */
/*  After the tree is formed, the user is prompted to see if he wants an   */
/*  alphabetized cross-reference listing (followed by the original source  */
/*  file with line numbers).  If so, such a file is created with a time    */
/*  stamp at its top.  Each line of the cross-reference shows an identi-   */
/*  fier followed by all lines where it can be found in the source file.   */
/*                                                                         */
/*  After this, the user is prompted for an identifier (until he types     */
/*  "quit program").  The program will then output all source lines (with  */
/*  line numbers in the leftmost column) containing that identifier. Thus, */
/*  the program allows interactive references to the source code.          */
/*                                                                         */
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include "perry.h"

#define BLANK     ""
#define MAXIDENT  256
#define MAXLINE   100

typedef struct node NODE;
typedef struct line LINE;

struct line
{
   long  linenum;
   LINE *next;
};

struct node 
{
   char  *identifier;
   NODE  *left, *right;
   LINE  *front, *rear;
};


main(int argc, char *argv[])
{
   void  check_program_args(int argc, char *argv[],FILE **infile,FILE **outfile);
   void  printtree(NODE *tree, FILE *outfile);
   void  printprog(FILE *infile, FILE *outfile);
   void  print_time_stamp(FILE *outfile);
   NODE *parse_file_into_ident_tree(FILE *infile);
   NODE *insertTree(char *identifier, long line, NODE *tree);
   void  pushq(LINE **q, LINE **rear, long line);

   FILE *infile, *outfile;
   NODE *tree = NULL;

    check_program_args(argc, argv, &infile, &outfile);
    tree = parse_file_into_ident_tree(infile);
    //print_time_stamp(outfile); 
    printtree(tree, outfile);   /* Prints cross-reference table. */
    //printprog(infile, outfile); /* Prints input program with line numbers in
    //                             left column. */
}

/***************************************************************************/
/*                                                                         */
/*  This function checks to see that a filename argument was supplied by   */
/*  the user when invoking this program.  It also makes sure that the      */
/*  filename is that of a C source file (name ends with .c) and that the   */
/*  source file exists.                                                    */
/*                                                                         */
/***************************************************************************/
void check_program_args(int argc, char *argv[], FILE **infile, FILE **outfile)
{
  int filename_len;

  if (argc != 3)
  {
       perror("Wrong number of command line arguments!");
       exit(1);
  }

  filename_len = strlen(argv[1]);
  if (argv[1][filename_len - 1] != 'c' || argv[1][filename_len - 2] != '.')
  {
     printf("C source file names must end with \".c\".  Aborting!\n\n");
     exit(1);
  }

  *infile  = Fopen(argv[1],"r");
  *outfile = Fopen(argv[2],"w");
}

NODE *parse_file_into_ident_tree(FILE *infile)
{
    NODE *insertTree(char *identifier, long line, NODE *tree);
    NODE *tree = NULL;
    long count = 0;
    int quotes = 0 , comment = 0, identifier = 0, brackets = 0;
    char buffer[MAXIDENT], temp[MAXLINE], *buffmover, *tempmover, ignore_until = '\0';

    while(fgets(buffer, MAXIDENT, infile))
    {
        count++;
        for(buffmover = buffer, tempmover = temp; *buffmover; buffmover++)
        {
            if (!comment && !quotes && !brackets)
            {
                if (*buffmover == '/') 
                {
                    if (*(buffmover + 1) == '/')
                    {
                        while (*buffmover)
                            buffmover++;
                        break;
                    }
                    else if (*(buffmover + 1) == '*')
                    {
                        buffmover += 2;
                        comment = 1;
                        continue;
                    }
                }

                if(*buffmover == '"')
                {
                    quotes = 1;
                    continue;
                }
            }

            if (quotes && *buffmover == '"') {
                quotes = 0;
                continue;
            }

            if (comment && *buffmover == '/')
            {
                comment = 0;
                continue;
            }

            if (comment)
                continue;

            if(!identifier && isalpha(*buffmover))
            {
                identifier = 1;    
            }

            if(identifier) 
            {
                if(isalnum(*buffmover))
                {
                    *tempmover = *buffmover;
                    tempmover++;
                }

                if(!isalnum(*buffmover))
                {
                    identifier = 0;
                    tree = insertTree(temp, count, tree);
                    memset(temp, 0, MAXLINE);
                    tempmover = temp;
                }
            }

        }
    }

    return tree;
}

void pushq(LINE **q, LINE **rear, long line)
{
    LINE *new;

    new = (LINE *)Malloc(sizeof(LINE));
    new->linenum = line;
    new->next = NULL;

    if(*q == NULL)
        *q = new;
    else (*rear)->next = new;

    *rear = new;
}

NODE *insertTree(char *identifier, long line, NODE *tree)
{
    NODE **ptr_ref = &tree;
    int compare;

    while(*ptr_ref)
    {
        compare = strcmp(identifier, (*ptr_ref)->identifier);

        if(compare > 0)
            ptr_ref = &(*ptr_ref)->right;
        else if(compare < 0)
            ptr_ref = &(*ptr_ref)->left;
        else
        {
            pushq(&(*ptr_ref)->front, &(*ptr_ref)->rear, line);
            return tree;
        }
    }

    *ptr_ref = (NODE *)Malloc(sizeof(NODE));
    (*ptr_ref)->identifier = (char *)Malloc(strlen(identifier) + 1);

    strcpy((*ptr_ref)->identifier, identifier);
    (*ptr_ref)->left = (*ptr_ref)->right = NULL;
    (*ptr_ref)->front = (*ptr_ref)->rear = NULL;
    pushq(&(*ptr_ref)->front, &(*ptr_ref)->rear, line);
    return tree;
}

void printtree(NODE *tree, FILE *outfile)
{
    
    if(!tree) return;
    LINE *mover = tree->front;

    printtree(tree->left,outfile);
    printtree(tree->right, outfile);
    
    printf("%s: ", tree->identifier);
    while(mover)
    {
        printf("%ld", mover->linenum);
        if (mover->next)
            printf(", ");
        mover = mover->next;
    }
    printf("\n");
}

