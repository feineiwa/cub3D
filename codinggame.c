#pragma GCC optimize("O3")

#include <stdio.h>
#include <unistd.h>



// void	draw_rotated_square(t_image *img, t_vector s_p, t_vector c, int size, double angle, int color)
// {
// 	int		half;
// 	t_vector corners[4];
// 	t_vector start, end;
// 	int		y, i;

// 	half = size / 2;

// 	// Calculate the four rotated corners of the square
// 	corners[0].x = s_p.x + (-half * cos(angle) - (-half) * sin(angle));
// 	corners[0].y = s_p.y + (-half * sin(angle) + (-half) * cos(angle));

// 	corners[1].x = s_p.x + (half * cos(angle) - (-half) * sin(angle));
// 	corners[1].y = s_p.y + (half * sin(angle) + (-half) * cos(angle));

// 	corners[2].x = s_p.x + (half * cos(angle) - (half) * sin(angle));
// 	corners[2].y = s_p.y + (half * sin(angle) + (half) * cos(angle));

// 	corners[3].x = s_p.x + (-half * cos(angle) - (half) * sin(angle));
// 	corners[3].y = s_p.y + (-half * sin(angle) + (half) * cos(angle));

// 	// Fill the square by iterating over each scanline inside its bounding box
// 	int min_y = corners[0].y, max_y = corners[0].y;

// 	// Find the min and max y-values (bounding box for the square)
// 	for (i = 1; i < 4; i++)
// 	{
// 		if (corners[i].y < min_y)
// 			min_y = corners[i].y;
// 		if (corners[i].y > max_y)
// 			max_y = corners[i].y;
// 	}

// 	// Iterate through each scanline in the bounding box
// 	for (y = min_y; y <= max_y; y++)
// 	{
// 		double intersections[4]; // Store x-coordinates of intersections with the edges
// 		int count = 0;

// 		// Find intersections of the scanline with the square's edges
// 		for (i = 0; i < 4; i++)
// 		{
// 			t_vector a = corners[i];
// 			t_vector b = corners[(i + 1) % 4]; // Next vertex (edge wrap-around)

// 			// Check if the edge crosses the scanline
// 			if ((y >= a.y && y < b.y) || (y >= b.y && y < a.y))
// 			{
// 				// Compute the x-coordinate of the intersection
// 				double t = (y - a.y) / (b.y - a.y); // Parametric line equation
// 				intersections[count++] = a.x + t * (b.x - a.x);
// 			}
// 		}

// 		// Sort the intersections by x-coordinate (left to right)
// 		if (count >= 2)
// 		{
// 			if (intersections[0] > intersections[1])
// 			{
// 				double temp = intersections[0];
// 				intersections[0] = intersections[1];
// 				intersections[1] = temp;
// 			}

// 			// Draw a horizontal line between the two intersections
// 			start.x = intersections[0];
// 			start.y = y;
// 			end.x = intersections[1];
// 			end.y = y;

// 			draw_line(img, start, end, color);
// 		}
// 	}
// }


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