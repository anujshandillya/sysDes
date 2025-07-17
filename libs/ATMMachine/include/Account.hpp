#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <string>
#include <vector>
#include <chrono>
#include "Transaction.hpp"

class Account {
private:
    std::string accountNumber;
    std::string accountType; // Savings, Checking, Credit
    std::string accountHolderName;
    double balance;
    double dailyWithdrawalLimit;
    double dailyWithdrawn;
    std::chrono::system_clock::time_point lastResetDate;
    bool isActive;
    std::vector<Transaction> transactionHistory;
    
public:
    Account(const std::string& number, const std::string& type, 
            const std::string& holder, double initialBalance = 0.0);
    ~Account();
    
    // Getters
    std::string getAccountNumber() const;
    std::string getAccountType() const;
    std::string getAccountHolderName() const;
    double getBalance() const;
    double getDailyWithdrawalLimit() const;
    double getDailyWithdrawn() const;
    bool isAccountActive() const;
    
    // Account operations
    bool deposit(double amount);
    bool withdraw(double amount);
    bool transfer(Account& targetAccount, double amount);
    bool updateBalance(double amount);
    
    // Transaction management
    void addTransaction(const Transaction& transaction);
    std::vector<Transaction> getTransactionHistory() const;
    std::vector<Transaction> getRecentTransactions(int count = 10) const;
    
    // Daily limit management
    bool checkDailyLimit(double amount) const;
    void resetDailyLimits();
    void updateDailyWithdrawn(double amount);
    
    // Account status
    std::string getAccountInfo() const;
    double getAvailableBalance() const;
    bool canWithdraw(double amount) const;
    
private:
    void updateLastResetDate();
    bool isNewDay() const;
};

#endif // ACCOUNT_HPP 