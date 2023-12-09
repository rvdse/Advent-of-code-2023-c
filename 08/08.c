#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 300
#define MAXLINES 1000
#define NODENAMELEN 4
#define MAXLOOPS 10

typedef struct Node {
    char name[NODENAMELEN];
    struct Node* left;
    struct Node* right;
} Node;

typedef struct Problem {
    char path[MAXLINE];
    int pathlen;
    Node nodes[MAXLINES];
    int numnodes;
} Problem;

int part1(Problem* p);
long int part2(Problem* p);
Problem* parseInput(char* filename);
void findloop(Problem* p, int n, long int* loopstart, int* looplen);
Node* setlocation(Problem* p, char* dst);

int main()
{
    Problem* prob = parseInput("input");
    int p1 = part1(prob);
    long int p2 = part2(prob);
    printf("Part 1: %i\nPart 2: %li\n", p1, p2);
    free(prob);
}

int part1(Problem* p)
{
    Node* loc = setlocation(p, "AAA");
    Node* dest = setlocation(p, "ZZZ");
    int count = 0;
    while (loc != dest) {
        loc = p->path[count%p->pathlen] == 'L' ? loc->left : loc->right;
        ++count;
    }
    return count;
}

/* For part 2, walk from each starting node until we can
 * identify a loop. For each loop, save its start point and
 * its length.
 */
long int part2(Problem* p)
{
    long int* loop = calloc(MAXLOOPS, sizeof (long int));
    int* step = calloc(MAXLOOPS, sizeof (int));
    // Find nodes ending with 'A' and loop.
    int numloops = 0;
    for (int n=0; n<p->numnodes; ++n)
        if (p->nodes[n].name[2] == 'A') {
            findloop(p, n, &(loop[numloops]), &(step[numloops]));
            ++numloops;
        }
    while (1) {
        // Check if we are done.
        int done = 1;
        for (int i = 1; i < numloops; ++i)
            if (loop[i] != loop[0])
                done = 0;
        if (done)
            break;
        // Step the loop least farthest along.
        int smallest = 0;
        for (int i = 1; i < numloops; ++i)
            if (loop[i] < loop[smallest])
                smallest = i;
        loop[smallest] += step[smallest];
    }
    return loop[0];
}

void findloop(Problem* p, int n, long int* loopstart, int* looplen)
{
    int i = 0;
    Node* loc = &(p->nodes[n]);
    while (1) {
        loc = p->path[i%p->pathlen] == 'L' ? loc->left : loc->right;
        ++i;
        if (loc->name[2] == 'Z') {
            if (*loopstart == 0)
                *loopstart = i;
            else if (*looplen == 0) {
                *looplen = i - *loopstart;
                return;
            }
        }
    }
}

// Set location to a name, rather than pointer.
Node* setlocation(Problem* p, char* dst)
{
    for (int n=0; n<p->numnodes; ++n)
        if (strncmp(p->nodes[n].name, dst, NODENAMELEN) == 0)
            return &(p->nodes[n]);
    return NULL;
}

Problem* parseInput(char* filename)
{
    Problem* p = calloc(1, sizeof (Problem));
    // Temporary storage for left/right names, do be
    // replaced with pointers when all names are read.
    char leftnames[MAXLINES][NODENAMELEN];
    char rightnames[MAXLINES][NODENAMELEN];
    memset(leftnames, 0, MAXLINES*NODENAMELEN);
    memset(rightnames, 0, MAXLINES*NODENAMELEN);
    FILE* fptr = fopen(filename, "r");
    if (fgets(p->path, MAXLINE, fptr) != NULL)
        p->path[strcspn(p->path, "\n")] = '\0';
    p->pathlen = strlen(p->path);
    fgetc(fptr); // Skip blank line.
    while (3 == fscanf(fptr, "%3s = (%3s, %3s)",
                p->nodes[p->numnodes].name,
                leftnames[p->numnodes],
                rightnames[p->numnodes]))
        ++p->numnodes;
    // When all node names are read, go back and update
    // left/right pointers.
    for (int i=0; i<p->numnodes; ++i) {
        for (int j=0; j<p->numnodes; ++j) {
            if (strncmp(leftnames[i], p->nodes[j].name, NODENAMELEN) == 0)
                p->nodes[i].left = &(p->nodes[j]);
            if (strncmp(rightnames[i], p->nodes[j].name, NODENAMELEN) == 0)
                p->nodes[i].right = &(p->nodes[j]);
        }
    }
    fclose(fptr);
    return p;
}
