#ifndef CARD_READER_HPP
#define CARD_READER_HPP

#include <string>

class CardReader {
private:
    bool isOperational;
    bool hasCard;
    std::string currentCardNumber;
    std::string cardData;
    
public:
    CardReader();
    ~CardReader();
    
    // Card operations
    bool insertCard(const std::string& cardNumber);
    bool ejectCard();
    bool readCard();
    bool isCardPresent() const;
    
    // Data operations
    std::string getCardNumber() const;
    std::string getCardData() const;
    bool validateCardData() const;
    
    // Status operations
    bool isReaderOperational() const;
    void setOperationalStatus(bool status);
    
    // Error handling
    std::string getLastError() const;
    void clearError();
    
private:
    std::string lastError;
    std::string getMaskedCardNumber() const;
    bool validateCardFormat(const std::string& cardNumber) const;
    std::string extractCardData(const std::string& cardNumber) const;
};

#endif // CARD_READER_HPP 