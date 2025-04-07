#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define MODULO ((1 << 30) - 1)


void	simulate(int grid[9], int depth, int max_depth, unsigned int *res);

int calculate_hash(int grid[9])
{
	int hash = 0;
	
	for (int i = 0; i < 9; i++)
		hash = hash * 10 + grid[i];
	return (hash);
}

int	in_bounds(int pos)
{
	if (pos >= 0 && pos < 9)
		return (1);
	return (0);
}

void	find_capture(int *grid, int pos, int *capt, int depth, int max_depth, unsigned int *res)
{
	int neighbors[4] = {-3, 3, -1, 1}; //up down left right

	int values[4];
	int positions[4];
	int	num_neighbors = 0;
	for (int i = 0; i < 4; i++)
	{
		int neighbor ;
		if ((pos == 2 || pos == 5) && i == 3)
			neighbor = -1;
		else if ((pos == 3 || pos == 6) && i == 2)
			neighbor = -1;
		else
			neighbor = pos + neighbors[i];
		if (in_bounds(neighbor) && (grid[neighbor] >= 1 && grid[neighbor] < 6))
		{
			positions[num_neighbors] = neighbor;
			values[num_neighbors] = grid[neighbor];
			num_neighbors++;
		}
	}
	if (num_neighbors < 2)
		*capt = 0;
	else
	{
		if (num_neighbors == 2)
		{
			if (values[0] + values[1] <= 6)
			{
				int new_grid[9];
				memcpy(new_grid, grid, sizeof(int) * 9);
				new_grid[pos] = values[0] + values[1];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				simulate(new_grid, depth + 1, max_depth, res);
				*capt = 1;
			}
		}
		if (num_neighbors == 3)
		{
			if (values[0] + values[1] <= 6)
			{
				int new_grid[9];
				memcpy(new_grid, grid, sizeof(int) * 9);
				new_grid[pos] = values[0] + values[1];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				simulate(new_grid, depth + 1, max_depth, res);
				*capt = 1;
			}
			if (values[1] + values[2] <= 6)
			{
				int new_grid[9];
				memcpy(new_grid, grid, sizeof(int) * 9);
				new_grid[pos] = values[1] + values[2];
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				simulate(new_grid, depth + 1, max_depth, res);
				*capt = 1;
			}
			if (values[0] + values[2] <= 6)
			{
				int new_grid[9];
				memcpy(new_grid, grid, sizeof(int) * 9);
				new_grid[pos] = values[0] + values[2];
				new_grid[positions[0]] = 0;
				new_grid[positions[2]] = 0;
				simulate(new_grid, depth + 1, max_depth, res);
				*capt = 1;
			}
			if (values[0] + values[1] + values[2] <= 6)
			{
				int new_grid[9];
				memcpy(new_grid, grid, sizeof(int) * 9);
				new_grid[pos] = values[0] + values[1] + values[2];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				simulate(new_grid, depth + 1, max_depth, res);
				*capt = 1;
			}
		}
		if (num_neighbors == 4)
		{
			if (values[0] + values[1] <= 6)
			{
				int new_grid[9];
				memcpy(new_grid, grid, sizeof(int) * 9);
				new_grid[pos] = values[0] + values[1];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				simulate(new_grid, depth + 1, max_depth, res);
				*capt = 1;
			}
			if (values[1] + values[2] <= 6)
			{
				int new_grid[9];
				memcpy(new_grid, grid, sizeof(int) * 9);
				new_grid[pos] = values[1] + values[2];
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				simulate(new_grid, depth + 1, max_depth, res);
				*capt = 1;
			}
			if (values[2] + values[3] <= 6)
			{
				int new_grid[9];
				memcpy(new_grid, grid, sizeof(int) * 9);
				new_grid[pos] = values[2] + values[3];
				new_grid[positions[2]] = 0;
				new_grid[positions[3]] = 0;
				simulate(new_grid, depth + 1, max_depth, res);
				*capt = 1;
			}
			if (values[0] + values[2] <= 6)
			{
				int new_grid[9];
				memcpy(new_grid, grid, sizeof(int) * 9);
				new_grid[pos] = values[0] + values[2];
				new_grid[positions[0]] = 0;
				new_grid[positions[2]] = 0;
				simulate(new_grid, depth + 1, max_depth, res);
				*capt = 1;
			}
			if (values[0] + values[3] <= 6)
			{
				int new_grid[9];
				memcpy(new_grid, grid, sizeof(int) * 9);
				new_grid[pos] = values[0] + values[3];
				new_grid[positions[0]] = 0;
				new_grid[positions[3]] = 0;
				simulate(new_grid, depth + 1, max_depth, res);
				*capt = 1;
			}
			if (values[1] + values[3] <= 6)
			{
				int new_grid[9];
				memcpy(new_grid, grid, sizeof(int) * 9);
				new_grid[pos] = values[1] + values[3];
				new_grid[positions[1]] = 0;
				new_grid[positions[3]] = 0;
				simulate(new_grid, depth + 1, max_depth, res);
				*capt = 1;
			}
			if (values[0] + values[1]  + values[2] <= 6)
			{
				int new_grid[9];
				memcpy(new_grid, grid, sizeof(int) * 9);
				new_grid[pos] = values[0] + values[1] + values[2];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				simulate(new_grid, depth + 1, max_depth, res);
				*capt = 1;
			}
			if (values[1] + values[2]  + values[3] <= 6)
			{
				int new_grid[9];
				memcpy(new_grid, grid, sizeof(int) * 9);
				new_grid[pos] = values[1] + values[2] + values[3];
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				new_grid[positions[3]] = 0;
				simulate(new_grid, depth + 1, max_depth, res);
				*capt = 1;
			}
			if (values[0] + values[2]  + values[3] <= 6)
			{
				int new_grid[9];
				memcpy(new_grid, grid, sizeof(int) * 9);
				new_grid[pos] = values[0] + values[2] + values[3];
				new_grid[positions[0]] = 0;
				new_grid[positions[2]] = 0;
				new_grid[positions[3]] = 0;
				simulate(new_grid, depth + 1, max_depth, res);
				*capt = 1;
			}
			if (values[0] + values[1]  + values[3] <= 6)
			{
				int new_grid[9];
				memcpy(new_grid, grid, sizeof(int) * 9);
				new_grid[pos] = values[0] + values[1] + values[3];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				new_grid[positions[3]] = 0;
				simulate(new_grid, depth + 1, max_depth, res);
				*capt = 1;
			}
			if (values[0] + values[1]  + values[2] +values[3] <= 6)
			{
				int new_grid[9];
				memcpy(new_grid, grid, sizeof(int) * 9);
				new_grid[pos] = values[0] + values[1]  + values[2] +values[3];
				new_grid[positions[0]] = 0;
				new_grid[positions[1]] = 0;
				new_grid[positions[2]] = 0;
				new_grid[positions[3]] = 0;
				simulate(new_grid, depth + 1, max_depth, res);
				*capt = 1;
			}
		}		
	}
}

void	simulate(int grid[9], int depth, int max_depth, unsigned int *res)
{
	int	find_zero = 0;

	if (depth == max_depth)
	{
		*res = (*res + calculate_hash(grid)) & MODULO;
		return ;
	}
	
	for (int i = 0; i < 9; i++)
	{
		if (grid[i] == 0)
		{
			find_zero = 1;
			int capt = 0;
			find_capture(grid, i, &capt, depth, max_depth, res);
			
			if (capt == 0)
			{
				int new_grid[9];
				memcpy(new_grid, grid, sizeof(int) * 9);
				new_grid[i] = 1;
				simulate(new_grid, depth + 1, max_depth, res);
			}
		}
	}
	if (find_zero == 0)
	{
		*res = (*res + calculate_hash(grid)) & MODULO;
		return ;
	}
}

void ft_putchar_fd(char c, int fd)
{
	write (fd, &c, 1);
}

void	ft_putnbr_fd(unsigned int n, int fd)
{
	char	digit;

	if (n >= 0 && n < 10)
	{
		digit = n + '0';
		ft_putchar_fd(digit, fd);
	}
	else if (n < 0)
	{
		ft_putchar_fd('-', fd);
		ft_putnbr_fd((n * (-1)), fd);
	}
	else
	{
		ft_putnbr_fd((n / 10), fd);
		ft_putnbr_fd((n % 10), fd);
	}
}

int main()
{
	int depth;
	int grid[9];
	scanf("%d", &depth);
	int	 i = 0;
	while (i < 9)
	{
		scanf("%d", &grid[i]);
		i++;
	}
	unsigned int res = 0;
	simulate(grid, 0, depth, &res);
	ft_putnbr_fd(res, 1);
	ft_putchar_fd('\n', 1);
	return 0;
}