#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <functional>
#include <queue>

#define SIZE 10
#define ITERATIONS 100

#define USE_EXAMPLE 0
#if USE_EXAMPLE
    #define FILE "./example.txt"
    #define P1_EXPECT 1656
    #define P2_EXPECT 195
#else
    #define FILE "./input.txt"
    #define P1_EXPECT 1723
    #define P2_EXPECT 327
#endif

struct grid_pos
{
    int row;
    int col;
};

void dump_grid(int grid[][SIZE])
{
    for (int row = 0; row < SIZE; ++row)
    {
        for (int col = 0; col < SIZE; ++col)
        {
            if (grid[col][row] == 0)
            {
                printf("\033[31;1;4m0\033[0m");
            }
            else
            {
                printf("%d", grid[col][row]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void increment(int** grid, bool flashes[][SIZE], int row, int col)
{
    if (row >= 0 && row < SIZE && col >= 0 && col < SIZE)
    {
        if (!flashes[col][row])
        {
            ++grid[col][row];
        }
    }
}

int do_count_flashes(bool flashes[][SIZE])
{
    int count = 0;
    for (int row = 0; row < SIZE; ++row)
    {
        for (int col = 0; col < SIZE; ++col)
        {
            if (flashes[col][row])
            {
                ++count;
            }
        }
    }
    return count;
}

int tick_grid(int** grid)
{
    // Increment each octopus
    for (int row = 0; row < SIZE; ++row)
    {
        for (int col = 0; col < SIZE; ++col)
        {
            ++grid[col][row];
        }
    }

    int count_flashes = 0;
    int count_flashes_new = 0;
    bool flashes[SIZE][SIZE] = { 0 };
    do
    {
        count_flashes = count_flashes_new;

        for (int row = 0; row < SIZE; ++row)
        {
            for (int col = 0; col < SIZE; ++col)
            {
                if (grid[col][row] > 9 && !flashes[col][row])
                {
                    flashes[col][row] = true;
                    grid[col][row] = 0;
                    increment(grid, flashes, row + 1, col - 1);
                    increment(grid, flashes, row + 1, col);
                    increment(grid, flashes, row + 1, col + 1);
                    increment(grid, flashes, row, col + 1);
                    increment(grid, flashes, row - 1, col + 1);
                    increment(grid, flashes, row - 1, col);
                    increment(grid, flashes, row - 1, col - 1);
                    increment(grid, flashes, row, col - 1);
                }
            }
        }

        count_flashes_new = do_count_flashes(flashes);
    } while (count_flashes != count_flashes_new);

    return count_flashes;
}

int** parse_file()
{
    std::fstream input(FILE);

    int** grid = new int*[SIZE];
    for (int i = 0; i < 10; ++i)
    {
        grid[i] = new int[SIZE];
    }

    int row = 0;
    int col = 0;
    std::string line;
    while (input >> line)
    {
        for (int i = 0; i < SIZE; ++i)
        {
            grid[col++][row] = ((int)line[i]) - '0';
        }
        col = 0;
        ++row;
    }

    return grid;
}

int part_one()
{
    int** grid = parse_file();

    int total_flashes = 0;
    for (int i = 0; i < ITERATIONS; ++i)
    {
        total_flashes += tick_grid(grid);
    }
    
    printf("%d\n", total_flashes);
    return total_flashes;
}


int part_two()
{
    int** grid = parse_file();

    int step = 0;
    while (true)
    {
        ++step;

        int count_flashes = tick_grid(grid);

        if (count_flashes == SIZE * SIZE)
        {
            break;
        }
    }

    printf("%d\n", step);
    return step;
}


int main()
{
    assert(part_one() == P1_EXPECT);
    assert(part_two() == P2_EXPECT);
}