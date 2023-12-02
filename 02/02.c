#include <stdio.h>
#include <string.h>

#define LINELEN 256
#define SUBSTRLEN 6
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MAXRED 12
#define MAXGREEN 13
#define MAXBLUE 14

int maxcubes(char* line, char* color);

int main()
{
    int part1 = 0;
    int part2 = 0;
    char line[LINELEN];
    FILE* fptr = fopen("input", "r");
    while (fgets(line, sizeof line, fptr) != NULL) {
        int game_id, red, green, blue;
        sscanf(line, "Game %i:", &game_id);
        red = maxcubes(line, "red");
        green = maxcubes(line, "green");
        blue = maxcubes(line, "blue");
        if (red <= MAXRED && green <= MAXGREEN && blue <= MAXBLUE)
            part1 += game_id;
        part2 += red * green * blue;
    }
    fclose(fptr);
    printf("Part 1: %i\nPart 2: %i\n", part1, part2);
}

// Count the maximum number of cubes seen on a line, of a specific color.
int maxcubes(char* line, char* color)
{
    int most_seen = 0;
    char* sub = line;
    while (1) {
        sub = strstr(sub, color); // Search for 1st occurrence.
        if (sub == NULL)
            return most_seen;
        int seen = 0;
        if (sscanf(sub-SUBSTRLEN, "%*s %i", &seen) == 1)
            most_seen = MAX(most_seen, seen);
        ++sub; // Increment so that next strstr will find new match.
    }
}
