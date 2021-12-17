#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

#define USE_EXAMPLE 0
#if USE_EXAMPLE
	#define FILE "example.txt"
#else
	#define FILE "input.txt"
#endif

struct point
{
    int x;
    int y;
};

struct fold
{
    bool is_vertical;
    int loc;
};

void parse_file(std::vector<point>& paper, std::vector<fold>& folds)
{
    std::string line;
    std::ifstream file(FILE);
    while (std::getline(file, line))
    {
        int x = 0; int y = 0;
        char dir = 0; int loc = 0;
        if (sscanf(line.c_str(), "%d,%d", &x, &y))
        {
            paper.push_back({ x, y });
        }
        else if (sscanf(line.c_str(), "fold along %c=%d", &dir, &loc))
        {
            folds.push_back({ dir == 'x', loc });
        }
    }
    paper.pop_back();
}

void dump_paper(const std::vector<point>& paper)
{
    int max_x = (*std::max_element(paper.begin(), paper.end(), [](const point& a, const point& b) { return a.x < b.x; })).x;
    int max_y = (*std::max_element(paper.begin(), paper.end(), [](const point& a, const point& b) { return a.y < b.y; })).y;
    for (int y = 0; y <= max_y; ++y)
    {
        for (int x = 0; x <= max_x; ++x)
        {
            bool found = false;
            for (const point& p : paper)
            {
                if (p.x == x && p.y == y)
                {
                    found = true;
                    break;
                }
            }
            printf("%c", found ? '#' : '.');
        }
        printf("\n");
    }
}

int count_points(std::vector<point> paper)
{
    int count = 0;
    std::unordered_set<int> set;
    for (const point& p : paper)
    {
        int hash = p.x + 100000 * p.y;
        if (set.count(hash) == 0)
        {
            set.insert(hash);
            ++count;
        }
    }
    return count;
}

void fold_paper(const fold& f, std::vector<point>& paper)
{
    if (!f.is_vertical)
    {
        for (point& p : paper)
        {
            if (p.y >= f.loc)
            {
                p.y = (f.loc - (p.y - f.loc));
            }
        }
    }
    else
    {
        for (point& p : paper)
        {
            if (p.x >= f.loc)
            {
                p.x = (f.loc - (p.x - f.loc));
            }
        }
    }
}

int main()
{
    std::vector<point> paper;
    std::vector<fold> folds;
    parse_file(paper, folds);

    // Part 1
    fold_paper(folds[0], paper);
    int count = count_points(paper);
    printf("%d\n", count);

    // Part 2
    for (int i = 1; i < folds.size(); ++i)
    {
        fold_paper(folds[i], paper);
    }
    dump_paper(paper);

	return 0;
}