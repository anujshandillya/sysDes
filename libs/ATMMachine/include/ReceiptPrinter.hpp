#ifndef RECEIPT_PRINTER_HPP
#define RECEIPT_PRINTER_HPP

#include <string>
#include <vector>
#include "Transaction.hpp"

class ReceiptPrinter {
private:
    bool isOperational;
    bool hasPaper;
    int paperCount;
    int maxPaperCapacity;
    std::vector<std::string> printHistory;
    
public:
    ReceiptPrinter();
    ~ReceiptPrinter();
    
    // Printing operations
    bool printReceipt(const Transaction& transaction);
    bool printMiniStatement(const std::vector<Transaction>& transactions);
    bool printBalanceReceipt(const std::string& accountNumber, double balance);
    bool printErrorReceipt(const std::string& error);
    
    // Paper management
    bool hasSufficientPaper() const;
    int getPaperCount() const;
    void addPaper(int count);
    void removePaper(int count);
    int getMaxPaperCapacity() const;
    void setMaxPaperCapacity(int capacity);
    
    // Status operations
    bool isPrinterOperational() const;
    void setOperationalStatus(bool status);
    std::vector<std::string> getPrintHistory() const;
    void clearHistory();
    
    // Maintenance operations
    void refillPaper(int count);
    bool needsRefill() const;
    std::string getPrinterStatus() const;
    
private:
    std::string formatReceipt(const Transaction& transaction) const;
    std::string formatMiniStatement(const std::vector<Transaction>& transactions) const;
    std::string formatBalanceReceipt(const std::string& accountNumber, double balance) const;
    std::string formatErrorReceipt(const std::string& error) const;
    void addToHistory(const std::string& receipt);
};

#endif // RECEIPT_PRINTER_HPP