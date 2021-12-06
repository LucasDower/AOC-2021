#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#define USE_EXAMPLE 0
#define P1_DAYS 80
#define P2_DAYS 256
#if USE_EXAMPLE
    #define FILE "./example.txt"
#else
    #define FILE "./input.txt"
#endif
   
std::vector<int> parse_file()
{
    std::ifstream input(FILE);

    std::vector<int> lanternfish;
    {
        int timer = 0;
        char ignore = 0;
        input >> timer;
        lanternfish.push_back(timer);
        while (input >> ignore && input >> timer)
        {
            lanternfish.push_back(timer);
        }
    }

    return lanternfish;
}

void part_one()
{
    std::vector<int> lanternfish = parse_file();

    for (int i = 0; i < P1_DAYS; ++i)
    {
        int starting_size = lanternfish.size();
        for (int j = 0; j < starting_size; ++j)
        {
            if (lanternfish[j] == 0)
            {
                lanternfish[j] = 6;
                lanternfish.push_back(8);
            }
            else
            {
                --lanternfish[j];
            }
        }

        for (int i = 0; i < 9; ++i)
        {
            int count = 0;
            for (int l = 0; l < lanternfish.size(); ++l)
            {
                if (lanternfish[l] == i)
                {
                    ++count;
                }
            }
        }
    }

    printf("%llu\n", lanternfish.size());
}

void part_two()
{
    std::vector<int> lanternfish = parse_file();

    unsigned long long timers[9] = { 0 };

    for (int lf : lanternfish)
    {
        ++timers[lf];
    }

    for (int i = 0; i < P2_DAYS; ++i)
    {
        std::rotate(&timers[0], &timers[0] + 1, &timers[9]);
        timers[6] += timers[8];
    }

    unsigned long long count_fish = 0;
    for (int i = 0; i < 9; ++i)
    {
        count_fish += timers[i];
    }

    printf("%llu\n", count_fish);
}

int main()
{
    part_one();
    part_two();
    return 0;
}
