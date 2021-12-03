#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
#include <functional>
#include <algorithm>

#define USE_EXAMPLE 0

#if USE_EXAMPLE
    #define LENGTH 5
    #define FILE "./example.txt"
#else
    #define LENGTH 12
    #define FILE "./input.txt"
#endif

void part_one()
{
    std::ifstream input(FILE);

    int counts[LENGTH] = {0};

    std::string entry;
    while (input >> entry)
    {
        for (int i = 0; i < LENGTH; ++i)
        {
            counts[i] += (entry[i] == '1') ? 1 : -1;
        }
    }

    int gamma_rate = 0;
    for (int i = 0; i < LENGTH; ++i)
    {
        gamma_rate += pow(2, LENGTH-1-i) * (counts[i] > 0 ? 1 : 0);
    }

    int epsilon_rate = ((int)pow(2, LENGTH)-1) ^ gamma_rate;

    printf("%d\n", gamma_rate * epsilon_rate);
}

void part_two()
{
    std::ifstream input(FILE);

    std::vector<std::string> entries;
    std::string entry;
    while (input >> entry)
    {
        entries.push_back(entry);
    }

    auto filter_entries = [entries](std::function<char (int)> keep_char)
    {
        std::vector<std::string> local_entries = entries;

        for (int i = 0; i < LENGTH && local_entries.size() > 1; ++i)
        {
            int count = 0;
            for (std::string entry : local_entries)
            {
                count += entry[i] == '1' ? 1 : -1;
            }

            char keep = keep_char(count);
            std::vector<std::string> survivors;
            std::copy_if(local_entries.begin(), local_entries.end(), std::back_inserter(survivors),
                [keep, i](std::string entry){ return entry[i] == keep; }
            );
            local_entries = survivors;
        }

        return std::stoi(local_entries[0], nullptr, 2);
    };

    int oxygen_rating = filter_entries([](int count){ return count >= 0 ? '1' : '0'; });
    int co2_rating    = filter_entries([](int count){ return count <  0 ? '1' : '0'; });

    printf("%d\n", oxygen_rating * co2_rating);
}

int main()
{
    part_one();
    part_two();
    return 0;
}