#ifndef CASH_DISPENSER_HPP
#define CASH_DISPENSER_HPP

#include <map>
#include <string>

class CashDispenser {
private:
    std::map<int, int> denominationCount; // denomination -> count
    double totalCash;
    bool isOperational;
    int maxCapacity;
    
public:
    CashDispenser();
    ~CashDispenser();
    
    // Cash operations
    bool dispenseCash(double amount);
    bool hasSufficientCash(double amount) const;
    double getAvailableCash() const;
    
    // Inventory management
    void addCash(int denomination, int count);
    void removeCash(int denomination, int count);
    std::map<int, int> getDenominationCount() const;
    
    // Status operations
    bool isDispenserOperational() const;
    void setOperationalStatus(bool status);
    int getMaxCapacity() const;
    void setMaxCapacity(int capacity);
    
    // Maintenance operations
    void refillCash(const std::map<int, int>& denominations);
    std::string getInventoryReport() const;
    bool needsRefill() const;
    
private:
    std::map<int, int> calculateDenominations(double amount) const;
    bool validateDenomination(int denomination) const;
};

#endif // CASH_DISPENSER_HPP 