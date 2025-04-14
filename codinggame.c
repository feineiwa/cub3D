#pragma GCC optimize("O3")

#include <stdio.h>
#include <unistd.h>

int				save[3996000000];

unsigned int	simulate(int grid[9], int depth, int max_depth);

unsigned int encode_grid(int grid[9]) {
    unsigned int encoded = 0;
    for (int i = 0; i < 9; i++) {
        encoded |= (grid[i] & 7) << (24 - (i * 3));
    }
    return encoded;
}


unsigned int	find_capture(int *grid, int pos, int depth, int max_depth)
{
	unsigned int	values[4];
	unsigned int	positions[4];
	unsigned short	num_neighbors;
	unsigned int	res;
	unsigned short	capt;
	unsigned short	i;
	unsigned short	neighbor;
	unsigned int key;

	unsigned int neighbors[4] = {-3, 3, -1, 1}; // up down left right
	num_neighbors = 0;
	res = 0;
	capt = 0;
	i = 0;
	neighbor = pos + neighbors[i];
	if ((neighbor >= 0 && neighbor < 9) && (grid[neighbor] >= 1 && grid[neighbor] < 6))
	{
		positions[num_neighbors] = neighbor;
		values[num_neighbors] = grid[neighbor];
		(num_neighbors)++;
	}
	i = 1;
	neighbor = pos + neighbors[i];
	if ((neighbor >= 0 && neighbor < 9) && (grid[neighbor] >= 1 && grid[neighbor] < 6))
	{
		positions[num_neighbors] = neighbor;
		values[num_neighbors] = grid[neighbor];
		(num_neighbors)++;
	}
	i = 2;
	if (pos == 3 || pos == 6)
		neighbor = -1;
	else
		neighbor = pos + neighbors[i];
	if ((neighbor >= 0 && neighbor < 9) && (grid[neighbor] >= 1 && grid[neighbor] < 6))
	{
		positions[num_neighbors] = neighbor;
		values[num_neighbors] = grid[neighbor];
		(num_neighbors)++;
	}
	i = 3;
	if (pos == 2 || pos == 5)
		neighbor = -1;
	else
		neighbor = pos + neighbors[i];
	if ((neighbor >= 0 && neighbor < 9) && (grid[neighbor] >= 1 && grid[neighbor] < 6))
	{
		positions[num_neighbors] = neighbor;
		values[num_neighbors] = grid[neighbor];
		(num_neighbors)++;
	}
	if (num_neighbors < 2)
		capt = 0;
	else
	{
		if (num_neighbors == 2)
		{
			if (values[0] + values[1] <= 6)
			{
				int new_grid[9] = {grid[0], grid[1], grid[2], grid[3], grid[4], grid[5], grid[6], grid[7], grid[8]};
				new_grid[pos] = values[0] + values[1];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				key = (encode_grid(new_grid) << 3) | (depth & 7);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & ((1 << 30) -1);
				capt = 1;
			}
		}
		else if (num_neighbors == 3)
		{
			if (values[0] + values[1] <= 6)
			{
				int new_grid[9] = {grid[0], grid[1], grid[2], grid[3], grid[4], grid[5], grid[6], grid[7], grid[8]};
				new_grid[pos] = values[0] + values[1];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				key = (encode_grid(new_grid) << 3) | (depth & 7);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & ((1 << 30) -1);
				capt = 1;
			}
			if (values[1] + values[2] <= 6)
			{
				int new_grid[9] = {grid[0], grid[1], grid[2], grid[3], grid[4], grid[5], grid[6], grid[7], grid[8]};
				new_grid[pos] = values[1] + values[2];
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				key = (encode_grid(new_grid) << 3) | (depth & 7);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & ((1 << 30) -1);
				capt = 1;
			}
			if (values[0] + values[2] <= 6)
			{
				int new_grid[9] = {grid[0], grid[1], grid[2], grid[3], grid[4], grid[5], grid[6], grid[7], grid[8]};
				new_grid[pos] = values[0] + values[2];
				new_grid[positions[0]] = 0;
				new_grid[positions[2]] = 0;
				key = (encode_grid(new_grid) << 3) | (depth & 7);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & ((1 << 30) -1);
				capt = 1;
			}
			if (values[0] + values[1] + values[2] <= 6)
			{
				int new_grid[9] = {grid[0], grid[1], grid[2], grid[3], grid[4], grid[5], grid[6], grid[7], grid[8]};
				new_grid[pos] = values[0] + values[1] + values[2];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				key = (encode_grid(new_grid) << 3) | (depth & 7);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & ((1 << 30) -1);
				capt = 1;
			}
		}
		if (num_neighbors == 4)
		{
			if (values[0] + values[1] <= 6)
			{
				int new_grid[9] = {grid[0], grid[1], grid[2], grid[3], grid[4], grid[5], grid[6], grid[7], grid[8]};
				new_grid[pos] = values[0] + values[1];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				key = (encode_grid(new_grid) << 3) | (depth & 7);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & ((1 << 30) -1);
				capt = 1;
			}
			if (values[1] + values[2] <= 6)
			{
				int new_grid[9] = {grid[0], grid[1], grid[2], grid[3], grid[4], grid[5], grid[6], grid[7], grid[8]};
				new_grid[pos] = values[1] + values[2];
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				key = (encode_grid(new_grid) << 3) | (depth & 7);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & ((1 << 30) -1);
				capt = 1;
			}
			if (values[2] + values[3] <= 6)
			{
				int new_grid[9] = {grid[0], grid[1], grid[2], grid[3], grid[4], grid[5], grid[6], grid[7], grid[8]};
				new_grid[pos] = values[2] + values[3];
				new_grid[positions[2]] = 0;
				new_grid[positions[3]] = 0;
				key = (encode_grid(new_grid) << 3) | (depth & 7);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & ((1 << 30) -1);
				capt = 1;
			}
			if (values[0] + values[2] <= 6)
			{
				int new_grid[9] = {grid[0], grid[1], grid[2], grid[3], grid[4], grid[5], grid[6], grid[7], grid[8]};
				new_grid[pos] = values[0] + values[2];
				new_grid[positions[0]] = 0;
				new_grid[positions[2]] = 0;
				key = (encode_grid(new_grid) << 3) | (depth & 7);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & ((1 << 30) -1);
				capt = 1;
			}
			if (values[0] + values[3] <= 6)
			{
				int new_grid[9] = {grid[0], grid[1], grid[2], grid[3], grid[4], grid[5], grid[6], grid[7], grid[8]};
				new_grid[pos] = values[0] + values[3];
				new_grid[positions[0]] = 0;
				new_grid[positions[3]] = 0;
				key = (encode_grid(new_grid) << 3) | (depth & 7);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & ((1 << 30) -1);
				capt = 1;
			}
			if (values[1] + values[3] <= 6)
			{
				int new_grid[9] = {grid[0], grid[1], grid[2], grid[3], grid[4], grid[5], grid[6], grid[7], grid[8]};
				new_grid[pos] = values[1] + values[3];
				new_grid[positions[1]] = 0;
				new_grid[positions[3]] = 0;
				key = (encode_grid(new_grid) << 3) | (depth & 7);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & ((1 << 30) -1);
				capt = 1;
			}
			if (values[0] + values[1] + values[2] <= 6)
			{
				int new_grid[9] = {grid[0], grid[1], grid[2], grid[3], grid[4], grid[5], grid[6], grid[7], grid[8]};
				new_grid[pos] = values[0] + values[1] + values[2];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				key = (encode_grid(new_grid) << 3) | (depth & 7);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & ((1 << 30) -1);
				capt = 1;
			}
			if (values[1] + values[2] + values[3] <= 6)
			{
				int new_grid[9] = {grid[0], grid[1], grid[2], grid[3], grid[4], grid[5], grid[6], grid[7], grid[8]};
				new_grid[pos] = values[1] + values[2] + values[3];
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				new_grid[positions[3]] = 0;
				key = (encode_grid(new_grid) << 3) | (depth & 7);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & ((1 << 30) -1);
				capt = 1;
			}
			if (values[0] + values[2] + values[3] <= 6)
			{
				int new_grid[9] = {grid[0], grid[1], grid[2], grid[3], grid[4], grid[5], grid[6], grid[7], grid[8]};
				new_grid[pos] = values[0] + values[2] + values[3];
				new_grid[positions[0]] = 0;
				new_grid[positions[2]] = 0;
				new_grid[positions[3]] = 0;
				key = (encode_grid(new_grid) << 3) | (depth & 7);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & ((1 << 30) -1);
				capt = 1;
			}
			if (values[0] + values[1] + values[3] <= 6)
			{
				int new_grid[9] = {grid[0], grid[1], grid[2], grid[3], grid[4], grid[5], grid[6], grid[7], grid[8]};
				new_grid[pos] = values[0] + values[1] + values[3];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				new_grid[positions[3]] = 0;
				key = (encode_grid(new_grid) << 3) | (depth & 7);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & ((1 << 30) -1);
				capt = 1;
			}
			if (values[0] + values[1] + values[2] + values[3] <= 6)
			{
				int new_grid[9] = {grid[0], grid[1], grid[2], grid[3], grid[4], grid[5], grid[6], grid[7], grid[8]};
				new_grid[pos] = values[0] + values[1] + values[2] + values[3];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				new_grid[positions[3]] = 0;
				key = (encode_grid(new_grid) << 3) | (depth & 7);
				if (!save[key])
					save[key] = simulate(new_grid, depth + 1, max_depth);
				res = (res + save[key]) & ((1 << 30) -1);
				capt = 1;
			}
		}
	}
	if (capt == 0)
	{
		int new_grid[9] = {grid[0], grid[1], grid[2], grid[3], grid[4], grid[5], grid[6], grid[7], grid[8]};
		new_grid[pos] = 1;
		key = (encode_grid(new_grid) << 3) | (depth & 7);
		if (!save[key])
			save[key] = simulate(new_grid, depth + 1, max_depth);
		res = (res + save[key]) & ((1 << 30) -1);
	}
	return (res);
}

unsigned int	simulate(int grid[9], int depth, int max_depth)
{
	unsigned short	find_zero;
	unsigned int	res;

	find_zero = 0;
	res = 0;
	if (depth == max_depth)
		return (grid[0] * 100000000 + grid[1] * 10000000 + grid[2] * 1000000
			+ grid[3] * 100000 + grid[4] * 10000 + grid[5] * 1000 + grid[6]
			* 100 + grid[7] * 10 + grid[8]);
	for (int i = 0; i < 9; i++)
	{
		if (grid[i] == 0)
		{
			find_zero = 1;
			res = (res + find_capture(grid, i, depth, max_depth)) & ((1 << 30) -1);
		}
	}
	if (find_zero == 0)
		return (grid[0] * 100000000 + grid[1] * 10000000 + grid[2] * 1000000
			+ grid[3] * 100000 + grid[4] * 10000 + grid[5] * 1000 + grid[6]
			* 100 + grid[7] * 10 + grid[8]);
	return (res);
}

void	ft_putnbr(unsigned int n)
{
	char	c;

	if (n >= 10)
		ft_putnbr(n / 10);
	c = ((n % 10) + '0');
	write(1, &c, 1);
}

int	main(void)
{
	int depth;
	int grid[9];
	scanf("%d", &depth);
	for (int i = 0; i < 9; i++)
		scanf("%d", &grid[i]);
	unsigned int res = simulate(grid, 0, depth);
	ft_putnbr(res);
	write(1, "\n", 1);
	return (0);
}