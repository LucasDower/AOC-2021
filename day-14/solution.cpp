#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <set>

#define USE_EXAMPLE 0
#if USE_EXAMPLE
	#define FILE "example.txt"
    #define P1_EXPECT 1588
    #define P2_EXPECT 2188189693529
#else
	#define FILE "input.txt"
    #define P1_EXPECT 3342
    #define P2_EXPECT 3776553567525
#endif

int get_hash(char left, char right)
{
    return left * 10000 + right;
}

struct t_pair
{
    char left;
    char right;

    bool operator==(const t_pair& other) const
    {
        return left == other.left && right == other.right;
    }
};

namespace std {

    template <>
    struct hash<t_pair>
    {
        std::size_t operator()(const t_pair& pair) const
        {
            return pair.right * 1000 + pair.left;
        }
    };

}

std::vector<t_pair> get_all_symbol_pairs(const std::set<char>& symbols)
{
    std::vector<t_pair> all_pairs;
    for (const char left : symbols)
    {
        for (const char right : symbols)
        {
            all_pairs.push_back({ left, right });
        }
    }
    return all_pairs;
}

std::unordered_map<char, uint64_t> count_symbols_in_sequence(const std::set<char>& symbols, const std::string& sequence)
{
    std::unordered_map<char, uint64_t> symbol_counts;
    for (char symbol : symbols)
    {
        if (!symbol_counts.contains(symbol)) // alternatively, (symbol_counts.count(symbol) > 0) for older C++
        {
            symbol_counts.insert({ symbol, 0 });
        }
    }
    for (char symbol : sequence)
    {
        ++symbol_counts.at(symbol);
    }
    return symbol_counts;
}

void parse_input(std::set<char>& symbols, std::string& sequence, std::unordered_map<t_pair, char>& rules)
{
    std::string line;
    std::ifstream file(FILE);
    file >> sequence;

    while (std::getline(file, line))
    {
        char left; char right;  char insert;
        if (!line.empty() && sscanf(line.c_str(), "%c%c -> %c", &left, &right, &insert))
        {
            symbols.insert({ left, right, insert });
            t_pair pair({ left, right });
            rules.insert({ pair, insert });
        }
    }
}

std::unordered_map<t_pair, unsigned long long> count_pairs_in_sequence(const std::vector<t_pair>& all_symbol_pairs, const std::string& sequence)
{
    std::unordered_map<t_pair, unsigned long long> pair_counts;
    for (t_pair pair : all_symbol_pairs)
    {
        pair_counts.insert({ pair, 0 });
    }

    for (size_t i = 0; i < sequence.size() - 1; ++i)
    {
        t_pair pair({ sequence[i], sequence[i + 1] });
        ++pair_counts.at(pair);
    }
    return pair_counts;
}

unsigned long long dump_length(const std::set<char>& symbols, const std::unordered_map<char, uint64_t>& symbol_counts)
{
    unsigned long long length = 0;
    for (const char s : symbols)
    {
        length += symbol_counts.at(s);
    }
    return length;
}

unsigned long long dump_pair_counts(const std::vector<t_pair>& all_symbol_pairs, const std::unordered_map<t_pair, unsigned long long>& pair_counts)
{
    unsigned long long count = 0;
    for (const t_pair pair : all_symbol_pairs)
    {
        count += pair_counts.at(pair);
    }
    return count;
}

std::unordered_map<t_pair, unsigned long long> get_blank_counts(const std::vector<t_pair>& all_symbol_pairs)
{
    std::unordered_map<t_pair, unsigned long long> counts;
    for (const t_pair pair : all_symbol_pairs)
    {
        counts.insert({ pair, 0 });
    }
    return counts;
}

unsigned long long run_step(const std::vector<t_pair>& all_symbol_pairs, std::unordered_map<t_pair, unsigned long long>& pair_counts, const std::unordered_map<t_pair, char>& rules, const std::set<char>& symbols, std::unordered_map<char, uint64_t>& symbol_counts)
{
    std::unordered_map<t_pair, unsigned long long> new_pair_counts = get_blank_counts(all_symbol_pairs);

    for (const t_pair& pair : all_symbol_pairs)
    {
        unsigned long long count = pair_counts.at(pair);
        if (count > 0)
        {
            char insert = rules.at(pair);
            //printf("Expanding %llux %c%c into %c%c and %c%c\n", count, pair.left, pair.right, pair.left, insert, insert, pair.right);
            new_pair_counts.at({ pair.left, insert }) += count;
            new_pair_counts.at({ insert, pair.right }) += count;
            symbol_counts.at(insert) += count;
        }
    }

    pair_counts = new_pair_counts;
    unsigned long long length = dump_length(symbols, symbol_counts);
    unsigned long long count = dump_pair_counts(all_symbol_pairs, pair_counts);
    //printf("Length: %llu, Pair count: %llu\n\n", length, count);
    assert(length == count + 1);

    struct t_symbol_count { char symbol; unsigned long long count; };
    std::vector<t_symbol_count> symbol_counts_vec;
    for (char s : symbols)
    {
        symbol_counts_vec.push_back({ s, symbol_counts.at(s) });
    }
    std::sort(symbol_counts_vec.begin(), symbol_counts_vec.end(), [](const t_symbol_count& left, const t_symbol_count& right) { return left.count < right.count; });

    unsigned long long count_diff = symbol_counts_vec[symbols.size() - 1].count - symbol_counts_vec[0].count;

    return count_diff;
}

int main()
{
    std::set<char> symbols;
    std::string sequence;
    std::unordered_map<t_pair, char> rules;
    parse_input(symbols, sequence, rules);

    std::vector<t_pair> all_symbol_pairs = get_all_symbol_pairs(symbols);
    std::unordered_map<char, uint64_t> symbol_counts = count_symbols_in_sequence(symbols, sequence);
    std::unordered_map<t_pair, unsigned long long> pair_counts = count_pairs_in_sequence(all_symbol_pairs, sequence);

    // Part 1
    unsigned long long diff = 0;
    for (int i = 0; i < 10; ++i)
    {
        diff = run_step(all_symbol_pairs, pair_counts, rules, symbols, symbol_counts);
    }
    printf("%llu\n", diff);
    assert(diff == P1_EXPECT);

    // Part 2
    for (int i = 10; i < 40; ++i)
    {
        diff = run_step(all_symbol_pairs, pair_counts, rules, symbols, symbol_counts);
    }
    printf("%llu\n", diff);
    assert(diff == P2_EXPECT);

	return 0;
}