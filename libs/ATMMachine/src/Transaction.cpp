#include "../include/Transaction.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>

Transaction::Transaction(TransactionType type, const std::string& account, 
                         const std::string& card, double amount)
    : type(type), status(TransactionStatus::PENDING), accountNumber(account),
      cardNumber(card), amount(amount) {
    
    transactionId = generateTransactionId();
    referenceNumber = generateReferenceNumber();
    timestamp = std::chrono::system_clock::now();
    location = "ATM-001"; // Default location
}

Transaction::~Transaction() = default;

std::string Transaction::getTransactionId() const {
    return transactionId;
}

TransactionType Transaction::getType() const {
    return type;
}

TransactionStatus Transaction::getStatus() const {
    return status;
}

std::string Transaction::getAccountNumber() const {
    return accountNumber;
}

std::string Transaction::getCardNumber() const {
    return cardNumber;
}

double Transaction::getAmount() const {
    return amount;
}

std::string Transaction::getDescription() const {
    return description;
}

std::chrono::system_clock::time_point Transaction::getTimestamp() const {
    return timestamp;
}

std::string Transaction::getLocation() const {
    return location;
}

std::string Transaction::getReferenceNumber() const {
    return referenceNumber;
}

void Transaction::setStatus(TransactionStatus newStatus) {
    status = newStatus;
}

void Transaction::setDescription(const std::string& desc) {
    description = desc;
}

void Transaction::setReferenceNumber(const std::string& reference) {
    referenceNumber = reference;
}

void Transaction::setLocation(const std::string& loc) {
    location = loc;
}

std::string Transaction::getTransactionTypeString() const {
    switch (type) {
        case TransactionType::WITHDRAWAL: return "Withdrawal";
        case TransactionType::DEPOSIT: return "Deposit";
        case TransactionType::TRANSFER: return "Transfer";
        case TransactionType::BALANCE_INQUIRY: return "Balance Inquiry";
        case TransactionType::PIN_CHANGE: return "PIN Change";
        case TransactionType::MINI_STATEMENT: return "Mini Statement";
        default: return "Unknown";
    }
}

std::string Transaction::getStatusString() const {
    switch (status) {
        case TransactionStatus::PENDING: return "Pending";
        case TransactionStatus::COMPLETED: return "Completed";
        case TransactionStatus::FAILED: return "Failed";
        case TransactionStatus::CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

std::string Transaction::getFormattedTimestamp() const {
    auto time_t = std::chrono::system_clock::to_time_t(timestamp);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Transaction::getTransactionSummary() const {
    std::stringstream ss;
    ss << getFormattedTimestamp() << " - ";
    ss << getTransactionTypeString() << " - ";
    ss << "$" << std::fixed << std::setprecision(2) << amount << " - ";
    ss << getStatusString();
    
    if (!description.empty()) {
        ss << " (" << description << ")";
    }
    
    return ss.str();
}

bool Transaction::isSuccessful() const {
    return status == TransactionStatus::COMPLETED;
}

std::string Transaction::generateTransactionId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(100000, 999999);
    
    std::stringstream ss;
    ss << "TXN" << dis(gen);
    return ss.str();
}

std::string Transaction::generateReferenceNumber() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(100000000, 999999999);
    
    std::stringstream ss;
    ss << "REF" << dis(gen);
    return ss.str();
} 