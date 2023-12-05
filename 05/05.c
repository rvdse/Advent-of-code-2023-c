#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 256
#define MAXSEEDS 20
#define NMAPS 7
#define NFIELDS 3
#define NRULES 64
#define DEST 0
#define SRC 1
#define RANGE 2
#define EMPTY -1
#define FORWARD 0
#define BACKWARD 1
#define MIN(a,b) ((a) < (b) ? (a) : (b))

void readproblemfile(long int seeds[MAXSEEDS], long int maps[NMAPS][NRULES][NFIELDS]);
void readlist(char* str, long int* list);
long int lookup(long int val, long int map[NRULES][NFIELDS], int dir);
int isseed(long int s, long int seeds[MAXSEEDS]);
long int part1(long int seeds[MAXSEEDS], long int maps[NMAPS][NRULES][NFIELDS]);
long int part2(long int seeds[MAXSEEDS], long int maps[NMAPS][NRULES][NFIELDS]);

int main()
{
    long int seeds[MAXSEEDS];
    memset(seeds, 0xFF, sizeof seeds); // Set all to EMPTY.
    long int maps[NMAPS][NRULES][NFIELDS];
    memset(maps, 0xFF, sizeof maps); // Set all to EMPTY.
    readproblemfile(seeds, maps);
    printf("Part 1: %li\nPart 2: %li\n", part1(seeds, maps), part2(seeds, maps));
}

// For part 1, just check the location of each seed.
long int part1(long int seeds[MAXSEEDS], long int maps[NMAPS][NRULES][NFIELDS])
{
    long int result = LONG_MAX;
    for (int i=0; i<MAXSEEDS && seeds[i] != EMPTY; ++i) {
        long int loc = seeds[i];
        for (int m = 0; m < NMAPS; ++m)
            loc = lookup(loc, maps[m], FORWARD);
        result = MIN(loc, result);
    }
    return result;
}

// For part 2, it is faster to search backwards (location to
// seed) instead of forwards (seed to location).
// Also, start with a step size about the size of the smallest
// seed range and use a logarithmic search pattern.
long int part2(long int seeds[MAXSEEDS], long int maps[NMAPS][NRULES][NFIELDS])
{
    long int seed, location = 0;
    int stepsign = 1, stepsize = INT_MAX;
    for (int i=1; i<MAXSEEDS && seeds[i] != EMPTY; i+=2)
        stepsize = MIN(stepsize, seeds[i]);
    while (1) {
        location += stepsign * stepsize;
        seed = location;
        for (int m = NMAPS-1; m >= 0; --m)
            seed = lookup(seed, maps[m], BACKWARD);
        if (stepsign != (isseed(seed, seeds) ? -1 : 1)) {
            if (stepsize == 1)
                return stepsign == 1 ? location : location + 1;
            stepsize /= 2;
            stepsign *= -1;
        }
    }
}

// Read the problem data from file.
void readproblemfile(long int seeds[MAXSEEDS], long int maps[NMAPS][NRULES][NFIELDS])
{
    char line[MAXLINE];
    FILE* fptr = fopen("input", "r");
    // First read list of seeds.
    if (fgets(line, sizeof line, fptr) != NULL)
        readlist(line + 7, seeds); // "+7" skips prefix "Seeds: ".
    // Then read all the various maps.
    int i_map = -1, i_rule = 0;
    while (fgets(line, sizeof line, fptr) != NULL) {
        long int dst, src, range;
        if (strstr(line, "map:") != NULL) {
            ++i_map;
            i_rule = 0;
        }
        else if (sscanf(line, "%li %li %li", &dst, &src, &range) == 3) {
            maps[i_map][i_rule][DEST] = dst;
            maps[i_map][i_rule][SRC] = src;
            maps[i_map][i_rule][RANGE] = range;
            ++i_rule;
        }
    }
}

// Convert string to list of numbers.
void readlist(char* str, long int* list)
{
    int i = 0, num_read;
    while (1) {
        if (sscanf(str, "%li %n", &list[i++], &num_read) != 1)
            break;
        str += num_read;
    }
}

// Look up a value in a map. direction dir can be 
// 0 or FORWARD: find destination from source.
// 1 or BACKWARD: find source from destination.
long int lookup(long int val, long int map[NRULES][NFIELDS], int dir)
{
    for (int i = 0; i < NRULES && map[i][0] != EMPTY; ++i) {
        if (val >= map[i][SRC-dir] && val < map[i][SRC-dir] + map[i][RANGE]) {
            val += map[i][dir-DEST] - map[i][SRC-dir];
            break;
        }
    }
    return val;
}

// Return 1 if s is a seed according to rules of part 2.
int isseed(long int s, long int seeds[MAXSEEDS])
{
    for (int i=0; i+1<MAXSEEDS; i+=2)
        if (seeds[i] != EMPTY && s >= seeds[i] && s < seeds[i] + seeds[i+1])
            return 1;
    return 0;
}
