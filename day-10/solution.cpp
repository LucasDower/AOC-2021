#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <stack>
#include <unordered_map>
#include <cstdint>
#include <algorithm>

#define USE_EXAMPLE 0
#if USE_EXAMPLE
    #define FILE "./example.txt"
    #define P1_EXPECT 26397
    #define P2_EXPECT 288957
#else
    #define FILE "./input.txt"
    #define P1_EXPECT 315693
    #define P2_EXPECT 1870887234
#endif

char open[]  = { '(', '[', '{', '<' };
std::unordered_map<char, char> pair = { { '(', ')' }, { '[', ']' }, { '{', '}' }, { '<', '>' } };
std::unordered_map<char, uint32_t> scores = { { ')', 3 }, { ']', 57 }, { '}', 1197 }, { '>', 25137 } };
std::unordered_map<char, uint32_t> scores2 = { { ')', 1 }, { ']', 2 }, { '}', 3 }, { '>', 4 } };

int part_one()
{
    std::fstream input(FILE);

    uint32_t total_syntax_score = 0;

    std::string line;
    while (input >> line)
    {
        std::stack<char> symbols;

        for (char c : line)
        {
            if (std::find(std::begin(open), std::end(open), c) != std::end(open))
            {
                symbols.push(pair.at(c));
            }
            else
            {
                char top = symbols.top();
                symbols.pop();
                if (top != c)
                {
                    total_syntax_score += scores.at(c);
                    //printf("Expected %c, but found %c instead\n", top, c);
                    break;
                }
            }
        }
    }

    printf("%d\n", total_syntax_score);
    return total_syntax_score;
}

int part_two()
{
    std::fstream input(FILE);

    std::vector<uint64_t> scores;

    std::string line;
    while (input >> line)
    {
        std::stack<char> symbols;

        bool corrupted = false;
        for (char c : line)
        {
            if (std::find(std::begin(open), std::end(open), c) != std::end(open))
            {
                symbols.push(pair.at(c));
            }
            else
            {
                char top = symbols.top();
                symbols.pop();
                if (top != c)
                {
                    corrupted = true;
                    break;
                }
            }
        }

        if (corrupted)
        {
            continue;
        }

        uint64_t score = 0;
        while (!symbols.empty())
        {
            score = score * 5 + scores2.at(symbols.top());
            symbols.pop();
        }
        scores.push_back(score);
    }

    std::sort(scores.begin(), scores.end());

    uint64_t result = scores[(scores.size() - 1) / 2];
    
    printf("%d\n", result);
    return result;
}


int main()
{
    assert(part_one() == P1_EXPECT);
    assert(part_two() == P2_EXPECT);
    return 0;
}