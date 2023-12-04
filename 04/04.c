#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINELEN 200
#define MAXCARDS 300
#define MAXNUMS 30

int readcard(char* line);
char* split(char* str, char* delimiters);
void readlist(char* str, int* list);
int numintersecting(int* a, int* b);

int main()
{
    int part1 = 0;
    int part2 = 0;
    int* numcards = calloc(MAXCARDS, sizeof(int));
    int card = 0;
    char line[LINELEN];
    FILE* fptr = fopen("input", "r");
    while (fgets(line, sizeof line, fptr) != NULL && ++card) {
        int numwinners = readcard(line);
        part1 += numwinners == 0 ? 0 : 1 << numwinners-1;
        // Add original card to count and update part2 total.
        part2 += ++numcards[card];
        // Give bonus cards based on score and number of cards.
        for (int i=1; i<=numwinners; ++i)
            numcards[card + i] += numcards[card];
    }
    printf("Part 1: %i\nPart 2: %i\n", part1, part2);
}

// Return the number of winning numbers on a card.
int readcard(char* line)
{
    int* win_nums = calloc(MAXNUMS, sizeof (int));
    int* card_nums = calloc(MAXNUMS, sizeof (int));
    char* win_nums_substr = split(line, ":");
    char* card_nums_substr = split(win_nums_substr, "|");
    readlist(win_nums_substr, win_nums);
    readlist(card_nums_substr, card_nums);
    return numintersecting(win_nums, card_nums);
}

// Split a string in two at the first occurrence of a character
// in delimiters.
// The original string will end at delimiter (non-inclusive).
// The returned value points to the start of the second half. 
char* split(char* str, char* delimiters)
{
    char* result = strpbrk(str, delimiters);
    *result = '\0'; // Terminate original string at delimiter.
    return ++result;// Return position after delimiter.
}

// Read a list of numbers from a string.
void readlist(char* str, int* list)
{
    int elem = 0;
    while (1) {
        int read; // Number of characters read.
        if (sscanf(str, "%i %n", &list[elem++], &read) != 1)
            break;
        str += read;
    }
}

// Given two lists, return the number of elements that appear
// in both lists BUT excluding 0 as an invalid element.
int numintersecting(int* a, int* b)
{
    int num = 0;
    for (int i = 0; i < MAXNUMS; ++i)
        for (int j = 0; j < MAXNUMS; ++j)
            if (a[i] != 0 && a[i] == b[j])
                ++num;
    return num;
}
