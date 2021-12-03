#include <iostream>
#include <fstream>
#include <string>

void part_one()
{
    std::ifstream input("./input.txt");

    std::string direction;
    int units;
    int distance = 0;
    int depth = 0;

    while (input >> direction && input >> units)
    {
        if (direction == "forward")
        {
            distance += units;
        }
        else if (direction == "down")
        {
            depth += units;
        }
        else if (direction == "up")
        {
            depth -= units;
        }
    }

    printf("%d\n", distance * depth);
}

void part_two()
{
    std::ifstream input("./input.txt");

    std::string direction;
    int units;
    int distance = 0;
    int depth = 0;
    int aim = 0;

    while (input >> direction && input >> units)
    {
        if (direction == "forward")
        {
            distance += units;
            depth += aim * units;
        }
        else if (direction == "down")
        {
            aim += units;
        }
        else if (direction == "up")
        {
            aim -= units;
        }
    }

    printf("%d\n", distance * depth);
}

int main()
{
    part_one();
    part_two();
    return 0;
}