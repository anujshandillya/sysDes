#include "../include/ReceiptPrinter.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

ReceiptPrinter::ReceiptPrinter() : isOperational(false), hasPaper(true), 
                                   paperCount(100), maxPaperCapacity(200) {
}

ReceiptPrinter::~ReceiptPrinter() = default;

bool ReceiptPrinter::printReceipt(const Transaction& transaction) {
    if (!isOperational) {
        std::cout << "[PRINTER ERROR] Printer not operational" << std::endl;
        return false;
    }
    
    if (!hasSufficientPaper()) {
        std::cout << "[PRINTER ERROR] Insufficient paper" << std::endl;
        return false;
    }
    
    std::string receipt = formatReceipt(transaction);
    addToHistory(receipt);
    removePaper(1);
    
    std::cout << "[PRINTER] Receipt printed:" << std::endl;
    std::cout << receipt << std::endl;
    
    return true;
}

bool ReceiptPrinter::printMiniStatement(const std::vector<Transaction>& transactions) {
    if (!isOperational) {
        std::cout << "[PRINTER ERROR] Printer not operational" << std::endl;
        return false;
    }
    
    if (!hasSufficientPaper()) {
        std::cout << "[PRINTER ERROR] Insufficient paper" << std::endl;
        return false;
    }
    
    std::string statement = formatMiniStatement(transactions);
    addToHistory(statement);
    removePaper(1);
    
    std::cout << "[PRINTER] Mini statement printed:" << std::endl;
    std::cout << statement << std::endl;
    
    return true;
}

bool ReceiptPrinter::printBalanceReceipt(const std::string& accountNumber, double balance) {
    if (!isOperational) {
        std::cout << "[PRINTER ERROR] Printer not operational" << std::endl;
        return false;
    }
    
    if (!hasSufficientPaper()) {
        std::cout << "[PRINTER ERROR] Insufficient paper" << std::endl;
        return false;
    }
    
    std::string receipt = formatBalanceReceipt(accountNumber, balance);
    addToHistory(receipt);
    removePaper(1);
    
    std::cout << "[PRINTER] Balance receipt printed:" << std::endl;
    std::cout << receipt << std::endl;
    
    return true;
}

bool ReceiptPrinter::printErrorReceipt(const std::string& error) {
    if (!isOperational) {
        std::cout << "[PRINTER ERROR] Printer not operational" << std::endl;
        return false;
    }
    
    if (!hasSufficientPaper()) {
        std::cout << "[PRINTER ERROR] Insufficient paper" << std::endl;
        return false;
    }
    
    std::string receipt = formatErrorReceipt(error);
    addToHistory(receipt);
    removePaper(1);
    
    std::cout << "[PRINTER] Error receipt printed:" << std::endl;
    std::cout << receipt << std::endl;
    
    return true;
}

bool ReceiptPrinter::hasSufficientPaper() const {
    return paperCount > 0;
}

int ReceiptPrinter::getPaperCount() const {
    return paperCount;
}

void ReceiptPrinter::addPaper(int count) {
    if (count > 0) {
        paperCount = std::min(paperCount + count, maxPaperCapacity);
        hasPaper = (paperCount > 0);
    }
}

void ReceiptPrinter::removePaper(int count) {
    if (count > 0) {
        paperCount = std::max(0, paperCount - count);
        hasPaper = (paperCount > 0);
    }
}

int ReceiptPrinter::getMaxPaperCapacity() const {
    return maxPaperCapacity;
}

void ReceiptPrinter::setMaxPaperCapacity(int capacity) {
    maxPaperCapacity = capacity;
}

bool ReceiptPrinter::isPrinterOperational() const {
    return isOperational;
}

void ReceiptPrinter::setOperationalStatus(bool status) {
    isOperational = status;
}

std::vector<std::string> ReceiptPrinter::getPrintHistory() const {
    return printHistory;
}

void ReceiptPrinter::clearHistory() {
    printHistory.clear();
}

void ReceiptPrinter::refillPaper(int count) {
    addPaper(count);
    std::cout << "[PRINTER] Paper refilled. Current count: " << paperCount << std::endl;
}

bool ReceiptPrinter::needsRefill() const {
    return paperCount < (maxPaperCapacity * 0.2); // Less than 20% of capacity
}

std::string ReceiptPrinter::getPrinterStatus() const {
    std::stringstream ss;
    ss << "Printer Status:\n";
    ss << "Operational: " << (isOperational ? "Yes" : "No") << "\n";
    ss << "Paper Count: " << paperCount << "/" << maxPaperCapacity << "\n";
    ss << "Has Paper: " << (hasPaper ? "Yes" : "No") << "\n";
    ss << "Needs Refill: " << (needsRefill() ? "Yes" : "No");
    
    return ss.str();
}

std::string ReceiptPrinter::formatReceipt(const Transaction& transaction) const {
    std::stringstream ss;
    ss << "================================\n";
    ss << "           ATM RECEIPT          \n";
    ss << "================================\n";
    ss << "Transaction ID: " << transaction.getTransactionId() << "\n";
    ss << "Reference: " << transaction.getReferenceNumber() << "\n";
    ss << "Date: " << transaction.getFormattedTimestamp() << "\n";
    ss << "Location: " << transaction.getLocation() << "\n";
    ss << "--------------------------------\n";
    ss << "Transaction Type: " << transaction.getTransactionTypeString() << "\n";
    ss << "Account: " << transaction.getAccountNumber() << "\n";
    ss << "Amount: $" << std::fixed << std::setprecision(2) << transaction.getAmount() << "\n";
    ss << "Status: " << transaction.getStatusString() << "\n";
    
    if (!transaction.getDescription().empty()) {
        ss << "Description: " << transaction.getDescription() << "\n";
    }
    
    ss << "================================\n";
    ss << "        Thank You!              \n";
    ss << "================================\n";
    
    return ss.str();
}

std::string ReceiptPrinter::formatMiniStatement(const std::vector<Transaction>& transactions) const {
    std::stringstream ss;
    ss << "================================\n";
    ss << "        MINI STATEMENT          \n";
    ss << "================================\n";
    ss << "Date: " << transactions[0].getFormattedTimestamp() << "\n";
    ss << "Account: " << transactions[0].getAccountNumber() << "\n";
    ss << "--------------------------------\n";
    
    for (const auto& transaction : transactions) {
        ss << transaction.getFormattedTimestamp() << " ";
        ss << transaction.getTransactionTypeString() << " ";
        ss << "$" << std::fixed << std::setprecision(2) << transaction.getAmount() << " ";
        ss << transaction.getStatusString() << "\n";
    }
    
    ss << "================================\n";
    ss << "        End of Statement        \n";
    ss << "================================\n";
    
    return ss.str();
}

std::string ReceiptPrinter::formatBalanceReceipt(const std::string& accountNumber, double balance) const {
    std::stringstream ss;
    ss << "================================\n";
    ss << "        BALANCE RECEIPT         \n";
    ss << "================================\n";
    ss << "Account: " << accountNumber << "\n";
    ss << "Balance: $" << std::fixed << std::setprecision(2) << balance << "\n";
    ss << "Date: " << std::chrono::system_clock::now() << "\n";
    ss << "================================\n";
    ss << "        Thank You!              \n";
    ss << "================================\n";
    
    return ss.str();
}

std::string ReceiptPrinter::formatErrorReceipt(const std::string& error) const {
    std::stringstream ss;
    ss << "================================\n";
    ss << "         ERROR RECEIPT          \n";
    ss << "================================\n";
    ss << "Error: " << error << "\n";
    ss << "Date: " << std::chrono::system_clock::now() << "\n";
    ss << "Please contact customer service\n";
    ss << "================================\n";
    
    return ss.str();
}

void ReceiptPrinter::addToHistory(const std::string& receipt) {
    printHistory.push_back(receipt);
    
    // Keep only last 50 receipts
    if (printHistory.size() > 50) {
        printHistory.erase(printHistory.begin());
    }
} 