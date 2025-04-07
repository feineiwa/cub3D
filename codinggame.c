#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MODULO 0x3FFFFFFF
int save[3996000000];

unsigned int	simulate(int grid[9], int depth, int max_depth);

unsigned int calculate_hash(int grid[9])
{
	return (grid[0] * 100000000 + grid[1] * 10000000 + grid[2] * 1000000 + grid[3] * 100000 + grid[4] * 10000 + grid[5] * 1000 + grid[6] * 100 + grid[7] * 10 + grid[8]);
}

// int calculate_hash(int grid[9])
// {
// 	int hash = 0;

// 	for (int i = 0; i < 9; i++)
// 		hash = (hash * 10) + grid[i];
// 	return (hash);
	
// }

int	in_bounds(int pos)
{
	if (pos >= 0 && pos < 9)
		return (1);
	return (0);
}

void	copy_grid(int new_grid[9], int grid[9])
{
	new_grid[0] = grid[0];
	new_grid[1] = grid[1];
	new_grid[2] = grid[2];
	new_grid[3] = grid[3];
	new_grid[4] = grid[4];
	new_grid[5] = grid[5];
	new_grid[6] = grid[6];
	new_grid[7] = grid[7];
	new_grid[8] = grid[8];
}


// void	check_neighbors(int *values, int *positions, int *num_neighbors, int grid[9], int pos, int i, int neighbors[4])
// {
// 	int neighbor = 0;
// 	if ((pos == 2 || pos == 5) && i == 3)
// 		neighbor = -1;
// 	else if ((pos == 3 || pos == 6) && i == 2)
// 		neighbor = -1;
// 	else
// 		neighbor = pos + neighbors[i];
// 	if (in_bounds(neighbor) && (grid[neighbor] >= 1 && grid[neighbor] < 6))
// 	{
// 		positions[*num_neighbors] = neighbor;
// 		values[*num_neighbors] = grid[neighbor];
// 		(*num_neighbors)++;
// 	}
// }

unsigned int	find_capture(int *grid, int pos, int depth, int max_depth)
{
	unsigned int neighbors[4] = {-3, 3, -1, 1}; //up down left right

	unsigned int values[4];
	unsigned int positions[4];
	unsigned short	num_neighbors = 0;
	unsigned int res = 0;
	unsigned short  capt = 0;

	// for neighbors;

	//for checking
	// int i = 0;
	// check_neighbors(values, positions, &num_neighbors, grid, pos, i++, neighbors);
	// check_neighbors(values, positions, &num_neighbors, grid, pos, i++, neighbors);
	// check_neighbors(values, positions, &num_neighbors, grid, pos, i++, neighbors);
	// check_neighbors(values, positions, &num_neighbors, grid, pos, i++, neighbors);

	//for loop
	for (int i = 0; i < 4; i++)
	{
		int neighbor ;
		if ((pos == 2 || pos == 5) && i == 3)
			continue;
		else if ((pos == 3 || pos == 6) && i == 2)
			continue;
		else
			neighbor = pos + neighbors[i];
		if (in_bounds(neighbor) && (grid[neighbor] >= 1 && grid[neighbor] < 6))
		{
			positions[num_neighbors] = neighbor;
			values[num_neighbors] = grid[neighbor];
			num_neighbors++;
		}
	}

	//if there is no capture;
	if (num_neighbors < 2)
		capt = 0;
	else
	{
		if (num_neighbors == 2)
		{
			if (values[0] + values[1] <= 6)
			{
				int new_grid[9];
				copy_grid(new_grid, grid);
				new_grid[pos] = values[0] + values[1];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				long key = (long long)calculate_hash(new_grid) * (long long)6 + (long long)(depth % 6);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & MODULO;
			capt = 1;
			}
		}
		else if (num_neighbors == 3)
		{
			if (values[0] + values[1] <= 6)
			{
				int new_grid[9];
				copy_grid(new_grid, grid);
				new_grid[pos] = values[0] + values[1];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				long key = (long long)calculate_hash(new_grid) * (long long)6 + (long long)(depth % 6);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & MODULO;
				capt = 1;
			}
			if (values[1] + values[2] <= 6)
			{
				int new_grid[9];
				copy_grid(new_grid, grid);
				new_grid[pos] = values[1] + values[2];
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				long key = (long long)calculate_hash(new_grid) * (long long)6 + (long long)(depth % 6);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & MODULO;
				capt = 1;
			}
			if (values[0] + values[2] <= 6)
			{
				int new_grid[9];
				copy_grid(new_grid, grid);
				new_grid[pos] = values[0] + values[2];
				new_grid[positions[0]] = 0;
				new_grid[positions[2]] = 0;
				long key = (long long)calculate_hash(new_grid) * (long long)6 + (long long)(depth % 6);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & MODULO;
				capt = 1;
			}
			if (values[0] + values[1] + values[2] <= 6)
			{
				int new_grid[9];
				copy_grid(new_grid, grid);
				new_grid[pos] = values[0] + values[1] + values[2];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				long key = (long long)calculate_hash(new_grid) * (long long)6 + (long long)(depth % 6);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & MODULO;
				capt = 1;
			}
		}
		if (num_neighbors == 4)
		{
			if (values[0] + values[1] <= 6)
			{
				int new_grid[9];
				copy_grid(new_grid, grid);
				new_grid[pos] = values[0] + values[1];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				long key = (long long)calculate_hash(new_grid) * (long long)6 + (long long)(depth % 6);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & MODULO;
				capt = 1;
			}
			if (values[1] + values[2] <= 6)
			{
				int new_grid[9];
				copy_grid(new_grid, grid);
				new_grid[pos] = values[1] + values[2];
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				long key = (long long)calculate_hash(new_grid) * (long long)6 + (long long)(depth % 6);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & MODULO;
				capt = 1;
			}
			if (values[2] + values[3] <= 6)
			{
				int new_grid[9];
				copy_grid(new_grid, grid);
				new_grid[pos] = values[2] + values[3];
				new_grid[positions[2]] = 0;
				new_grid[positions[3]] = 0;
				long key = (long long)calculate_hash(new_grid) * (long long)6 + (long long)(depth % 6);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & MODULO;
				capt = 1;
			}
			if (values[0] + values[2] <= 6)
			{
				int new_grid[9];
				copy_grid(new_grid, grid);
				new_grid[pos] = values[0] + values[2];
				new_grid[positions[0]] = 0;
				new_grid[positions[2]] = 0;
				long key = (long long)calculate_hash(new_grid) * (long long)6 + (long long)(depth % 6);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & MODULO;
				capt = 1;
			}
			if (values[0] + values[3] <= 6)
			{
				int new_grid[9];
				copy_grid(new_grid, grid);
				new_grid[pos] = values[0] + values[3];
				new_grid[positions[0]] = 0;
				new_grid[positions[3]] = 0;
				long key = (long long)calculate_hash(new_grid) * (long long)6 + (long long)(depth % 6);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & MODULO;
				capt = 1;
			}
			if (values[1] + values[3] <= 6)
			{
				int new_grid[9];
				copy_grid(new_grid, grid);
				new_grid[pos] = values[1] + values[3];
				new_grid[positions[1]] = 0;
				new_grid[positions[3]] = 0;
				long key = (long long)calculate_hash(new_grid) * (long long)6 + (long long)(depth % 6);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & MODULO;
				capt = 1;
			}
			if (values[0] + values[1]  + values[2] <= 6)
			{
				int new_grid[9];
				copy_grid(new_grid, grid);
				new_grid[pos] = values[0] + values[1] + values[2];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				long key = (long long)calculate_hash(new_grid) * (long long)6 + (long long)(depth % 6);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & MODULO;
				capt = 1;
			}
			if (values[1] + values[2]  + values[3] <= 6)
			{
				int new_grid[9];
				copy_grid(new_grid, grid);
				new_grid[pos] = values[1] + values[2] + values[3];
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				new_grid[positions[3]] = 0;
				long key = (long long)calculate_hash(new_grid) * (long long)6 + (long long)(depth % 6);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & MODULO;
				capt = 1;
			}
			if (values[0] + values[2]  + values[3] <= 6)
			{
				int new_grid[9];
				copy_grid(new_grid, grid);
				new_grid[pos] = values[0] + values[2] + values[3];
				new_grid[positions[0]] = 0;
				new_grid[positions[2]] = 0;
				new_grid[positions[3]] = 0;
				long key = (long long)calculate_hash(new_grid) * (long long)6 + (long long)(depth % 6);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & MODULO;
				capt = 1;
			}
			if (values[0] + values[1]  + values[3] <= 6)
			{
				int new_grid[9];
				copy_grid(new_grid, grid);
				new_grid[pos] = values[0] + values[1] + values[3];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				new_grid[positions[3]] = 0;
				long key = (long long)calculate_hash(new_grid) * (long long)6 + (long long)(depth % 6);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & MODULO;
				capt = 1;
			}
			if (values[0] + values[1]  + values[2] +values[3] <= 6)
			{
				int new_grid[9];
				copy_grid(new_grid, grid);
				new_grid[pos] = values[0] + values[1]  + values[2] +values[3];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				new_grid[positions[3]] = 0;
				long key = (long long)calculate_hash(new_grid) * (long long)6 + (long long)(depth % 6);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & MODULO;
				capt = 1;
			}
		}
	}
	if (capt == 0)
	{
		int new_grid[9];
		copy_grid(new_grid, grid);
		new_grid[pos] = 1;
		long key = (long long)calculate_hash(new_grid) * (long long)6 + (long long)(depth % 6);
		if (!save[key])
			save[key] = simulate(new_grid, depth + 1, max_depth);
		res = (res + save[key]) & MODULO;
	}
	return (res);
}

unsigned int	simulate(int grid[9], int depth, int max_depth)
{
	unsigned short	find_zero = 0;
	unsigned int res = 0;
	
	if (depth == max_depth)
		return (calculate_hash(grid));
	for (int i = 0; i < 9; i++)
	{
		if (grid[i] == 0)
		{
			find_zero = 1;
			res = (res + find_capture(grid, i, depth, max_depth)) & MODULO;
		}
	}
	if (find_zero == 0)
		return (calculate_hash(grid));
	return (res);
}

void	ft_putnbr(unsigned int n)
{
	if (n >= 10)
		ft_putnbr(n / 10);
	char c = ((n % 10) + '0');
	write (1, &c, 1);
}

int main()
{
	int depth;
	int grid[9];
	scanf("%d", &depth);
	for (int i = 0; i < 9; i++)
		scanf("%d", &grid[i]);
	unsigned int res = simulate(grid, 0, depth);
	ft_putnbr(res);
	write (1, "\n", 1);
	return 0;
}