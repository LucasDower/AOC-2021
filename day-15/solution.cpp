search#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <set>
#include <queue>
#include <utility>

#define USE_EXAMPLE 0
#if USE_EXAMPLE
	#define FILE "example.txt"
    #define SIZE 10
    #define P1_EXPECT 40
    #define P2_EXPECT 315
#else
	#define FILE "input.txt"
    #define SIZE 100
    #define P1_EXPECT 393
    #define P2_EXPECT 3776553567525
#endif

struct t_grid_pos
{
    int col;
    int row;
    uint32_t dist;
    uint32_t weight;
    t_grid_pos* prev;
    uint32_t priority;

    t_grid_pos() : col(0), row(0), dist(UINT32_MAX), prev(nullptr), weight(0), priority(dist) {}
    t_grid_pos(int col, int row) : col(col), row(row), dist(UINT32_MAX), prev(nullptr), weight(0), priority(dist) {}

    bool operator<(const t_grid_pos& other) const
    {
        return priority > other.priority;
    }
    
    bool operator==(const t_grid_pos& other) const
    {
        return col == other.col && row == other.row;
    }
};

namespace std {
    template <>
    struct hash<t_grid_pos>
    {
        std::size_t operator()(const t_grid_pos& pair) const
        {
            return pair.row * 1000 + pair.col;
        }
    };
}

template <unsigned int size>
int search(t_grid_pos grid[][size], std::vector<t_grid_pos>& vertex_queue)
{
    std::sort(vertex_queue.begin(), vertex_queue.end(), [](const t_grid_pos& a, const t_grid_pos& b) { return a.priority < b.priority; });
    std::vector<t_grid_pos> neighbour_deltas = { { -1, 0 }, { 1, 0 }, { 0,-1 }, { 0, 1 } };
    size_t s = vertex_queue.size();
    while (!vertex_queue.empty())
    {
        --s;
        //printf("%d\n", vertex_queue.size());
        t_grid_pos u = vertex_queue[0];
        //printf("Looking at (%d,%d)\n", u.col, u.row);
        vertex_queue.erase(vertex_queue.begin());

        for (t_grid_pos& nd : neighbour_deltas)
        {
            int n_col = u.col + nd.col;
            int n_row = u.row + nd.row;
            if (n_col >= 0 && n_col < size && n_row >= 0 && n_row < size)
            {
                t_grid_pos& v = grid[n_col][n_row];
                uint32_t alt = u.dist + v.weight;
                if (alt < v.dist)
                {
                    v.dist = alt;
                    v.prev = &u;
                    v.priority = alt;

                    // Remove
                    bool found = false;
                    for (size_t i = 0; i < s; ++i)
                    {
                        t_grid_pos& t = vertex_queue[i];
                        if (t.col == v.col && t.row == v.row)
                        {
                            //printf("Removing (%d,%d) at index %d\n", v.col, v.row, i);
                            vertex_queue.erase(vertex_queue.begin() + i);
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        assert(0);
                    }
                    // Insert
                    found = false;
                    for (size_t i = 0; i < vertex_queue.size(); ++i)
                    {
                        t_grid_pos& t = vertex_queue[i];
                        if (t.dist > v.dist)
                        {
                            //printf("Inserting (%d,%d) at index %d\n\n", v.col, v.row, i);
                            vertex_queue.insert(vertex_queue.begin() + i, v);
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        //assert(0);
                        vertex_queue.insert(vertex_queue.end(), v);
                    }
                }
            }
        }
    }

    printf("%d\n", grid[size - 1][size - 1].dist);
    return grid[size - 1][size - 1].dist;
}

int main()
{
    std::ifstream input(FILE);
    std::string line;

    auto grid = new t_grid_pos[SIZE][SIZE];
    auto big_grid = new t_grid_pos[SIZE*5][SIZE*5];
    
    std::vector<t_grid_pos> vertex_queue;
    std::vector<t_grid_pos> big_vertex_queue;

    int row = 0;
    while (input >> line)
    {
        assert(line.size() == SIZE);
        for (int col = 0; col < SIZE; ++col)
        {
            t_grid_pos grid_pos(col, row);
            grid_pos.weight = line[col] - 48;

            for (int i = 0; i < 5; ++i)
            {
                for (int j = 0; j < 5; ++j)
                {
                    t_grid_pos n = grid_pos;
                    n.weight += i + j;
                    if (n.weight > 9) { n.weight -= 9; }
                    n.col += i * SIZE;
                    n.row += j * SIZE;
                    if (col == 0 && row == 0 && i == 0 && j == 0)
                    {
                        n.dist = 0;
                        n.priority = 0;
                    }
                    big_grid[n.col][n.row] = n;
                    big_vertex_queue.push_back(big_grid[n.col][n.row]);
                }
            }

            if (col == 0 && row == 0)
            {
                grid_pos.dist = 0;
                grid_pos.priority = 0;
            }
            grid[col][row] = grid_pos;
            vertex_queue.push_back(grid[col][row]);

        }
        ++row;
    }

    search<SIZE>(grid, vertex_queue);
    search<SIZE*5>(big_grid, big_vertex_queue);

    return 0;
}