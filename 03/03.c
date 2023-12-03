#include <stdio.h>
#include <stdlib.h>

#define BUFFERSIZE 160
#define MAXGEARS 512

// Global variable to keep track of gears.
struct gear { unsigned long id; int num1; int num2; } gears[MAXGEARS];

// Function declarations.
int sumparts(char* prev, char* cur, char* next);
int symbol(char* line, int num, int pos);
void foundgear(char* line, int num, int pos);
int countgears();
unsigned long hash(unsigned char *str);

int main()
{
    int part1 = 0;
    for (int i=0; i<MAXGEARS; ++i) {
        gears[i].id = 0;
        gears[i].num1 = 0;
        gears[i].num2 = 0;
    }
    char* prevline = calloc(BUFFERSIZE, sizeof(char));
    char* curline = calloc(BUFFERSIZE, sizeof(char));
    char* nextline = calloc(BUFFERSIZE, sizeof(char));
    FILE* fptr = fopen("input", "r");
    fgets(curline, BUFFERSIZE, fptr);
    while (fgets(nextline, BUFFERSIZE, fptr) != NULL) {
        part1 += sumparts(prevline, curline, nextline);
        // Rotate line buffers.
        char* tmp = prevline;
        prevline = curline;
        curline = nextline;
        nextline = tmp;
    }
    fclose(fptr);
    // After all lines are read, the last line has only
    // been tested as "next", not as "current". Set
    // nextline to empty and run one last time.
    for (int i=0; i<BUFFERSIZE; ++i)
        nextline[i] = '\0';
    part1 += sumparts(prevline, curline, nextline);
    printf("Part 1: %i\nPart 2: %i\n", part1, countgears());
}

// Takes three lines. Looks for numbers on the middle line (cur)
// and then looks for adjacent symbols using the other lines.
// Returns sum of numbers with adjacent symbols.
// Sneakily, the register of gears will be built in the calls
// to symbol().
int sumparts(char* prev, char* cur, char* next)
{
    int result = 0;
    for (int i=0; cur[i] != '\0'; ++i) {
        if (cur[i] >= '0' && cur[i] <= '9') {
            // Read the entire number.
            int start = i;
            int num = cur[i] - '0';
            while (cur[i+1] >= '0' && cur[i+1] <= '9') {
                ++i;
                num = 10 * num + cur[i] - '0';
            }
            // Look for adjacent symbols.
            int adjacent = symbol(cur, num, start-1) + symbol(cur, num, i+1);
            for (int j=start-1; j<=i+1; ++j) {
                adjacent += symbol(prev, num, j);
                adjacent += symbol(next, num, j);
            }
            if (adjacent)
                result += num;
            ++i;
        }
    }
    return result;
}

// Return 1 if the character on the specified position in the line
// counts as a symbol. Additionally, if the symbol is a gear,
// register that gear along with the number it is adjacent to.
int symbol(char* line, int num, int pos)
{
    switch (line[pos]) {
        case '.': case '\n': case '\0':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return 0;
        case '*':
            foundgear(line, num, pos);
        default:
            return 1;
    }
}

// When a gear is found next to a number, register that gear and number.
// When the entire file has been read, gear ratios can be calculated
// from this register.
// A gear is identified by a hash created by the line where it was
// found, but altered to mark the gear position as well.
void foundgear(char* line, int num, int pos)
{
    line[pos] = 'G';
    unsigned long h = hash(line);
    line[pos] = '*';
    int i;
    for (i=0; gears[i].id != 0 && gears[i].id != h && i < MAXGEARS; ++i)
        ;
    gears[i].id = h;
    if (gears[i].num1 == 0)
        gears[i].num1 = num;
    else
        gears[i].num2 = num;
}

int countgears()
{
    int result = 0;
    for (int i=0; i<MAXGEARS && gears[i].id != 0; ++i)
        if (gears[i].num1 && gears[i].num2)
            result += gears[i].num1 * gears[i].num2;
    return result;
}

// Hash function djb2 by Dan Bernstein,
// from http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}
