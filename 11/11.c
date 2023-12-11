#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Macros.
#define SIZE 140
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

// Data structures.
typedef struct Galaxy {
    int row;
    int col;
} Galaxy;
typedef struct Universe {
    Galaxy* galaxies;
    char* occupiedrows;
    char* occupiedcols;
} Universe;

// Function declarations.
long int distance(Universe* u, int g1, int g2, int expansionfactor);
Universe* parseinput(char* filename);

int main()
{
    Universe* u = parseinput("input");
    long int part1 = 0, part2 = 0;
    // Row and column coordinates start at (1, 1) in the upper
    // left corner. A row coordinate of 0 marks the end of the
    // initialized galaxies. Assert that there is at least one
    // uninitialized galaxy at the end of the list.
    assert(!u->galaxies[SIZE*SIZE-1].row);
    for (int i=0; u->galaxies[i].row; ++i)
        for (int j=i+1; u->galaxies[j].row; ++j) {
            part1 += distance(u, i, j, 2);
            part2 += distance(u, i, j, 1000000);
        }
    printf("Part 1: %li\nPart 2: %li\n", part1, part2);
    assert(part1 == 9918828);
    assert(part2 == 692506533832);
}

long int distance(Universe* u, int g1, int g2, int expansionfactor)
{
    int maxrow = MAX(u->galaxies[g1].row, u->galaxies[g2].row);
    int maxcol = MAX(u->galaxies[g1].col, u->galaxies[g2].col);
    int minrow = MIN(u->galaxies[g1].row, u->galaxies[g2].row);
    int mincol = MIN(u->galaxies[g1].col, u->galaxies[g2].col);
    long int distance = maxrow + maxcol - minrow - mincol;
    for (int i=minrow+1; i<maxrow; ++i)
        distance += u->occupiedrows[i] ? 0 : expansionfactor - 1;
    for (int i=mincol+1; i<maxcol; ++i)
        distance += u->occupiedcols[i] ? 0 : expansionfactor - 1;
    return distance;
}

Universe* parseinput(char* filename)
{
    Universe* u = malloc(sizeof (Universe));
    u->galaxies = calloc(SIZE*SIZE, sizeof (Galaxy));
    u->occupiedrows = calloc(SIZE+1, sizeof (char));
    u->occupiedcols = calloc(SIZE+1, sizeof (char));
    int c, row = 1, col = 1, gnum = 0;
    FILE* fptr = fopen(filename, "r");
    while ((c = fgetc(fptr)) != EOF) {
        if (c == '#') {
            u->occupiedrows[row] = 1;
            u->occupiedcols[col] = 1;
            u->galaxies[gnum].row = row;
            u->galaxies[gnum++].col = col;
        }
        else if (c == '\n') {
            ++row;
            col = 0; // Will be 1 at end of loop.
        }
        ++col;
    }
    return u;
}
