#include "../include/CardReader.hpp"
#include <iostream>

CardReader::CardReader() : isOperational(false), hasCard(false) {
}

CardReader::~CardReader() = default;

bool CardReader::insertCard(const std::string& cardNumber) {
    if (!isOperational) {
        lastError = "Card reader not operational";
        return false;
    }
    
    if (hasCard) {
        lastError = "Card already inserted";
        return false;
    }
    
    if (!validateCardFormat(cardNumber)) {
        lastError = "Invalid card format";
        return false;
    }
    
    currentCardNumber = cardNumber;
    cardData = extractCardData(cardNumber);
    hasCard = true;
    
    std::cout << "Card inserted: " << getMaskedCardNumber() << std::endl;
    return true;
}

bool CardReader::ejectCard() {
    if (!hasCard) {
        lastError = "No card to eject";
        return false;
    }
    
    std::cout << "Card ejected: " << getMaskedCardNumber() << std::endl;
    
    currentCardNumber.clear();
    cardData.clear();
    hasCard = false;
    clearError();
    
    return true;
}

bool CardReader::readCard() {
    if (!hasCard) {
        lastError = "No card to read";
        return false;
    }
    
    if (!validateCardData()) {
        lastError = "Invalid card data";
        return false;
    }
    
    return true;
}

bool CardReader::isCardPresent() const {
    return hasCard;
}

std::string CardReader::getCardNumber() const {
    return currentCardNumber;
}

std::string CardReader::getCardData() const {
    return cardData;
}

bool CardReader::validateCardData() const {
    return !cardData.empty() && cardData.length() >= 10;
}

bool CardReader::isReaderOperational() const {
    return isOperational;
}

void CardReader::setOperationalStatus(bool status) {
    isOperational = status;
}

std::string CardReader::getLastError() const {
    return lastError;
}

void CardReader::clearError() {
    lastError.clear();
}

bool CardReader::validateCardFormat(const std::string& cardNumber) const {
    // Basic card number validation
    if (cardNumber.empty() || cardNumber.length() < 13 || cardNumber.length() > 19) {
        return false;
    }
    
    // Check if all characters are digits
    for (char c : cardNumber) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    
    return true;
}

std::string CardReader::extractCardData(const std::string& cardNumber) const {
    // In a real system, this would read magnetic stripe or chip data
    // For demo purposes, we'll just return the card number
    return cardNumber;
}

std::string CardReader::getMaskedCardNumber() const {
    if (currentCardNumber.length() < 4) {
        return currentCardNumber;
    }
    
    std::string masked = currentCardNumber;
    for (size_t i = 4; i < currentCardNumber.length() - 4; ++i) {
        masked[i] = '*';
    }
    return masked;
} 