#include "../include/Account.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <string>

Account::Account(const std::string& number, const std::string& type, 
                 const std::string& holder, double initialBalance)
    : accountNumber(number), accountType(type), accountHolderName(holder),
      balance(initialBalance), dailyWithdrawn(0.0), isActive(true) {
    
    // Set daily withdrawal limit based on account type
    if (type == "Savings") {
        dailyWithdrawalLimit = 1000.0;
    } else if (type == "Checking") {
        dailyWithdrawalLimit = 2000.0;
    } else if (type == "Credit") {
        dailyWithdrawalLimit = 5000.0;
    } else {
        dailyWithdrawalLimit = 1000.0; // Default
    }
    
    lastResetDate = std::chrono::system_clock::now();
}

Account::~Account() = default;

std::string Account::getAccountNumber() const {
    return accountNumber;
}

std::string Account::getAccountType() const {
    return accountType;
}

std::string Account::getAccountHolderName() const {
    return accountHolderName;
}

double Account::getBalance() const {
    return balance;
}

double Account::getDailyWithdrawalLimit() const {
    return dailyWithdrawalLimit;
}

double Account::getDailyWithdrawn() const {
    return dailyWithdrawn;
}

bool Account::isAccountActive() const {
    return isActive;
}

bool Account::deposit(double amount) {
    if (amount <= 0) {
        return false;
    }
    
    balance += amount;
    
    // Create transaction record
    Transaction transaction(TransactionType::DEPOSIT, accountNumber, "", amount);
    transaction.setStatus(TransactionStatus::COMPLETED);
    addTransaction(transaction);
    
    return true;
}

bool Account::withdraw(double amount) {
    if (amount <= 0) {
        return false;
    }
    
    if (amount > balance) {
        return false;
    }
    
    if (!checkDailyLimit(amount)) {
        return false;
    }
    
    balance -= amount;
    updateDailyWithdrawn(amount);
    
    // Create transaction record
    Transaction transaction(TransactionType::WITHDRAWAL, accountNumber, "", amount);
    transaction.setStatus(TransactionStatus::COMPLETED);
    addTransaction(transaction);
    
    return true;
}

bool Account::transfer(Account& targetAccount, double amount) {
    if (amount <= 0) {
        return false;
    }
    
    if (amount > balance) {
        return false;
    }
    
    if (!checkDailyLimit(amount)) {
        return false;
    }
    
    // Perform transfer
    balance -= amount;
    targetAccount.balance += amount;
    
    updateDailyWithdrawn(amount);
    
    // Create transaction records
    Transaction withdrawal(TransactionType::TRANSFER, accountNumber, "", amount);
    withdrawal.setStatus(TransactionStatus::COMPLETED);
    withdrawal.setDescription("Transfer to " + targetAccount.getAccountNumber());
    addTransaction(withdrawal);
    
    Transaction deposit(TransactionType::TRANSFER, targetAccount.getAccountNumber(), "", amount);
    deposit.setStatus(TransactionStatus::COMPLETED);
    deposit.setDescription("Transfer from " + accountNumber);
    targetAccount.addTransaction(deposit);
    
    return true;
}

bool Account::updateBalance(double amount) {
    balance += amount;
    return true;
}

void Account::addTransaction(const Transaction& transaction) {
    transactionHistory.push_back(transaction);
}

std::vector<Transaction> Account::getTransactionHistory() const {
    return transactionHistory;
}

std::vector<Transaction> Account::getRecentTransactions(int count) const {
    std::vector<Transaction> recent;
    int start = std::max(0, static_cast<int>(transactionHistory.size()) - count);
    
    for (size_t i = start; i < transactionHistory.size(); ++i) {
        recent.push_back(transactionHistory[i]);
    }
    
    return recent;
}

bool Account::checkDailyLimit(double amount) const {
    if (isNewDay()) {
        return amount <= dailyWithdrawalLimit;
    }
    
    return (dailyWithdrawn + amount) <= dailyWithdrawalLimit;
}

void Account::resetDailyLimits() {
    dailyWithdrawn = 0.0;
    updateLastResetDate();
}

void Account::updateDailyWithdrawn(double amount) {
    if (isNewDay()) {
        dailyWithdrawn = 0.0;
        updateLastResetDate();
    }
    
    dailyWithdrawn += amount;
}

std::string Account::getAccountInfo() const {
    std::stringstream ss;
    ss << "Account Number: " << accountNumber << "\n";
    ss << "Account Type: " << accountType << "\n";
    ss << "Account Holder: " << accountHolderName << "\n";
    ss << "Balance: $" << std::fixed << std::setprecision(2) << balance << "\n";
    ss << "Daily Withdrawal Limit: $" << dailyWithdrawalLimit << "\n";
    ss << "Daily Withdrawn: $" << dailyWithdrawn << "\n";
    ss << "Status: " << (isActive ? "Active" : "Inactive");
    
    return ss.str();
}

double Account::getAvailableBalance() const {
    return balance;
}

bool Account::canWithdraw(double amount) const {
    return amount <= balance && checkDailyLimit(amount);
}

void Account::updateLastResetDate() {
    lastResetDate = std::chrono::system_clock::now();
}

bool Account::isNewDay() const {
    auto now = std::chrono::system_clock::now();
    auto time_diff = std::chrono::duration_cast<std::chrono::hours>(now - lastResetDate);
    return time_diff.count() >= 24;
} 