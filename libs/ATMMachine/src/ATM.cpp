#include "../include/ATM.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

ATM::ATM() : isOperational(false), currentState(ATMState::IDLE) {
    // Initialize hardware components
    cashDispenser = std::make_unique<CashDispenser>();
    cardReader = std::make_unique<CardReader>();
    display = std::make_unique<Display>();
    keypad = std::make_unique<Keypad>();
    receiptPrinter = std::make_unique<ReceiptPrinter>();
    
    // Initialize with some sample accounts
    initializeSampleAccounts();
}

ATM::~ATM() = default;

void ATM::initializeSampleAccounts() {
    // Create sample accounts for testing
    auto account1 = std::make_shared<Account>("1234567890", "Savings", "John Doe", 5000.0);
    auto account2 = std::make_shared<Account>("0987654321", "Checking", "Jane Smith", 2500.0);
    
    accounts["1234567890"] = account1;
    accounts["0987654321"] = account2;
}

bool ATM::start() {
    std::lock_guard<std::mutex> lock(atmMutex);
    
    if (isOperational) {
        return false; // Already running
    }
    
    // Initialize hardware components
    cashDispenser->setOperationalStatus(true);
    cardReader->setOperationalStatus(true);
    display->setOperationalStatus(true);
    keypad->setOperationalStatus(true);
    receiptPrinter->setOperationalStatus(true);
    
    // Refill cash dispenser with initial cash
    std::map<int, int> initialCash = {
        {100, 50},   // 50 $100 bills
        {50, 100},   // 100 $50 bills
        {20, 200},   // 200 $20 bills
        {10, 300},   // 300 $10 bills
        {5, 500},    // 500 $5 bills
        {1, 1000}    // 1000 $1 bills
    };
    cashDispenser->refillCash(initialCash);
    
    isOperational = true;
    currentState = ATMState::IDLE;
    display->showWelcome();
    
    std::cout << "ATM started successfully" << std::endl;
    return true;
}

void ATM::stop() {
    std::lock_guard<std::mutex> lock(atmMutex);
    
    if (!isOperational) {
        return;
    }
    
    // Eject card if present
    if (currentCard) {
        ejectCard();
    }
    
    // Shutdown hardware components
    cashDispenser->setOperationalStatus(false);
    cardReader->setOperationalStatus(false);
    display->setOperationalStatus(false);
    keypad->setOperationalStatus(false);
    receiptPrinter->setOperationalStatus(false);
    
    isOperational = false;
    currentState = ATMState::IDLE;
    
    std::cout << "ATM stopped" << std::endl;
}

bool ATM::isRunning() const {
    return isOperational;
}

bool ATM::insertCard(const std::string& cardNumber) {
    std::lock_guard<std::mutex> lock(atmMutex);
    
    if (!isOperational || currentState != ATMState::IDLE) {
        return false;
    }
    
    if (!cardReader->insertCard(cardNumber)) {
        display->showError("Card insertion failed");
        return false;
    }
    
    if (!validateCard(cardNumber)) {
        display->showError("Invalid card");
        cardReader->ejectCard();
        return false;
    }
    
    // Create card object
    currentCard = std::make_shared<Card>(cardNumber, "Card Holder", "Bank", "Debit");
    currentState = ATMState::CARD_INSERTED;
    
    display->showMessage("Please enter your PIN");
    return true;
}

bool ATM::ejectCard() {
    std::lock_guard<std::mutex> lock(atmMutex);
    
    if (!cardReader->ejectCard()) {
        return false;
    }
    
    currentCard.reset();
    currentAccount.reset();
    currentState = ATMState::IDLE;
    
    display->showGoodbye();
    return true;
}

bool ATM::validateCard(const std::string& cardNumber) {
    // Basic card validation (Luhn algorithm)
    if (cardNumber.length() < 13 || cardNumber.length() > 19) {
        return false;
    }
    
    int sum = 0;
    bool alternate = false;
    
    for (int i = cardNumber.length() - 1; i >= 0; i--) {
        int n = cardNumber[i] - '0';
        if (alternate) {
            n *= 2;
            if (n > 9) {
                n = (n % 10) + 1;
            }
        }
        sum += n;
        alternate = !alternate;
    }
    
    return (sum % 10 == 0);
}

bool ATM::enterPIN(const std::string& pin) {
    std::lock_guard<std::mutex> lock(atmMutex);
    
    if (currentState != ATMState::CARD_INSERTED) {
        return false;
    }
    
    if (!validatePIN(pin)) {
        display->showError("Invalid PIN");
        return false;
    }
    
    currentState = ATMState::PIN_ENTERED;
    display->showMainMenu();
    return true;
}

bool ATM::validatePIN(const std::string& pin) {
    // In a real system, this would validate against a secure database
    // For demo purposes, we'll use a simple validation
    return pin.length() == 4 && pin == "1234";
}

int ATM::getRemainingPINAttempts() const {
    return MAX_PIN_ATTEMPTS; // Simplified for demo
}

bool ATM::selectAccount(const std::string& accountType) {
    std::lock_guard<std::mutex> lock(atmMutex);
    
    if (currentState != ATMState::PIN_ENTERED) {
        return false;
    }
    
    // Find account for current card
    for (const auto& pair : accounts) {
        if (pair.second->getAccountType() == accountType) {
            currentAccount = pair.second;
            currentState = ATMState::TRANSACTION_SELECTION;
            display->showTransactionMenu();
            return true;
        }
    }
    
    display->showError("Account not found");
    return false;
}

double ATM::getBalance() const {
    if (!currentAccount) {
        return 0.0;
    }
    return currentAccount->getBalance();
}

std::string ATM::getAccountInfo() const {
    if (!currentAccount) {
        return "No account selected";
    }
    return currentAccount->getAccountInfo();
}

bool ATM::withdraw(double amount) {
    std::lock_guard<std::mutex> lock(atmMutex);
    
    if (currentState != ATMState::TRANSACTION_SELECTION) {
        return false;
    }
    
    if (!validateTransaction(amount)) {
        return false;
    }
    
    if (!currentAccount->withdraw(amount)) {
        display->showError("Insufficient funds");
        return false;
    }
    
    if (!cashDispenser->dispenseCash(amount)) {
        display->showError("Cash dispensing failed");
        // Reverse the withdrawal
        currentAccount->deposit(amount);
        return false;
    }
    
    // Create and log transaction
    Transaction transaction(TransactionType::WITHDRAWAL, 
                          currentAccount->getAccountNumber(),
                          currentCard->getCardNumber(), amount);
    transaction.setStatus(TransactionStatus::COMPLETED);
    logTransaction(transaction);
    
    // Print receipt
    receiptPrinter->printReceipt(transaction);
    
    display->showMessage("Please take your cash and receipt");
    return true;
}

bool ATM::deposit(double amount) {
    std::lock_guard<std::mutex> lock(atmMutex);
    
    if (currentState != ATMState::TRANSACTION_SELECTION) {
        return false;
    }
    
    if (!validateTransaction(amount)) {
        return false;
    }
    
    if (!currentAccount->deposit(amount)) {
        display->showError("Deposit failed");
        return false;
    }
    
    // Create and log transaction
    Transaction transaction(TransactionType::DEPOSIT,
                          currentAccount->getAccountNumber(),
                          currentCard->getCardNumber(), amount);
    transaction.setStatus(TransactionStatus::COMPLETED);
    logTransaction(transaction);
    
    // Print receipt
    receiptPrinter->printReceipt(transaction);
    
    display->showMessage("Deposit successful");
    return true;
}

bool ATM::transfer(const std::string& targetAccount, double amount) {
    std::lock_guard<std::mutex> lock(atmMutex);
    
    if (currentState != ATMState::TRANSACTION_SELECTION) {
        return false;
    }
    
    if (!validateTransaction(amount)) {
        return false;
    }
    
    auto target = accounts.find(targetAccount);
    if (target == accounts.end()) {
        display->showError("Target account not found");
        return false;
    }
    
    if (!currentAccount->transfer(*target->second, amount)) {
        display->showError("Transfer failed");
        return false;
    }
    
    // Create and log transaction
    Transaction transaction(TransactionType::TRANSFER,
                          currentAccount->getAccountNumber(),
                          currentCard->getCardNumber(), amount);
    transaction.setStatus(TransactionStatus::COMPLETED);
    transaction.setDescription("Transfer to " + targetAccount);
    logTransaction(transaction);
    
    // Print receipt
    receiptPrinter->printReceipt(transaction);
    
    display->showMessage("Transfer successful");
    return true;
}

std::string ATM::getTransactionHistory() const {
    if (!currentAccount) {
        return "No account selected";
    }
    
    auto transactions = currentAccount->getRecentTransactions(5);
    std::stringstream ss;
    ss << "Recent Transactions:\n";
    
    for (const auto& transaction : transactions) {
        ss << transaction.getTransactionSummary() << "\n";
    }
    
    return ss.str();
}

bool ATM::hasCash(double amount) const {
    return cashDispenser->hasSufficientCash(amount);
}

double ATM::getAvailableCash() const {
    return cashDispenser->getAvailableCash();
}

std::string ATM::getATMStatus() const {
    std::stringstream ss;
    ss << "ATM Status:\n";
    ss << "Operational: " << (isOperational ? "Yes" : "No") << "\n";
    ss << "Available Cash: $" << std::fixed << std::setprecision(2) << getAvailableCash() << "\n";
    ss << "Card Present: " << (currentCard ? "Yes" : "No") << "\n";
    ss << "Account Selected: " << (currentAccount ? "Yes" : "No") << "\n";
    
    return ss.str();
}

void ATM::refillCash(double amount) {
    std::lock_guard<std::mutex> lock(atmMutex);
    
    // Calculate denominations for refill
    std::map<int, int> denominations;
    double remaining = amount;
    
    std::vector<int> bills = {100, 50, 20, 10, 5, 1};
    for (int bill : bills) {
        int count = static_cast<int>(remaining / bill);
        if (count > 0) {
            denominations[bill] = count;
            remaining -= count * bill;
        }
    }
    
    cashDispenser->refillCash(denominations);
    std::cout << "ATM refilled with $" << amount << std::endl;
}

void ATM::resetDailyLimits() {
    std::lock_guard<std::mutex> lock(atmMutex);
    
    for (auto& pair : accounts) {
        pair.second->resetDailyLimits();
    }
    
    std::cout << "Daily limits reset" << std::endl;
}

void ATM::generateMaintenanceReport() const {
    std::cout << "\n=== ATM Maintenance Report ===" << std::endl;
    std::cout << getATMStatus() << std::endl;
    std::cout << "Cash Dispenser: " << cashDispenser->getInventoryReport() << std::endl;
    std::cout << "Receipt Printer Paper: " << receiptPrinter->getPaperCount() << "/" 
              << receiptPrinter->getMaxPaperCapacity() << std::endl;
    std::cout << "Total Transactions: " << transactionHistory.size() << std::endl;
}

// Private helper methods

bool ATM::validateTransaction(double amount) {
    if (amount <= 0) {
        display->showError("Invalid amount");
        return false;
    }
    
    if (amount > TRANSACTION_LIMIT) {
        display->showError("Amount exceeds transaction limit");
        return false;
    }
    
    if (!checkDailyLimit(amount)) {
        display->showError("Daily limit exceeded");
        return false;
    }
    
    if (!hasCash(amount)) {
        display->showError("Insufficient cash in ATM");
        return false;
    }
    
    return true;
}

bool ATM::checkDailyLimit(double amount) {
    if (!currentAccount) {
        return false;
    }
    return currentAccount->checkDailyLimit(amount);
}

void ATM::logTransaction(const Transaction& transaction) {
    transactionHistory.push_back(transaction);
    if (currentAccount) {
        currentAccount->addTransaction(transaction);
    }
}

void ATM::updateAccountBalance(double amount) {
    if (currentAccount) {
        currentAccount->updateBalance(amount);
    }
}

bool ATM::processTransaction(TransactionType type, double amount, const std::string& details) {
    Transaction transaction(type, 
                          currentAccount ? currentAccount->getAccountNumber() : "",
                          currentCard ? currentCard->getCardNumber() : "", 
                          amount);
    
    if (!details.empty()) {
        transaction.setDescription(details);
    }
    
    logTransaction(transaction);
    return true;
}

void ATM::displayMessage(const std::string& message) {
    display->showMessage(message);
}

void ATM::displayError(const std::string& error) {
    display->showError(error);
}

void ATM::changeState(ATMState newState) {
    currentState = newState;
} 