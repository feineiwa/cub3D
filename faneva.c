#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

void find_adjacent(int tab[3][3], int i, int j)
{
    if (i == 0 && j == 0)
    {
        if (tab[j][i + 1] != 0 && tab[j + 1][i] != 0 && tab[j][i + 1] + tab[j + 1][i] <= 6)
            printf("%d", tab[j][i + 1] + tab[j + 1][i]);
    };
}


int main()
{
    int depth;
    int tab[3][3];
    scanf("%d", &depth);
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 3; i++) {
            scanf("%d", &tab[j][i]);
        }
    }
    while (depth != 0)
    {
        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < 3; i++) {
                find_adjacent(tab, i, j);
            }
        }
        depth--;
    }
    return 0;
}