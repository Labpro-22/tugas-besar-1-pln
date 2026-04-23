#pragma once
#include "models/card/Card.hpp"
#include "models/card/CardDeckException.hpp"
#include <algorithm>
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

template <typename T>
T CardDeck<T>::drawCard()
{
    if (cards.empty()) {
        if (drawnCards.empty()) {
            throw CardDeckException("Deck kosong dan tidak ada kartu untuk di-reshuffle.");
        }
        reshuffle();
    }

    T drawn = cards.back();
    cards.pop_back();
    drawnCards.push_back(drawn);

    return drawn;
}

template <typename T>
void CardDeck<T>::addCard(T card)
{
    cards.push_back(card);
}

template <typename T>
void CardDeck<T>::clearDeck()
{
    cards.clear();
    drawnCards.clear();
}

template <typename T>
void CardDeck<T>::reshuffle()
{
    if (drawnCards.empty()) return;

    cards.insert(cards.end(), drawnCards.begin(), drawnCards.end());
    drawnCards.clear();

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

template <typename T>
size_t CardDeck<T>::getSize() const
{
    return cards.size();
}

template <typename T>
std::vector<T> CardDeck<T>::getCards() const
{
    return cards;
}

template <typename T>
T CardDeck<T>::removeCardAt(int index)
{
    if (index < 0 || index >= static_cast<int>(cards.size())) {
        throw CardDeckException("Indeks kartu tidak valid.");
    }
    T removed = cards[index];
    cards.erase(cards.begin() + index);
    return removed;
}