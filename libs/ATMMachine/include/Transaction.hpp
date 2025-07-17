#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
#include <chrono>

enum class TransactionType {
    WITHDRAWAL,
    DEPOSIT,
    TRANSFER,
    BALANCE_INQUIRY,
    PIN_CHANGE,
    MINI_STATEMENT
};

enum class TransactionStatus {
    PENDING,
    COMPLETED,
    FAILED,
    CANCELLED
};

class Transaction {
private:
    std::string transactionId;
    TransactionType type;
    TransactionStatus status;
    std::string accountNumber;
    std::string cardNumber;
    double amount;
    std::string description;
    std::chrono::system_clock::time_point timestamp;
    std::string location; // ATM location
    std::string referenceNumber;
    
public:
    Transaction(TransactionType type, const std::string& account, 
                const std::string& card, double amount = 0.0);
    ~Transaction();
    
    // Getters
    std::string getTransactionId() const;
    TransactionType getType() const;
    TransactionStatus getStatus() const;
    std::string getAccountNumber() const;
    std::string getCardNumber() const;
    double getAmount() const;
    std::string getDescription() const;
    std::chrono::system_clock::time_point getTimestamp() const;
    std::string getLocation() const;
    std::string getReferenceNumber() const;
    
    // Transaction operations
    void setStatus(TransactionStatus status);
    void setDescription(const std::string& description);
    void setReferenceNumber(const std::string& reference);
    void setLocation(const std::string& location);
    
    // Utility methods
    std::string getTransactionTypeString() const;
    std::string getStatusString() const;
    std::string getFormattedTimestamp() const;
    std::string getTransactionSummary() const;
    bool isSuccessful() const;
    
private:
    std::string generateTransactionId();
    std::string generateReferenceNumber();
};

#endif // TRANSACTION_HPP 