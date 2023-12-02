#include <stdio.h>
#include <string.h>
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define LINELEN 100

void part1(char *line, int *i1, int *i2);
int part2(char *line, int i_fst, int i_snd);
char* words[9] = {"one","two","three","four","five","six","seven","eight","nine"};
char* words_reverse[9] = {"eno","owt","eerht","ruof","evif","xis","neves","thgie","enin"};

int main()
{
    int p1 = 0;
    int p2 = 0;
    char line[LINELEN];
    FILE* fptr = fopen("input", "r");
    while(fgets(line, sizeof line, fptr) != NULL) {
        int i_fst, i_snd;
        part1(line, &i_fst, &i_snd);
        p1 += 10 * (line[i_fst] - '0') + (line[i_snd] - '0');
        p2 += part2(line, i_fst, i_snd);
    }
    fclose(fptr);
    printf("Part 1: %i\nPart 2: %i\n", p1, p2);
}

void part1(char *line, int *i1, int *i2)
{
    *i1 = LINELEN;
    for (int i=0; i<LINELEN && line[i] != '\0'; ++i)
        if (line[i] >= '0' && line[i] <= '9') {
            *i1 = MIN(i, *i1);
            *i2 = i;
        }
}

int part2(char *line, int i_fst, int i_snd)
{
    int val1 = line[i_fst] - '0'; // Init to values from digits (part 1).
    int val2 = line[i_snd] - '0';
    const long int len = strlen(line);
    char line_reverse[LINELEN];
    for (int i=0; i<len; ++i)
        line_reverse[i] = line[len-i-1];
    line_reverse[len] = '\0';
    for (int i=0; i<9; ++i) {
        char *p = strstr(line, words[i]);
        if (p != NULL && (p-line) < i_fst) {
            i_fst = p-line;
            val1 = i + 1;
        }
        p = strstr(line_reverse, words_reverse[i]);
        if (p != NULL && len - (p-line_reverse) > i_snd) {
            i_snd = len - (p-line_reverse);
            val2 = i + 1;
        }
    }
    return 10 * val1 + val2;
}
