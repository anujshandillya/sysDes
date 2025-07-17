#ifndef CARD_HPP
#define CARD_HPP

#include <string>
#include <chrono>

class Card {
private:
    std::string cardNumber;
    std::string cardHolderName;
    std::string bankName;
    std::string cardType; // Debit, Credit, etc.
    std::string pinHash;
    std::chrono::system_clock::time_point expiryDate;
    bool isBlocked;
    bool isActive;
    
public:
    Card(const std::string& number, const std::string& holder, 
         const std::string& bank, const std::string& type);
    ~Card();
    
    // Getters
    std::string getCardNumber() const;
    std::string getCardHolderName() const;
    std::string getBankName() const;
    std::string getCardType() const;
    bool isExpired() const;
    bool isCardBlocked() const;
    bool isCardActive() const;
    
    // Card operations
    bool validateCard();
    bool blockCard();
    bool unblockCard();
    bool updatePIN(const std::string& newPin);
    bool verifyPIN(const std::string& pin) const;
    
    // Utility methods
    std::string getMaskedCardNumber() const;
    std::string getCardInfo() const;
};

#endif // CARD_HPP 