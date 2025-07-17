#include "../include/Card.hpp"
#include <sstream>
#include <chrono>

Card::Card(const std::string& number, const std::string& holder, 
           const std::string& bank, const std::string& type)
    : cardNumber(number), cardHolderName(holder), bankName(bank), 
      cardType(type), isBlocked(false), isActive(true) {
    
    // Set expiry date to 5 years from now
    auto now = std::chrono::system_clock::now();
    expiryDate = now + std::chrono::hours(24 * 365 * 5);
    
    // For demo purposes, set a default PIN hash
    pinHash = "1234"; // In real system, this would be hashed
}

Card::~Card() = default;

std::string Card::getCardNumber() const {
    return cardNumber;
}

std::string Card::getCardHolderName() const {
    return cardHolderName;
}

std::string Card::getBankName() const {
    return bankName;
}

std::string Card::getCardType() const {
    return cardType;
}

bool Card::isExpired() const {
    auto now = std::chrono::system_clock::now();
    return now > expiryDate;
}

bool Card::isCardBlocked() const {
    return isBlocked;
}

bool Card::isCardActive() const {
    return isActive && !isBlocked && !isExpired();
}

bool Card::validateCard() {
    if (cardNumber.empty() || cardNumber.length() < 13 || cardNumber.length() > 19) {
        return false;
    }
    
    if (isExpired()) {
        return false;
    }
    
    if (isBlocked) {
        return false;
    }
    
    if (!isActive) {
        return false;
    }
    
    return true;
}

bool Card::blockCard() {
    isBlocked = true;
    return true;
}

bool Card::unblockCard() {
    isBlocked = false;
    return true;
}

bool Card::updatePIN(const std::string& newPin) {
    if (newPin.length() != 4) {
        return false;
    }
    
    // In a real system, this would hash the PIN
    pinHash = newPin;
    return true;
}

bool Card::verifyPIN(const std::string& pin) const {
    return pin == pinHash;
}

std::string Card::getMaskedCardNumber() const {
    if (cardNumber.length() < 4) {
        return cardNumber;
    }
    
    std::string masked = cardNumber;
    for (size_t i = 4; i < cardNumber.length() - 4; ++i) {
        masked[i] = '*';
    }
    return masked;
}

std::string Card::getCardInfo() const {
    std::stringstream ss;
    ss << "Card Number: " << getMaskedCardNumber() << "\n";
    ss << "Card Holder: " << cardHolderName << "\n";
    ss << "Bank: " << bankName << "\n";
    ss << "Type: " << cardType << "\n";
    ss << "Status: " << (isCardActive() ? "Active" : "Inactive") << "\n";
    ss << "Expired: " << (isExpired() ? "Yes" : "No") << "\n";
    ss << "Blocked: " << (isBlocked ? "Yes" : "No");
    
    return ss.str();
} 