#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define HISTORYLEN 21
#define NUMLINES 200

int predict(int l[], int length);
int predict_backwards(int l[], int length);
int zeroes(int list[], int length);
void* parseinput(char* filename);
void readlist(char* str, int* list);

int main()
{
    int (*history)[HISTORYLEN] = parseinput("input");
    int part1 = 0, part2 = 0;
    for (int h=0; h<NUMLINES; ++h) {
        part1 += predict(history[h], HISTORYLEN);
        part2 += predict_backwards(history[h], HISTORYLEN);
    }
    printf("Part 1: %i\nPart 2: %i\n", part1, part2);
    assert(part1 == 1772145754);
    assert(part2 == 867);
}

int predict(int l[], int length)
{
    if (zeroes(l, length))
        return 0;
    int tmp[HISTORYLEN];
    for (int i=0; i<length-1; ++i)
        tmp[i] = l[i+1] - l[i];
    return l[length-1] + predict(tmp, length-1);
}

int predict_backwards(int l[], int length)
{
    if (zeroes(l, length))
        return 0;
    int tmp[HISTORYLEN];
    for (int i=0; i<length-1; ++i)
        tmp[i] = l[i+1] - l[i];
    return l[0] - predict_backwards(tmp, length-1);
}

int zeroes(int list[], int length)
{
    for (int i=0; i<length; ++i)
        if (list[i] != 0)
            return 0;
    return 1;
}

void* parseinput(char* filename)
{
    int (*h)[HISTORYLEN] = calloc(NUMLINES*HISTORYLEN, sizeof (int));
    FILE* fptr = fopen(filename, "r");
    int i = 0;
    char line[220];
    while (fgets(line, sizeof line, fptr)) {
        readlist(line, h[i++]);
    }
    return h;
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
