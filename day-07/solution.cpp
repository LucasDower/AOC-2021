#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

#define USE_EXAMPLE 0
#if USE_EXAMPLE
    #define FILE "./example.txt"
#else
    #define FILE "./input.txt"
#endif

void part_one()
{
    std::ifstream input(FILE);

    std::set<int> alignments;
    std::vector<int> numbers;
    {
        int value = 0;
        char ignore = 0;
        input >> value;
        numbers.push_back(value);
        while (input >> ignore && input >> value)
        {
            numbers.push_back(value);
            alignments.insert(value);
        }
    }
    
    int min_total_fuel = INT_MAX;
    int min_alignment = 0;
    for (int alignment : alignments)
    {
        int total_fuel = 0;
        for (int number : numbers)
        {
            total_fuel += std::abs(number - alignment);
        }
        if (total_fuel < min_total_fuel)
        {
            min_total_fuel = total_fuel;
            min_alignment = alignment;
        }
    }

    printf("%d\n", min_total_fuel);
}

void part_two()
{
    std::ifstream input(FILE);

    std::vector<int> numbers;
    {
        int value = 0;
        char ignore = 0;
        input >> value;
        numbers.push_back(value);
        while (input >> ignore && input >> value)
        {
            numbers.push_back(value);
        }
    }

    int start_alignment = *std::min_element(numbers.begin(), numbers.end());
    int end_alignment = *std::max_element(numbers.begin(), numbers.end());

    int min_total_fuel = INT_MAX;
    int min_alignment = 0;
    for (int a = min_alignment; a <= end_alignment; ++a)
    {
        int total_fuel = 0;
        for (int number : numbers)
        {
            int n = std::abs(number - a);
            int fuel = n * (n + 1) / 2; // sum of natural numbers eqn
            total_fuel += fuel;
        }
        if (total_fuel < min_total_fuel)
        {
            min_total_fuel = total_fuel;
            min_alignment = a;
        }
    }

    printf("%d\n", min_total_fuel);
}


int main()
{
    part_one();
    part_two();
    return 0;
}