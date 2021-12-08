#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <set>
#include <algorithm>
#include <unordered_map>

#define USE_EXAMPLE 0
#if USE_EXAMPLE
    #define FILE "./example.txt"
    #define P1_EXPECT 26
    #define P2_EXPECT 61229
#else
    #define FILE "./input.txt"
    #define P1_EXPECT 440
    #define P2_EXPECT 1046281
#endif

struct readout {
    std::string input[10];
    std::string output[4];
};

std::vector<readout> parse_file()
{
    std::ifstream input(FILE);
    char ignore;

    std::vector<readout> readouts;
    while (!input.eof())
    {
        readout ro;
        for (int i = 0; i < 10; ++i)
        {
            input >> ro.input[i];
        }
        input >> ignore;
        for (int i = 0; i < 4; ++i)
        {
            input >> ro.output[i];
        }
        readouts.push_back(ro);
    }

    return readouts;
}

int part_one()
{
    std::vector<readout> readouts = parse_file();
    std::vector<int> lengths = { 2 /*1*/, 3 /*7*/, 4 /*4*/, 7 /*8*/};

    int count = 0;
    for (readout& ro : readouts)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (std::find(lengths.begin(), lengths.end(), ro.output[i].length()) != lengths.end())
            {
                ++count;
            }
        }
    }

    printf("%d\n", count);
    return count;
}

std::string find_segment_of_length(readout ro, int length)
{
    assert(length == 2 || length == 3 || length == 4 || length == 7);
    for (int i = 0; i < 10; ++i)
    {
        if (ro.input[i].length() == length)
        {
            return ro.input[i];
        }
    }
    assert(false);
}

char get_unique_char(std::string a, std::string b)
{
    for (int i = 0; i < a.length(); ++i)
    {
        if (b.find(a[i]) == std::string::npos)
        {
            return a[i];
        }
    }
    for (int i = 0; i < b.length(); ++i)
    {
        if (a.find(b[i]) == std::string::npos)
        {
            return b[i];
        }
    }
    assert(false);
}

std::string find_common_chars(std::vector<std::string> strs)
{
    bool prim[7];
    memset(prim, true, sizeof(prim));
    for (int i = 0; i < strs.size(); ++i) {
        bool sec[7] = { false };

        for (int j = 0; strs[i][j]; j++) {
            if (prim[strs[i][j] - 'a'])
                sec[strs[i][j] - 'a'] = true;
        }
        memcpy(prim, sec, 7);
    }

    std::string common = "";
    for (int i = 0; i < 7; ++i)
    {
        if (prim[i])
        {
            common += 'a' + i;
        }
    }
    return common;
}

std::string char_difference(std::string left, std::string right)
{
    std::string difference = "";
    for (char c : left)
    {
        if (right.find(c) == std::string::npos)
        {
            difference += c;
        }
    }
    return difference;
}

std::vector<std::string> find_segments_of_length(readout ro, int length)
{
    std::vector<std::string> segments;
    for (int i = 0; i < 10; ++i)
    {
        if (ro.input[i].length() == length)
        {
            segments.push_back(ro.input[i]);
        }
    }
    return segments;
}

int part_two()
{
    std::vector<readout> readouts = parse_file();

    const std::unordered_map<int, int> number_map {
        { 0b1111110, 0 }, { 0b0110000, 1 }, { 0b1101101, 2 }, { 0b1111001, 3 }, { 0b0110011, 4 },
        { 0b1011011, 5 }, { 0b1011111, 6 }, { 0b1110000, 7 }, { 0b1111111, 8 }, { 0b1111011, 9 }
    };

    unsigned int sum = 0;

    for (readout& ro : readouts)
    {
        // Determine top segment
        std::string digit_one = find_segment_of_length(ro, 2);
        std::string digit_seven = find_segment_of_length(ro, 3);
        char top_segment_wire = get_unique_char(digit_one, digit_seven);

        std::string digit_four = find_segment_of_length(ro, 4);
        std::string l_shape = char_difference(digit_four, digit_one);

        // Determine bottom segment
        std::vector<std::string> candidates = find_segments_of_length(ro, 5);
        assert(candidates.size() == 3);
        std::string common = find_common_chars(candidates);
        common.erase(std::remove(common.begin(), common.end(), top_segment_wire), common.end());

        std::string tmp = char_difference(common, l_shape);
        assert(tmp.length() == 1);
        char bottom_segment_wire = tmp[0];

        // Determine bottom-left segment
        std::string digit_eight = find_segment_of_length(ro, 7);
        std::string backwards_three = char_difference(digit_eight, digit_one);
        std::string lowercase_t = char_difference(char_difference(backwards_three, std::string(1,top_segment_wire)), std::string(1,bottom_segment_wire));
        tmp = char_difference(lowercase_t, digit_four);

        assert(tmp.length() == 1);
        char bottom_left_segment_wire = tmp[0];

        // Determine top-left segment
        candidates = find_segments_of_length(ro, 6);
        assert(candidates.size() == 3);
        std::string broken_s = find_common_chars(candidates);

        tmp = char_difference(broken_s, std::string(1, top_segment_wire));
        std::string super_broken_s = char_difference(tmp, std::string(1, bottom_segment_wire));
        
        tmp = char_difference(super_broken_s, digit_one);
        assert(tmp.length() == 1);
        char top_left_segment_wire = tmp[0];

        // Determine middle segment
        tmp = char_difference(l_shape, std::string(1, top_left_segment_wire));
        assert(tmp.length() == 1);
        char middle_segment_wire = tmp[0];

        // Determine bottom-right segment
        tmp = char_difference(super_broken_s, std::string(1, top_left_segment_wire));
        assert(tmp.length() == 1);
        char bottom_right_segment_wire = tmp[0];

        // Determine top-right segment
        tmp = char_difference(digit_one, std::string(1, bottom_right_segment_wire));
        assert(tmp.length() == 1);
        char top_right_segment_wire = tmp[0];

        char segments[7] = {
            top_segment_wire,
            top_right_segment_wire,
            bottom_right_segment_wire,
            bottom_segment_wire,
            bottom_left_segment_wire,
            top_left_segment_wire,
            middle_segment_wire
        };

        int value = 0;

        for (int i = 0; i < 4; ++i)
        {
            std::string cipher = ro.output[i];

            int encoding = 0;
            for (int j = 0; j < 7; ++j)
            {
                if (cipher.find(segments[j]) != std::string::npos)
                {
                    encoding += (int)std::pow(2, 6 - j);
                }
            }

            int number = number_map.at(encoding);
            value += number * (int)std::pow(10, 3 - i);
        }

        sum += value;
    }
    
    printf("%d\n", sum);
    return sum;
}

int main()
{
    assert(part_one() == P1_EXPECT);
    assert(part_two() == P2_EXPECT);
    return 0;
}