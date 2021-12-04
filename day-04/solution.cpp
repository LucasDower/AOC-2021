#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

struct Entry {
    int number;
    bool checked;
};

typedef std::vector<Entry> Card;

std::vector<int> parse_draw_numbers(std::ifstream& input)
{
    std::vector<int> draw_numbers;
    {
        std::string draw_order;
        input >> draw_order;
        std::replace(draw_order.begin(), draw_order.end(), ',', ' ');
        std::stringstream iss(draw_order);
        int number = 0;
        while (iss >> number)
        {
            draw_numbers.push_back(number);
        }
    }
    return draw_numbers;
}

std::vector<Card> parse_bingo_cards(std::ifstream& input)
{
    std::vector<Card> cards;
    {
        int number;
        Card card;
        while (input >> number)
        {
            card.push_back({ number, false });
            if (card.size() == 25)
            {
                cards.push_back(card);
                card = Card();
            }
        }
    }
    return cards;
}

bool card_bingo(const Card& card)
{
    for (int i = 0; i < 5; ++i)
    {
        bool checked_row = true;
        bool checked_col = true;
        for (int j = 0; j < 5; ++j)
        {
            checked_row &= card[i * 5 + j].checked;
            checked_col &= card[j * 5 + i].checked;
        }
        if (checked_row || checked_col)
        {
            return true;
        }
    }
    return false;
}

int get_unmarked_sum(Card& card)
{
    int unmarked_sum = 0;
    for (int i = 0; i < 25; ++i)
    {
        if (!card[i].checked)
        {
            unmarked_sum += card[i].number;
        }
    }
    return unmarked_sum;
}

void mark_number(Card& card, int number)
{
    for (int i = 0; i < 25; ++i)
    {
        if (card[i].number == number)
        {
            card[i].checked = true;
            return;
        }
    }
}


void part_one()
{
    std::ifstream input("./input.txt");

    std::vector<int> draw_numbers = parse_draw_numbers(input);
    std::vector<Card> cards = parse_bingo_cards(input);

    // find winner
    Card winning_card;
    int last_drawn = 0;
    [&]
    {
        for (int draw : draw_numbers)
        {
            for (Card& card : cards)
            {
                mark_number(card, draw);
                if (card_bingo(card))
                {
                    winning_card = card;
                    last_drawn = draw;
                    return;
                }
            }
        }
    }();
    
    printf("%d\n", get_unmarked_sum(winning_card) * last_drawn);
}

void part_two()
{
    std::ifstream input("./input.txt");

    std::vector<int> draw_numbers = parse_draw_numbers(input);
    std::vector<Card> cards = parse_bingo_cards(input);

    int cards_won = 0;
    for (int draw : draw_numbers)
    {
        std::vector<Card> non_winners;
        for (Card& card : cards)
        {
            mark_number(card, draw);
            if (!card_bingo(card))
            {
                non_winners.push_back(card);
            }
            else
            {
                if (cards.size() == 1)
                {
                    printf("%d\n", get_unmarked_sum(card) * draw);
                }
            }
        }
        cards = non_winners;
    }
}

int main()
{
    part_one();
    part_two();
    return 0;
}