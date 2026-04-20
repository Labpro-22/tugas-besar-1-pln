#pragma once
#include "include/models/card/Card.hpp"
#include <vector>

template <typename T>
class CardDeck {
private:
    std::vector<T> cards;
    std::vector<T> drawnCards;

public:
    CardDeck() = default;
    ~CardDeck() = default;

    T drawCard();
    void addCard(T card);
    void clearDeck();
    void reshuffle();
    size_t getSize() const;
    std::vector<T> getCards() const;
    T removeCardAt(int index);
};

#include "CardDeck.cpp"