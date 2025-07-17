#ifndef ATM_HPP
#define ATM_HPP

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include "Card.hpp"
#include "Account.hpp"
#include "Transaction.hpp"
#include "CashDispenser.hpp"
#include "CardReader.hpp"
#include "Display.hpp"
#include "Keypad.hpp"
#include "ReceiptPrinter.hpp"

class ATM {
private:
    std::unique_ptr<CashDispenser> cashDispenser;
    std::unique_ptr<CardReader> cardReader;
    std::unique_ptr<Display> display;
    std::unique_ptr<Keypad> keypad;
    std::unique_ptr<ReceiptPrinter> receiptPrinter;
    
    std::map<std::string, std::shared_ptr<Account>> accounts;
    std::vector<Transaction> transactionHistory;
    
    std::shared_ptr<Card> currentCard;
    std::shared_ptr<Account> currentAccount;
    
    bool isOperational;
    std::mutex atmMutex;
    
    // ATM state management
    enum class ATMState {
        IDLE,
        CARD_INSERTED,
        PIN_ENTERED,
        TRANSACTION_SELECTION,
        PROCESSING_TRANSACTION,
        ERROR
    };
    
    ATMState currentState;
    
    // Configuration
    const int MAX_PIN_ATTEMPTS = 3;
    const double DAILY_WITHDRAWAL_LIMIT = 1000.0;
    const double TRANSACTION_LIMIT = 500.0;
    
public:
    ATM();
    ~ATM();
    
    // Core ATM operations
    bool start();
    void stop();
    bool isRunning() const;
    
    // Card operations
    bool insertCard(const std::string& cardNumber);
    bool ejectCard();
    bool validateCard(const std::string& cardNumber);
    
    // PIN operations
    bool enterPIN(const std::string& pin);
    bool validatePIN(const std::string& pin);
    int getRemainingPINAttempts() const;
    
    // Account operations
    bool selectAccount(const std::string& accountType);
    double getBalance() const;
    std::string getAccountInfo() const;
    
    // Transaction operations
    bool withdraw(double amount);
    bool deposit(double amount);
    bool transfer(const std::string& targetAccount, double amount);
    std::string getTransactionHistory() const;
    
    // ATM status
    bool hasCash(double amount) const;
    double getAvailableCash() const;
    std::string getATMStatus() const;
    
    // Maintenance operations
    void refillCash(double amount);
    void resetDailyLimits();
    void generateMaintenanceReport() const;
    
private:
    // Helper methods
    void initializeSampleAccounts();
    bool validateTransaction(double amount);
    bool checkDailyLimit(double amount);
    void logTransaction(const Transaction& transaction);
    void updateAccountBalance(double amount);
    bool processTransaction(TransactionType type, double amount, const std::string& details = "");
    void displayMessage(const std::string& message);
    void displayError(const std::string& error);
    void changeState(ATMState newState);
};

#endif // ATM_HPP 