#include <stdio.h>

void    find_captures(int grid[9], int pos, int *capture_masks, int *capture_count)
{
    int neighbors[4] = {-3, +3, -1, +1};
    int values[4], positions[4], num_neighbors = 0;
    for(int i = 0; i < 4; i++)
    {
        int neighbor = pos + neighbors[i];
        if (neighbors >= 0 && neighbor < 9 && grid[neighbor] > 0)
        {
            values[num_neighbors] = grid[neighbor];
            positions[num_neighbors] = neighbor;
            num_neighbors++;
        }
    }
    *capture_count = 0;
    for(int mask = 1; mask < (1 << num_neighbors); mask++)
    {
        int sum = 0, capture_mask = 0;
    }
}

void    f(int grid[9], int depth, int max_depth, unsigned int *sum)
{
    if (depth == max_depth)
    {
        return ;
    }
    int move_possible = 0;
    for(int i = 0; i < 9; i++)
    {
        int capture_mask[16], capture_count = 0;
        find_captures(grid, i, capture_mask, &capture_count);
    }
}

int main()
{
    int depth, grid[9];

    scanf("%d", &depth);
    for (int i = 0; i < 9; i++)
        scanf("%d", &grid[i]);
    
    unsigned int sum = 0;
    f(grid, 0, depth, &sum);
    printf("%u\n", sum);
}