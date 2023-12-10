#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAZESIZE 150

enum direction { UP, DOWN, LEFT, RIGHT };
char mazechar(char (*maze)[MAZESIZE], int row, int col);
void setfloodchar(char (*flood)[MAZESIZE], int row, int col, char c);
void floodfill(char (*flood)[MAZESIZE], int row, int col, char c);
void* parseinput(char* filename);
void findstart(char (*maze)[MAZESIZE], int* startrow, int* startcol);

// for part 1, just find the starting location and walk
// along the pipes until returning to the starting point.
// Half the final distance will be the furthest point.

// When walking along the pipe, also keep marking the pipe
// locations as well as left and right sides of the path.
// This will help in part 2.
int main()
{
    char (*maze)[MAZESIZE] = parseinput("input");

    // Make a blank copy of the maze for part 2.
    char (*flood)[MAZESIZE] = malloc(MAZESIZE* (MAZESIZE+1) * sizeof (char));
    for (int i=0; i<MAZESIZE; ++i) {
        memset(flood[i], ' ', MAZESIZE);
        flood[i][MAZESIZE-1] = '\0';
    }
    int row, col, distance = 0;
    enum direction dir;
    
    findstart(maze, &row, &col);

    setfloodchar(flood, row, col, 'P'); // Mark as pipe in flood map.
    char at = mazechar(maze, row, col);
    char above = mazechar(maze, row-1, col);
    char below = mazechar(maze, row+1, col);
    char left = mazechar(maze, row, col-1);
    char right = mazechar(maze, row, col+1);
    if (above == '|' || above == 'F' || above == '7') {
        dir = UP;
        --row;
        setfloodchar(flood, row, col, 'P');
    }
    else if (below == '|' || below == 'J' || below == 'L') {
        dir = DOWN;
        ++row;
        setfloodchar(flood, row, col, 'P');
    }
    else if (left == '-' || left == 'F' || left == 'L') {
        dir = LEFT;
        --col;
        setfloodchar(flood, row, col, 'P');
    }
    else if (right == '-' || right == 'J' || right == '7') {
        dir = RIGHT;
        ++col;
        setfloodchar(flood, row, col, 'P');
    }
    do {
        at = mazechar(maze, row, col);
        above = mazechar(maze, row-1, col);
        below = mazechar(maze, row+1, col);
        left = mazechar(maze, row, col-1);
        right = mazechar(maze, row, col+1);
        if (dir==UP && at=='|') {
            setfloodchar(flood, row, col, 'P');
            setfloodchar(flood, row, col-1, 'L');
            setfloodchar(flood, row, col+1, 'R');
            dir = UP;
            --row;
        }
        else if (dir==LEFT && at=='L') {
            setfloodchar(flood, row, col, 'P');
            setfloodchar(flood, row+1, col, 'L');
            setfloodchar(flood, row, col-1, 'L');
            dir = UP;
            --row;
        }
        else if (dir==RIGHT && at=='J') {
            setfloodchar(flood, row, col, 'P');
            setfloodchar(flood, row+1, col, 'R');
            setfloodchar(flood, row, col+1, 'R');
            dir = UP;
            --row;
        }
        else if (dir==DOWN && at=='|') {
            setfloodchar(flood, row, col, 'P');
            setfloodchar(flood, row, col+1, 'L');
            setfloodchar(flood, row, col-1, 'R');
            dir = DOWN;
            ++row;
        }
        else if (dir==LEFT && at=='F') {
            setfloodchar(flood, row, col, 'P');
            setfloodchar(flood, row-1, col, 'R');
            setfloodchar(flood, row, col-1, 'R');
            dir = DOWN;
            ++row;
        }
        else if (dir==RIGHT && at=='7') {
            setfloodchar(flood, row, col, 'P');
            setfloodchar(flood, row-1, col, 'L');
            setfloodchar(flood, row, col+1, 'L');
            dir = DOWN;
            ++row;
        }
        else if (dir==UP && at=='7') {
            setfloodchar(flood, row, col, 'P');
            setfloodchar(flood, row-1, col, 'R');
            setfloodchar(flood, row, col+1, 'R');
            dir = LEFT;
            --col;
        }
        else if (dir==DOWN && at=='J') {
            setfloodchar(flood, row, col, 'P');
            setfloodchar(flood, row+1, col, 'L');
            setfloodchar(flood, row, col+1, 'L');
            dir = LEFT;
            --col;
        }
        else if (dir==LEFT && at=='-') {
            setfloodchar(flood, row, col, 'P');
            setfloodchar(flood, row+1, col, 'L');
            setfloodchar(flood, row-1, col, 'R');
            dir = LEFT;
            --col;
        }
        else if (dir==UP && at=='F') {
            setfloodchar(flood, row, col, 'P');
            setfloodchar(flood, row-1, col, 'L');
            setfloodchar(flood, row, col-1, 'L');
            dir = RIGHT;
            ++col;
        }
        else if (dir==DOWN && at=='L') {
            setfloodchar(flood, row, col, 'P');
            setfloodchar(flood, row+1, col, 'R');
            setfloodchar(flood, row, col-1, 'R');
            dir = RIGHT;
            ++col;
        }
        else if (dir==RIGHT && at=='-') {
            setfloodchar(flood, row, col, 'P');
            setfloodchar(flood, row-1, col, 'L');
            setfloodchar(flood, row+1, col, 'R');
            dir = RIGHT;
            ++col;
        }
        ++distance;
    } while (at != 'S');

    // Part 2. For each character that has been marked as R,
    // flood fill around it to cover then entire area between
    // the pipes.
    int p2 = 0;
    for (int i=0; i<MAZESIZE; ++i)
        for (int j=0; j<MAZESIZE-1; ++j)
            if (mazechar(flood, i, j) == 'R') {
                floodfill(flood, i-1, j, 'R');
                floodfill(flood, i+1, j, 'R');
                floodfill(flood, i, j-1, 'R');
                floodfill(flood, i, j+1, 'R');
            }
    // Then count all the Rs.
    for (int i=0; i<MAZESIZE; ++i)
        for (int j=0; j<MAZESIZE-1; ++j)
            if (mazechar(flood, i, j) == 'R')
                ++p2;

    printf("Part 1: %i\nPart 2: %i\n", distance/2, p2);
    assert(distance/2 == 6768);
    assert(p2 == 351);
}

char mazechar(char (*maze)[MAZESIZE], int row, int col)
{
    if (row < 0 || row >= strlen(maze[0]) || col < 0 || col > strlen(maze[0]))
        return '\0';
    return maze[row][col];
}

void setfloodchar(char (*flood)[MAZESIZE], int row, int col, char c)
{
    if (row < 0 || row >= strlen(flood[0]) || col < 0 || col > strlen(flood[0]))
        return;
    if (flood[row][col] != 'P' && flood[row][col] != '\0') // Don't overwrite pipe.
        flood[row][col] = c;
}

void floodfill(char (*flood)[MAZESIZE], int row, int col, char c)
{
    if (row < 0 || row >= strlen(flood[0]) || col < 0 || col > strlen(flood[0]))
        return;
    if (flood[row][col] != ' ')
        return;
    flood[row][col] = c;
    floodfill(flood, row-1, col, c);
    floodfill(flood, row+1, col, c);
    floodfill(flood, row, col-1, c);
    floodfill(flood, row, col+1, c);
}

void* parseinput(char* filename)
{
    char (*maze)[MAZESIZE] = calloc(MAZESIZE*(MAZESIZE+1), sizeof (char));
    FILE* fptr = fopen(filename, "r");
    int i = 0;
    while (fgets(maze[i++], MAZESIZE+1, fptr))
        ;
    return maze;
}

// Find starting point.
void findstart(char (*maze)[MAZESIZE], int* startrow, int* startcol)
{
    char* cpt;
    for (int l=0; l<MAZESIZE; ++l) {
        if (cpt = strchr(maze[l], 'S')) {
            *startrow = l;
            *startcol = cpt - maze[l];
            return;
        }
    }
}
