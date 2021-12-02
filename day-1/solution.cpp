#include <iostream>
#include <fstream>

void part_one()
{
    std::ifstream input("./input.txt");

    int prev_depth, depth;
    int count = 0;
    input >> prev_depth;
    while (input >> depth)
    {
        if (depth > prev_depth)
        {
            ++count;
        }
        prev_depth = depth;
    }

    printf("%d\n", count);
}

void part_two()
{
    std::ifstream input("./input.txt");

    int a, b, c, d;
    int count = 0;
    input >> a;
    input >> b;
    input >> c;
    while (input >> d)
    {
        if (b + c + d > a + b + c)
        {
            ++count;
        }
        a = b;
        b = c;
        c = d;
    }

    printf("%d\n", count);
}

int main()
{
    part_one();
    part_two();
    return 0;
}