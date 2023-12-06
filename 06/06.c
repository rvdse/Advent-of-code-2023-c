#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 128
#define MAXRACES 4

// Data structure to hold problem details.
typedef struct Race {
    long int time;
    long int distance;
} Race;

// Function declarations.
int part1(Race*);
int part2(Race*);
int ways_to_win(Race r);
Race* parseInput();
void readlist(char* str, int* list);
long int concat(long int a, long int b);

int main()
{
    Race* races = parseInput();
    printf("Part 1: %i\nPart 2: %i\n", part1(races), part2(races));
    free(races);
}

int part1(Race* races)
{
    int result = 1;
    for (int i = 0; i < MAXRACES && races[i].time > 0; ++i)
        result *= ways_to_win(races[i]);
    return result;
}

int part2(Race* races)
{
    Race r = { .time = 0, .distance = 0 };
    for (int i = 0; i < MAXRACES && races[i].time > 0; ++i) {
        r.time = concat(r.time, races[i].time);
        r.distance = concat(r.distance, races[i].distance);
    }
    return ways_to_win(r);
}

// Return the number of ways to win a race.
// Solved as a quadratic equation.
int ways_to_win(Race r)
{
    double t = r.time;
    double d = r.distance;
    double tmp = sqrt(t*t/4 - d);
    return ceil(t/2+tmp) - floor(t/2-tmp) - 1;
}

// Read problem from input file.
Race* parseInput()
{
    Race* races = calloc(MAXRACES, sizeof (Race));
    char line[MAXLINE];
    FILE* fptr = fopen("input", "r");
    if (fgets(line, sizeof line, fptr) != NULL) {
        int* tmp = calloc(MAXRACES, sizeof (int));
        readlist(line + 10, tmp);
        for (int i = 0; i < MAXRACES; ++i)
            races[i].time = tmp[i];
    }
    if (fgets(line, sizeof line, fptr) != NULL) {
        int* tmp = calloc(MAXRACES, sizeof (int));
        readlist(line + 10, tmp);
        for (int i = 0; i < MAXRACES; ++i)
            races[i].distance = tmp[i];
    }
    return races;
}

// Read list of numbers from string: "1 2 3" -> {1, 2, 3}
void readlist(char* str, int* list)
{
    int i = 0, num_read;
    while (1) {
        if (sscanf(str, "%i %n", &list[i++], &num_read) != 1)
            break;
        str += num_read;
    }
}

// Concatenate two numbers, so that concat(12, 34) == 1234.
long int concat(long int a, long int b)
{
    return a * pow(10, ceil( log10( b ))) + b;
}
