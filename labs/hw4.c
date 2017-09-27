/*
 * This program uses main to show that the functions work. toggleBits toggles
 * all bits except bits in the denoted range. turnOnBits turns on all bits
 * except bits in the denoted range. leftrot rotates left and encryptFile
 * encrypts and decrypts a file.
 *
 * Mina Yi
 * Homework 4
 * May 31, 2011
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 512
#define TOGGLE_RANGE ^= ~(~((unsigned short) ~0 << numbits) << (startbit - 1))
#define TURN_BITS |= ~(~((unsigned short) ~0 << numbits) << (startbit - 1))

int testbit(unsigned short word, int bit_to_test);
void printbits(unsigned short word);
unsigned short toggleBits(unsigned short word, int numbits, int startbit);
int encryptFile(char *input, char *output, char *word);
unsigned short leftrot(unsigned short word, int numbits);
unsigned short turnOnBits(unsigned short word, int numbits, int startbit);

int main()
{
    unsigned short a = 4, b = 12, c;
    int numbits = 4, startbit = 4;
    char *word = "word"; 
    char *input = "hw3in.txt", *output = "hw4out.txt", *output2 = "hw4out2.txt";

    c = toggleBits(a, numbits, startbit);
    printbits(c);
    putchar('\n');

    encryptFile(input, output, word);
    encryptFile(output, output2, word);

    c = turnOnBits(a, numbits, startbit);
    printbits(c);
    putchar('\n');

    c = leftrot(a, numbits);
    printbits(c);
    putchar('\n');

    a TOGGLE_RANGE;
    b TOGGLE_RANGE;
    printbits(a);
    putchar('\n');
    printbits(b);
    putchar('\n');

    a TURN_BITS;
    b TURN_BITS;
    printbits(a);
    putchar('\n');
    printbits(c);
    putchar('\n');
   
    return 0;
}

/*
 * testbit is passed an unsigned short word and int bit_to_test. It returns 1
 * or 0 and is called by printbits in order to print the number in binary.
 */
int testbit(unsigned short word, int bit_to_test)
{   
    word>>= bit_to_test;
    word &= 1;
    return word;
}

/*
 * printbits takes in an unsigned short word. It calls testbit in order to
 * print the binary number.
 */
void printbits(unsigned short word)
{
    int i;

    for(i = 15; i >= 0; i--)
        printf("%ld", testbit(word, i));
}

/*
 * toggleBits takes in an unsigned short word, the number of bits and the
 * startbit position. It returns the bits toggled that are not in the denoted
 * range which is specified by numbits and startbit.
 */
unsigned short toggleBits(unsigned short word, int numbits, int startbit)
{
    return word ^ ~(~((unsigned short) ~0 << numbits) << (startbit - 1));
}

/*
 * encryptFile takes in the name of an input file, name of an output file, and
 * a word that is used as the encryption/decryption key. It returns 0 if the
 * file can not be opened else it returns 1 on completion of
 * encryption/decryption.
 *
 */
int encryptFile(char *input, char *output, char *word)
{
    unsigned int key;
    char *mover, buffer[MAX];
    FILE *fpin, *fpout;

    if(!(fpin = fopen(input, "r")))
        return 0;

    if(!(fpout = fopen(output, "w")))
        return 0;

    for(key = 0; *word; word++)
        key += *word;

    while(fgets(buffer, MAX, fpin))
    {
        for(mover = buffer; *mover && *mover != '\n'; mover++)
            *mover ^= key;

        fputs(buffer, fpout);
    }

    fclose(fpin);
    fclose(fpout);
    return 1;
}

/*
 * leftrot takes in an unsigned short called word and the number of bits. it
 * returns word after the bits have been rotated left.
 */
unsigned short leftrot(unsigned short word, int numbits)
{
    unsigned short nleft;

    nleft = word >> (8 * sizeof(ushort) - numbits);
    word <<= numbits;
    word |= nleft;
    return word;
}

/*
 * turnOnBits takes in an unsigned short word, the number of bits and the
 * startbit. It turns on all bits except bits denoted in the range which is
 * specified by startbit and numbits. It returns word.
 */
unsigned short turnOnBits(unsigned short word, int numbits, int startbit)
{
    word |= ~(~((unsigned short) ~0 << numbits) << (startbit - 1));
    return word;
}

