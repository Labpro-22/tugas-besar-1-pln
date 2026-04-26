#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

Player::Player(const std::string& username, long long initialMoney) :
    username(username), money(initialMoney), state(PlayerState::ACTIVE),
    properties(), streetPropertyCount(0), railroadPropertyCount(0),
    utilityPropertyCount(0), skillCards(), piece(0),
    doubleRollCounter(0), getOutOfJailCardCount(0), jailTurns(0) {}
Player::Player( const std::string& username, long long money, PlayerState state,
                const std::vector<SkillCard*>& skillCards, int getOutOfJailCardCount, int jailTurns) : 
    username(username), money(money), state(state),
    properties(), streetPropertyCount(0), railroadPropertyCount(0), 
    utilityPropertyCount(0), skillCards(skillCards), piece(0),
    doubleRollCounter(0), getOutOfJailCardCount(getOutOfJailCardCount), jailTurns(jailTurns) 
    { }

const std::string& Player::getUsername() const { return username; }
long long Player::getMoney() const { return money; }
PlayerState Player::getState() const { return state; }
PlayerPiece& Player::getPiece() { return piece; }
const std::vector<Property*>& Player::getProperties() const{ return properties; }
const std::vector<SkillCard*>& Player::getSkillCards() const{ return skillCards; }
std::vector<SkillCard*>& Player::getSkillCardsRef() { return skillCards; }
const std::vector<PlayerEffect>& Player::getEffects() const{ return effects; }
int Player::getStreetPropertyCount() const { return streetPropertyCount; }
int Player::getRailroadPropertyCount() const { return railroadPropertyCount; }
int Player::getUtilityPropertyCount() const { return utilityPropertyCount; }

void Player::rollDiceAndMove(GameManager& gm) {
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

    piece.goForward(total, *this, gm); 

    return;
}
void Player::setDiceAndMove(int value1, int value2, GameManager& gm) {
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

    piece.goForward(total, *this, gm); 

    return;
}

long long Player::calculateTotalWealth() const {
    long long total = money;

    for (Property* pr : properties) {
        total += pr->calculateAssetValue();
    }

    return total;
}
void Player::receiveMoney(long long amount) {
    money += amount;
}
bool Player::giveMoney(Player& recipient, long long amount) {
    if (hasEffect("SHIELD")) return true;
    money -= amount;
    recipient.receiveMoney(amount);
    if (money < 0) return false;

    return true;
}

bool Player::payRent(Property* pr) {
    if (hasEffect("SHIELD")) return true;

    long long rent = pr->calculateRent();
    Player* owner = pr->getOwner();
    
    if (pr->getPropertyType() == "UTILITY"){
        rent *= (lastRoll.first + lastRoll.second);
    }

    if (pr->getPropertyType() == "STREET") {
        StreetProperty* sp = dynamic_cast<StreetProperty*>(pr);
        if (sp && sp->getHouseCount() == 0 && sp->getFestivalDuration() == 0) {
            // Count how many of the same color the owner has vs total in board
            // Use owner's property list to check monopoly
            int ownedCount = 0;
            int totalInColor = 0;
            for (Property* p : owner->getProperties()) {
                if (p->getColor() == sp->getColor()) ownedCount++;
            }
            // Get total from the board via owner's knowledge - we stored color count per player property check
            // We approximate: if ownedCount >= 2 for brown/dark blue, or >= 3 for others
            // Actually use the same isPropertySetComplete logic but we don't have board here.
            // We track: if ALL properties in color group owned by same player.
            // Simple approximation: count total same-color properties owner has.
            // For color groups: brown=2, dark_blue=2, others=3
            bool isMonopoly = false;
            std::string color = sp->getColor();
            if ((color == "COKLAT" || color == "BIRU_TUA") && ownedCount >= 2) {
                isMonopoly = true;
            } else if (ownedCount >= 3) {
                isMonopoly = true;
            }
            if (isMonopoly) {
                rent *= 2;
            }
        }
    }

    if (hasEffect("DISCOUNT")) {
        rent = rent * (100 - getEffectValue("DISCOUNT")) / 100;
    }
    
    money -= rent;
    if (money < 0) {
        return false;
    }
    
    owner->receiveMoney(rent);
    return true;
}

bool Player::payTax(long long amount) {
    if (hasEffect("SHIELD")) return true;
    if (hasEffect("DISCOUNT")) {
        amount = amount * (100 - getEffectValue("DISCOUNT")) / 100;
    }
    
    money -= amount;

    if (money < 0) {
        return false;
    }
    return true;
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
        creditor->addProperty(pr);
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
    if (pr->getPropertyType() == "STREET") {
        streetPropertyCount++;
    } else if (pr->getPropertyType() == "RAILROAD") {
        railroadPropertyCount++;
    } else if (pr->getPropertyType() == "UTILITY") {
        utilityPropertyCount++;
    }
    pr->setOwner(this);
}

void Player::removeProperty(Property* pr) {
    auto it = std::find(properties.begin(), properties.end(), pr);
    if (it != properties.end()) {
        properties.erase(it);
    }
    if (pr->getPropertyType() == "STREET") {
        streetPropertyCount--;
    } else if (pr->getPropertyType() == "RAILROAD") {
        railroadPropertyCount--;
    } else if (pr->getPropertyType() == "UTILITY") {
        utilityPropertyCount--;
    }
}

bool Player::buyProperty(Property* pr) {
    if (pr->getPropertyType() == "STREET"){
        long long price = pr->getPrice();
        if (hasEffect("DISCOUNT")) {
            price = price * (100 - getEffectValue("DISCOUNT")) / 100;
        }
        if (money < price) return false;
        money -= price;
    }
    addProperty(pr);
    return true;
}

void Player::buyProperty(Property* pr, long long bid) {
    if (hasEffect("DISCOUNT")) {
        bid = bid * (100 - getEffectValue("DISCOUNT")) / 100;
    }
    if (money < bid)
        throw InsufficientFundsException(
            "Uang tidak cukup untuk bid M" + std::to_string(bid));
    money -= bid;
    addProperty(pr);
}

void Player::sellProperty(Property* pr) {
    money += pr->calculateSellValue();
    pr->sellProperty();
    removeProperty(pr);
}

void Player::mortgageProperty(Property* pr) {
    long long mortgageVal = pr->getMortgageValue();
    pr->setMortgaged(true);
    money += mortgageVal;
}

void Player::unmortgageProperty(Property* pr) {
    long long redeemPrice = pr->redemptionPrice();
    if (hasEffect("DISCOUNT")) {
        redeemPrice = redeemPrice * (100 - getEffectValue("DISCOUNT")) / 100;
    }
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
    
    std::map<std::string,int> map = board.getMapTilesColorCount();
    int totalInGroup = map[color];

    return ownedCount == totalInGroup;
}
void Player::buildOnProperty(StreetProperty* pr) {
    if (pr->getHouseCount() == 4) {
        long long hotelPrice = pr->getHotelPrice();
        if (hasEffect("DISCOUNT")) {
            hotelPrice = hotelPrice * (100 - getEffectValue("DISCOUNT")) / 100;
        }
        if (money < hotelPrice){
            throw InsufficientFundsException( "Uang tidak cukup untuk hotel. Harga: M" + std::to_string(hotelPrice));
            }
        money -= hotelPrice;
        pr->buildHotel();
    } else {
        long long housePrice = pr->getHousePrice();
        if (hasEffect("DISCOUNT")) {
            housePrice = housePrice * (100 - getEffectValue("DISCOUNT")) / 100;
        }
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
    if (skillCards.size() >= 3) {
        throw FullHandException(
            "Pemain " + username + " sudah memiliki 3 kartu. Harus buang 1.");
    }
    skillCards.push_back(card);
}

void Player::useSkillCard(int index, GameManager& gm) {
    if (index < 0 || index >= static_cast<int>(skillCards.size()))
        throw InvalidCardIndexException(
            "Indeks kartu " + std::to_string(index) + " tidak valid.");

    SkillCard* card = skillCards[index];
    card->takeEffect(*this, gm);

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
    if (hasEffect("SHIELD")) return;
    state = PlayerState::JAILED;
    jailTurns = 0;
    doubleRollCounter = 0;
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
    }

    return;
}

void Player::setDiceToGetOutOfJail(int value1, int value2) {
    if (state != PlayerState::JAILED)
        throw InJailException("Pemain tidak sedang di penjara.");

    lastRoll = {value1, value2};

    if (lastRoll.first == lastRoll.second) {
        getOutOfJail();
    }
}

void Player::payFineToGetOutOfJail(long long fine) {
    if (state != PlayerState::JAILED)
        throw InJailException("Pemain tidak sedang di penjara.");

    if (hasEffect("DISCOUNT")) {
        fine = fine * (100 - getEffectValue("DISCOUNT")) / 100;
    }

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

int Player::getJailTurns() {
    return jailTurns;
}

void Player::addEffect(PlayerEffect effect) {
    effects.push_back(effect);
}

bool Player::hasEffect(const std::string& name) const {
    for (const PlayerEffect& e : effects) {
        if (e.getName() == name && !e.isExpired()) return true;
    }
    return false;
}

int Player::getEffectValue(const std::string& name) const {
    for (const PlayerEffect& e : effects) {
        if (e.getName() == name && !e.isExpired()) return e.getValue();
    }
    return 0;
}

void Player::onNextTurn() {
    for (Property* pr : properties) {
        pr->onNextTurn();
    }

    if (state == PlayerState::JAILED) {
        jailTurns++;
    }

    for (PlayerEffect& e : effects) {
        e.decrementDuration();
    }
    effects.erase(
        std::remove_if(effects.begin(), effects.end(),
            [](const PlayerEffect& e) { return e.isExpired(); }),
        effects.end()
    );
}

int Player::getGetOutOfJailCardCount() const {
    return getOutOfJailCardCount;
}

void Player::setMoney(long long amount) {
    money = amount;
}