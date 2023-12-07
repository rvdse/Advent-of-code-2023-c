#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 64
#define MAXHANDS 1000
#define NUMCARDS 13
#define HANDSIZE 5

// Datatype for problem description.
typedef struct Hand {
    char cards[HANDSIZE+1];
    int bid;
    int strength;
} Hand;

// Global variables for game rules.
char CARDS[NUMCARDS+1] = "23456789TJQKA";
char* JOKERCARDS = NULL;

// Function declarations.
int play(Hand* hands);
Hand* parseInput();
void score(Hand* h);
int typestrength(Hand* h);
int secondarystrength(Hand* h);
int countcards(Hand* h, char card);
int cmp(const void* a, const void* b);

int main()
{
    Hand* hands = parseInput();
    int part1 = play(hands);
    JOKERCARDS = "J23456789TQKA"; // Activates joker rules.
    int part2 = play(hands);
    free(hands);
    printf("Part 1: %i\nPart 2: %i\n", part1, part2);
}

int play(Hand* hands)
{
    int result = 0, numhands = 0;
    // Update score for each hand, also count number of hands.
    while (numhands<MAXHANDS && hands[numhands].cards[0] != '\0')
        score(&hands[numhands++]);
    // Sort based on scores, and sum the winnings.
    qsort(hands, numhands, sizeof (Hand), cmp);
    for (int i=0; i<MAXHANDS && hands[i].cards[0] != '\0'; ++i)
        result += (i+1)*hands[i].bid;
    return result;
}

Hand* parseInput()
{
    Hand* hands = calloc(MAXHANDS, sizeof (Hand));
    int i = 0;
    char line[MAXLINE];
    FILE* fptr = fopen("input", "r");
    while (fgets(line, sizeof line, fptr) != NULL) {
        strncpy(hands[i].cards, line, HANDSIZE);
        hands[i].cards[HANDSIZE] = '\0';
        hands[i++].bid = atoi(line + 6);
    }
    fclose(fptr);
    return hands;
}

// Calculate and store the relative strength of a hand.
void score(Hand* h)
{
    h->strength = typestrength(h) + secondarystrength(h);
}

// The primary hand strength, based on hand type.
int typestrength(Hand* h)
{
    int first = 0; // Number of cards in the first group.
    int snd = 0; // Number of cards in 2nd first group.
    int jokers = JOKERCARDS ? countcards(h, 'J') : 0;
    for (int i=0; i<NUMCARDS; ++i) {
        if (JOKERCARDS && CARDS[i] == 'J')
            continue;
        int num = countcards(h, CARDS[i]);
        if (num >= first) {
            snd = first;
            first = num;
        }
        else if (num > snd)
            snd = num;
    }
    if (first + jokers >= 5) return 6e6; // Five of a kind.
    if (first + jokers >= 4) return 5e6; // Four of a kind.
    if (first + snd + jokers >= 5) return 4e6; // Full house.
    if (first + jokers >= 3) return 3e6; // Three of a kind.
    if (first + snd + jokers >= 4) return 2e6; // Two pair.
    if (first + jokers >= 2) return 1e6; // One pair.
    return 0;
}

// In a given hand h, return the number of cards equal to c.
int countcards(Hand* h, char c)
{
    int count = 0;
    for (int i=0; i<HANDSIZE; ++i)
        count += h->cards[i] == c;
    return count;
}

// The strength based on card values, used to break ties.
int secondarystrength(Hand* h)
{
    int str = 0;
    char* card_order = JOKERCARDS ? JOKERCARDS : CARDS;
    for (int i=0; i<HANDSIZE; ++i)
        str = NUMCARDS*str + strchr(card_order, h->cards[i]) - card_order;
    return str;
}

// Help function to qsort, compares hand strengths.
int cmp(const void* a, const void* b)
{
    return ((const Hand*)a)->strength - ((const Hand*)b)->strength;
}
