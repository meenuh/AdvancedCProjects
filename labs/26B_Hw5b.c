
/*
 * Mina Yi
 * Homework 5
 * June 14, 2011
 */
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


int main(int argc, char *argv[])
{
    void  check_program_args(int argc, char *argv[],FILE **infile,FILE **outfile);
    void  printtree(NODE *tree, FILE *outfile);
    void  printprog(FILE *infile, FILE *outfile);
    void  print_time_stamp(FILE *outfile);
    NODE *parse_file_into_ident_tree(FILE *infile);
    NODE *insertTree(char *identifier, long line, NODE *tree);
    void  pushq(LINE **q, LINE **rear, long line);
    void freemem(NODE *tree);

    FILE *infile, *outfile;
    NODE *tree = NULL;

    check_program_args(argc, argv, &infile, &outfile);
    tree = parse_file_into_ident_tree(infile);
    print_time_stamp(outfile); 
    printtree(tree, outfile);   /* Prints cross-reference table. */
    printprog(infile, outfile); /* Prints input program with line numbers in
                                 left column. */
    freemem(tree);
    Fclose(infile);
    Fclose(outfile);
    return 0;
}

/*                                                                        
*  This function checks to see that a filename argument was supplied by   
*  the user when invoking this program.  It also makes sure that the      
*  filename is that of a C source file (name ends with .c) and that the   
*  source file exists.                                                    
*/
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

/*
 * parse_file_into_ident_tree takes in a FILE *. It parses the c source code
 * and returns a NODE * after filling the nodes in the tree.
 */
NODE *parse_file_into_ident_tree(FILE *infile)
{
    NODE *insertTree(char *identifier, long line, NODE *tree);
    char buffer[MAXIDENT], temp[MAXLINE], *buffmover, *tempmover, ignore_until = '\0';
    int count = 0;
    NODE *tree = NULL;

    while(fgets(buffer, MAXIDENT, infile))
    {
        count++;
        for(buffmover = buffer, tempmover = temp; *buffmover; buffmover++)
        {
            if (ignore_until) 
            {
                if(ignore_until == '/' && *(buffmover - 1) != '*')
                    continue;
                while(*buffmover) 
                {
                    if (*buffmover == ignore_until) 
                    {
                        ignore_until = '\0';
                        break;
                    }
                    buffmover++;
                }
                continue;
            }

            if (*buffmover)
            {
                switch (*buffmover)
                {
                case '/':
                    if (*(buffmover + 1) == '/')
                    {
                        ignore_until = '\n';
                    }
                    else if (*(buffmover + 1) == '*')
                    {
                        ignore_until = '/';
                    }
                    buffmover++;
                    break;
                case '\'':
                    ignore_until = '\'';
                    break;
                case '\\':
                    buffmover++;
                    break;
                case '"':
                    ignore_until = '"';
                    break;
                case '<':
                    ignore_until = '>';
                    break;
                default:
                    if (isalpha(*buffmover) || *buffmover == '_')
                    {
                        memset(temp, 0, MAXLINE);
                        tempmover = temp;

                        while(*buffmover && (isalnum(*buffmover) || *buffmover == '_'))
                            *tempmover++ = *buffmover++;
                        tree = insertTree(temp, count, tree);
                    }
                }
            }
        }
    }
    return tree;
}

/*
 * pushq takes in LINE **q, **rear and a long line which is the line number. It
 * pushes new line numbers into the queue. It does not return anything.
 */
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

/*
 * insertTree takes in the identifier, the line number and the tree. It inserts
 * identifiers into the tree. It calls pushq to push line numbers into the
 * queue when the identifier is found. It returns the tree.
 */
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
            if((*ptr_ref)->rear->linenum != line)
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

/*
 * printtree takes in the tree and the output file pointer. It recursively
 * traverses the tree and prints the identifier along with the line numbers out
 * to file. It does not return anything.
 */
void printtree(NODE *tree, FILE *outfile)
{
    int count = 0;
    if(!tree) return;
    LINE *mover = tree->front;

    printtree(tree->left,outfile);
    fprintf(outfile, "%-20s  ", tree->identifier);
    while(mover)
    {
        count++;
        if(count > 12)
        {
            fprintf(outfile, "\n%-22s", "");
            count = 1;
        }
        fprintf(outfile, "%-5ld", mover->linenum);
        mover = mover->next;
    }
    fputc('\n', outfile);
    printtree(tree->right, outfile);
}

/*
 * print_time_stamp takes in the outfile. It prints the current time reference
 * to the file. It does not return anything.
 */
void print_time_stamp(FILE *outfile)
{
    time_t t;
    struct tm *tmp;
    char buff[MAXLINE];

    t = time(NULL);
    tmp = localtime(&t);

    strftime(buff, MAXLINE, "%a %b %d %T %Y", tmp);
    fprintf(outfile, "Cross reference listing made at: %s\n\n", buff);
}
    
/*
 * printprog takes in the FILE * of infile and outfile. It prints out the
 * original source code into the output file along with line numbers. It does
 * not return  anything.
 */
void printprog(FILE *infile, FILE *outfile)
{
    int count = 0;
    char buffer[MAXIDENT];

    rewind(infile);
    while(fgets(buffer, MAXIDENT, infile))
    {
        count++;
        fprintf(outfile, "%-5d: %s", count, buffer);
    }
}

/*
 * freemem takes in the tree. It recursively traverses the tree. It frees the
 * queue, the string for the identifier and the tree node itself. It does not
 * return anything.
 */
void freemem(NODE *tree)
{
    if(!tree) return;
    LINE *current = tree->front, *mover = current;

    freemem(tree->left);
    freemem(tree->right);
    free(tree->identifier);

    while(current)
    {
        mover = mover->next;
        free(current);
        current = mover;
    }
    free(tree);

