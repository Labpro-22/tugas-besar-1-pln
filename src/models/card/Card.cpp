#include "include/models/card/Card.hpp"

Card::Card(const std::string& message) : message(message) {}

std::string Card::getMessage() {return message;}