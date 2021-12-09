#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <set>
#include <algorithm>

#define USE_EXAMPLE 0
#if USE_EXAMPLE
    #define FILE "./example.txt"
    #define WIDTH 10
    #define HEIGHT 5
    #define P1_EXPECT 15
    #define P2_EXPECT 1134
#else
    #define FILE "./input.txt"
    #define WIDTH 100
    #define HEIGHT 100
    #define P1_EXPECT 522
    #define P2_EXPECT 916688
#endif

int part_one()
{
    std::fstream input(FILE);

    int grid[HEIGHT][WIDTH];

    std::string line = "";
    int row = 0;
    while (input >> line)
    {
        for (int col = 0; col < WIDTH; ++col)
        {
            grid[row][col] = line[col] - 48;
        }
        ++row;
    }

    int risk_level_sum = 0;
    for (int row = 0; row < HEIGHT; ++row)
    {
        for (int col = 0; col < WIDTH; ++col)
        {
            bool isLowest = true;
            int height = grid[row][col];
            if (col > 0 && grid[row][col-1] <= height) { isLowest = false; }
            if (col < WIDTH-1 && grid[row][col+1] <= height) { isLowest = false; }
            if (row > 0 && grid[row-1][col] <= height) { isLowest = false; }
            if (row < HEIGHT-1 && grid[row+1][col] <= height) { isLowest = false; }
            if (isLowest)
            {
                int risk_level = 1 + height;
                risk_level_sum += risk_level;
            }
        }
    }

    printf("%d\n", risk_level_sum);
    return risk_level_sum;
}

int part_two()
{
    std::fstream input(FILE);

    int grid[HEIGHT][WIDTH];

    std::string line = "";
    int row = 0;
    while (input >> line)
    {
        for (int col = 0; col < WIDTH; ++col)
        {
            grid[row][col] = line[col] - 48;
        }
        ++row;
    }

    struct grid_pos
    {
        int row;
        int col;
    };

    auto find_low_point = [grid](int row, int col)
    {
        int height = grid[row][col];
        if (col > 0 && grid[row][col - 1] < height) { return grid_pos({ row, col - 1 }); };
        if (col < WIDTH - 1 && grid[row][col + 1] < height) { return grid_pos({ row, col + 1 }); };
        if (row > 0 && grid[row - 1][col] < height) { return grid_pos({ row - 1, col }); };
        if (row < HEIGHT - 1 && grid[row + 1][col] < height) { return grid_pos({ row + 1, col }); };
        return grid_pos({ -1, -1 });
    };


    std::set<int> basins_pos_encodings;
    std::unordered_map<int, int> basin_sizes;
    for (int row = 0; row < HEIGHT; ++row)
    {
        for (int col = 0; col < WIDTH; ++col)
        {
            int height = grid[row][col];
            if (height == 9) { continue; }
            int r = row;
            int c = col;
            grid_pos low_point = find_low_point(r, c);
            // could cache points found along the way
            while (low_point.row != -1 && low_point.col != -1)
            {
                r = low_point.row;
                c = low_point.col;
                low_point = find_low_point(r, c);
            };
            //printf("(%d, %d) leads to (%d, %d)\n", row, col, r, c);

            low_point = { r, c };
            int basin_pos_encoding = low_point.row * WIDTH + low_point.col;

            if (basin_sizes.find(basin_pos_encoding) == basin_sizes.end())
            {
                basin_sizes.insert({ basin_pos_encoding, 1 });
            }
            else
            {
                ++basin_sizes.at(basin_pos_encoding);
                basins_pos_encodings.insert(basin_pos_encoding);
            }
        }
    }

    std::vector<int> basin_sorted_sizes;
    for (auto basin_enc : basins_pos_encodings)
    {
        basin_sorted_sizes.push_back(basin_sizes.at(basin_enc));
    }

    std::sort(basin_sorted_sizes.begin(), basin_sorted_sizes.end());

    int l = basin_sorted_sizes.size();
    int result = basin_sorted_sizes[l - 1] * basin_sorted_sizes[l - 2] * basin_sorted_sizes[l - 3];
    
    printf("%d\n", result);
    return result;
}


int main()
{
    assert(part_one() == P1_EXPECT);
    assert(part_two() == P2_EXPECT);
    return 0;
}