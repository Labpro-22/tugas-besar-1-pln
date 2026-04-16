#include "include/models/player/Player.hpp"

Player::Player(const string& username, long long initialMoney, Board* board) :
    username(username), money(initialMoney), state(PlayerState::ACTIVE),
    properties(), streetPropertyCount(0), railroadPropertyCount(0),
    utilityPropertyCount(0), skillCards(), piece(board, 0),
    doubleRollCounter(0), getOutOfJailCardCount(0), jailTurns(0) {}
Player::Player( const string& username, long long money, PlayerState state, const vector<Property*>& properties,
                const vector<SkillCard*>& skillCards, int getOutOfJailCardCount, int jailTurns, int position, Board* board) : 
    username(username), money(money), state(state),
    properties(), streetPropertyCount(0), railroadPropertyCount(0), 
    utilityPropertyCount(0), skillCards(skillCards), piece(board, position),
    doubleRollCounter(0), getOutOfJailCardCount(getOutOfJailCardCount), jailTurns(jailTurns) 
    {
        for (Property* pr : properties) {
            addProperty(pr);
        }
    }

const string& Player::getUsername() const { return username; }
long long Player::getMoney() const { return money; }
PlayerState Player::getState() const { return state; }
PlayerPiece& Player::getPiece() { return piece; }
const vector<Property*>& Player::getProperties() const{ return properties; }
const vector<SkillCard*>& Player::getSkillCards() const{ return skillCards; }
int Player::getStreetPropertyCount() const { return streetPropertyCount; }
int Player::getRailroadPropertyCount() const { return railroadPropertyCount; }
int Player::getUtilityPropertyCount() const { return utilityPropertyCount; }

void Player::rollDiceAndMove() {
    if (state == PlayerState::BANKRUPT)
        throw AlreadyBankruptException("Pemain " + username + " sudah bangkrut.");
    if (state == PlayerState::JAILED)
        throw InJailException("Pemain " + username + " sedang di penjara.");

    lastRoll = DiceRoller::roll();

    int total = lastRoll.first + lastRoll.second;
    bool isDouble = (lastRoll.first == lastRoll.second);

    if (isDouble) {
        doubleRollCounter++;
        if (doubleRollCounter >= 3) {
            doubleRollCounter = 0;
            goToJail();
            return;
        }
    } else {
        doubleRollCounter = 0;
    }

    piece.goForward(total);

    return;
}
void Player::setDiceAndMove(int value1, int value2) {
    if (state == PlayerState::BANKRUPT)
        throw AlreadyBankruptException("Pemain " + username + " sudah bangkrut.");
    if (state == PlayerState::JAILED)
        throw InJailException("Pemain " + username + " sedang di penjara.");

    lastRoll = {value1, value2};
    int total = value1 + value2;
    bool isDouble = (value1 == value2);

    if (isDouble) {
        doubleRollCounter++;
        if (doubleRollCounter >= 3) {
            doubleRollCounter = 0;
            goToJail();
            return;
        }
    } else {
        doubleRollCounter = 0;
    }

    piece.goForward(total);

    return;
}

long long Player::calculateTotalWealth() const {
    long long total = money;

    for (Property* pr : properties) {
        total += pr->getPrice();

        StreetProperty* street = dynamic_cast<StreetProperty*>(pr);
        if (street) {
            int houses = street->getHouseCount();
            if (houses <= 4) {
                total += houses * street->getHousePrice();
            } else {
                total += 4 * street->getHousePrice() + street->getHotelPrice();
            }
        }
    }

    return total;
}
void Player::receiveMoney(long long amount) {
    money += amount;
}
void Player::giveMoney(Player* recipient, long long amount) {
    if (money < amount)
        throw InsufficientFundsException(
            "Pemain " + username + " tidak punya cukup uang (M" +
            std::to_string(money) + ") untuk memberi M" + std::to_string(amount));

    money -= amount;
    recipient->receiveMoney(amount);
}

void Player::payRent(Property* pr) {
    long long rent = pr->calculateRent();
    Player* owner = pr->getOwner();
    
    if (pr->getPropertyType == PropertyType::UTILITY){
        rent *= lastRoll;
    }

    if (money < rent) {
        throw InsufficientFundsException(
            "Pemain " + username + " tidak mampu membayar sewa M" +
            std::to_string(rent));
    }
    
    money -= rent;
    owner->receiveMoney(rent);
}

void Player::payTax(long long amount) {
    if (money < amount) {
        throw InsufficientFundsException(
            "Pemain " + username + " tidak mampu membayar pajak M" +
            std::to_string(amount));
    }
    money -= amount;
}

void Player::bankruptByBank() {
    if (state == PlayerState::BANKRUPT)
        throw AlreadyBankruptException("Pemain " + username + " sudah bangkrut.");

    money = 0;

    for (Property* pr : properties) {
        pr->resetOwnerAsBank();
    }
    properties.clear();
    streetPropertyCount = 0;
    railroadPropertyCount = 0;
    utilityPropertyCount = 0;

    skillCards.clear();

    state = PlayerState::BANKRUPT;
}
void Player::bankruptByPlayer(Player* creditor) {
    if (state == PlayerState::BANKRUPT)
        throw AlreadyBankruptException("Pemain " + username + " sudah bangkrut.");

    creditor->receiveMoney(money);
    money = 0;

    for (Property* pr : properties) {
        addProperty(pr);
    }

    properties.clear();
    streetPropertyCount = 0;
    railroadPropertyCount = 0;
    utilityPropertyCount = 0;

    skillCards.clear();

    state = PlayerState::BANKRUPT;
}
bool Player::isBankrupt() const {
    return state == PlayerState::BANKRUPT;
}

void Player::addProperty(Property* pr) {
    properties.push_back(pr);
    switch (pr->getPropertyType())
    {
        case PropertyType::STREET:
            streetPropertyCount++;
            break;
        case PropertyType::RAILROAD:
            railroadPropertyCount++;
            break;
        case PropertyType::UTILITY:
            utilityPropertyCount++;
            break;
        default:
            break;
    }
    pr->setOwner(this);
}

void Player::removeProperty(Property* pr) {
    auto it = std::find(properties.begin(), properties.end(), pr);
    if (it != properties.end()) {
        properties.erase(it);
    }
    switch (pr->getPropertyType())
    {
        case PropertyType::STREET:
            streetPropertyCount--;
            break;
        case PropertyType::RAILROAD:
            railroadPropertyCount--;
            break;
        case PropertyType::UTILITY:
            utilityPropertyCount--;
            break;
        default:
            break;
    }
}

void Player::buyProperty(Property* pr) {
    if (pr->getPropertyType() == PropertyType::STREET){
        long long price = pr->getPrice();
        if (money < price)
            throw InsufficientFundsException(
                "Uang tidak cukup untuk membeli properti seharga M" +
                std::to_string(price));
        money -= price;
    }
    addProperty(pr);
}

void Player::buyProperty(Property* pr, long long bid) {
    if (money < bid)
        throw InsufficientFundsException(
            "Uang tidak cukup untuk bid M" + std::to_string(bid));
    money -= bid;
    addProperty(pr);
}

void Player::sellProperty(Property* pr) {
    money += pr->calculateSellValue();
    pr->resetOwnerAsBank();
    removeProperty(pr);
}

void Player::mortgageProperty(Property* pr) {
    long long mortgageVal = pr->getMortgageValue();
    pr->setMortgaged(true);
    money += mortgageVal;
}

void Player::unmortgageProperty(Property* pr) {
    long long redeemPrice = pr->getPrice();
    if (money < redeemPrice)
        throw InsufficientFundsException(
            "Uang tidak cukup untuk menebus. Harga: M" +
            std::to_string(redeemPrice));
    
    money -= redeemPrice;
    pr->setMortgaged(false);
}
bool Player::isPropertySetComplete(const std::string& color, const Board& board) const {
    int ownedCount = 0;
    for (Property* pr : properties) {
        if (pr->getColor() == color) {
            ownedCount++;
        }
    }
    
    map<string,int> map = board.getMapTilesColorCount();
    int totalInGroup = map[color];

    return ownedCount == totalInGroup;
}
void Player::buildOnProperty(StreetProperty* pr) {
    if (pr->getHouseCount() == 4) {
        long long hotelPrice = pr->getHotelPrice();
        if (money < hotelPrice){
            throw InsufficientFundsException( "Uang tidak cukup untuk hotel. Harga: M" + std::to_string(hotelPrice));
            }
        money -= hotelPrice;
        pr->buildHotel();
    } else {
        long long housePrice = pr->getHousePrice();
        if (money < housePrice)
            throw InsufficientFundsException(
                "Uang tidak cukup untuk rumah. Harga: M" +
                std::to_string(housePrice));
        money -= housePrice;
        pr->buildHouse(1);
    }
}

void Player::sellBuilding(StreetProperty* pr) {
    int houses = pr->getHouseCount();
    long long refund;
    if (houses == 5) {
        refund = pr->getHotelPrice() / 2;
    } else {
        refund = pr->getHousePrice() / 2;
    }
    
    money += refund;
    pr->removeBuilding();
}

void Player::addSkillCard(SkillCard* card) {
    if (skillCards.size() >= 4) {
        throw FullHandException(
            "Pemain " + username + " sudah memiliki 4 kartu. Harus buang 1.");
    }
    skillCards.push_back(card);
}

void Player::useSkillCard(int index) {
    if (index < 0 || index >= static_cast<int>(skillCards.size()))
        throw InvalidCardIndexException(
            "Indeks kartu " + std::to_string(index) + " tidak valid.");

    SkillCard* card = skillCards[index];
    card->takeEffect(this);

    skillCards.erase(skillCards.begin() + index);
}

void Player::dropSkillCard(int index) {
    if (index < 0 || index >= static_cast<int>(skillCards.size()))
        throw InvalidCardIndexException(
            "Indeks kartu " + std::to_string(index) + " tidak valid.");

    skillCards.erase(skillCards.begin() + index);
}

bool Player::isJailed() const {
    return state == PlayerState::JAILED;
}


void Player::goToJail(){
    state = PlayerState::JAILED;
    jailTurns = 0;
    doubleRollCounter = 0;
    // piece.setPosition(board.getTilePosition("PEN")); 
}

void Player::useGetOutOfJailCard() {
    if (getOutOfJailCardCount <= 0)
        throw InJailException("Tidak memiliki kartu Bebas Penjara.");

    getOutOfJailCardCount--;
    getOutOfJail();
}
void Player::addGetOutOfJailCard() {
    getOutOfJailCardCount++;
}
void Player::rollToGetOutOfJail() {
    if (state != PlayerState::JAILED)
        throw InJailException("Pemain tidak sedang di penjara.");

    lastRoll = DiceRoller::roll();

    if (lastRoll.first == lastRoll.second) {
        getOutOfJail();
        piece.goForward(lastRoll.first + lastRoll.second);
    }

    return;
}
void Player::payFineToGetOutOfJail(long long fine) {
    if (state != PlayerState::JAILED)
        throw InJailException("Pemain tidak sedang di penjara.");

    if (money < fine)
        throw InsufficientFundsException(
            "Uang tidak cukup untuk bayar denda M" + std::to_string(fine));

    money -= fine;
    getOutOfJail();
}
void Player::getOutOfJail() {
    state = PlayerState::ACTIVE;
    jailTurns = 0;
}

void Player::onNextTurn() {
    for (Property* pr : properties) {
        pr->onNextTurn();
    }

    if (state == PlayerState::JAILED) {
        jailTurns++;
    }

    // counter efek kartu?
}