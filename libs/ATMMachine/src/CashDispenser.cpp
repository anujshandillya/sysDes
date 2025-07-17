#include "../include/CashDispenser.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

CashDispenser::CashDispenser() : totalCash(0.0), isOperational(false), maxCapacity(10000) {
    // Initialize with standard denominations
    denominationCount = {
        {100, 0}, {50, 0}, {20, 0}, {10, 0}, {5, 0}, {1, 0}
    };
}

CashDispenser::~CashDispenser() = default;

bool CashDispenser::dispenseCash(double amount) {
    if (!isOperational) {
        return false;
    }
    
    if (!hasSufficientCash(amount)) {
        return false;
    }
    
    auto denominations = calculateDenominations(amount);
    if (denominations.empty()) {
        return false;
    }
    
    // Remove cash from inventory
    for (const auto& pair : denominations) {
        removeCash(pair.first, pair.second);
    }
    
    std::cout << "Dispensing $" << amount << std::endl;
    return true;
}

bool CashDispenser::hasSufficientCash(double amount) const {
    return totalCash >= amount;
}

double CashDispenser::getAvailableCash() const {
    return totalCash;
}

void CashDispenser::addCash(int denomination, int count) {
    if (!validateDenomination(denomination) || count < 0) {
        return;
    }
    
    denominationCount[denomination] += count;
    totalCash += denomination * count;
}

void CashDispenser::removeCash(int denomination, int count) {
    if (!validateDenomination(denomination) || count < 0) {
        return;
    }
    
    if (denominationCount[denomination] >= count) {
        denominationCount[denomination] -= count;
        totalCash -= denomination * count;
    }
}

std::map<int, int> CashDispenser::getDenominationCount() const {
    return denominationCount;
}

bool CashDispenser::isDispenserOperational() const {
    return isOperational;
}

void CashDispenser::setOperationalStatus(bool status) {
    isOperational = status;
}

int CashDispenser::getMaxCapacity() const {
    return maxCapacity;
}

void CashDispenser::setMaxCapacity(int capacity) {
    maxCapacity = capacity;
}

void CashDispenser::refillCash(const std::map<int, int>& denominations) {
    for (const auto& pair : denominations) {
        addCash(pair.first, pair.second);
    }
    
    std::cout << "Cash dispenser refilled. Total cash: $" << totalCash << std::endl;
}

std::string CashDispenser::getInventoryReport() const {
    std::stringstream ss;
    ss << "Cash Inventory Report:\n";
    ss << "Total Cash: $" << std::fixed << std::setprecision(2) << totalCash << "\n";
    ss << "Denomination Breakdown:\n";
    
    for (const auto& pair : denominationCount) {
        ss << "  $" << pair.first << " bills: " << pair.second << " ($" 
           << (pair.first * pair.second) << ")\n";
    }
    
    ss << "Operational: " << (isOperational ? "Yes" : "No") << "\n";
    ss << "Capacity: $" << maxCapacity;
    
    return ss.str();
}

bool CashDispenser::needsRefill() const {
    return totalCash < (maxCapacity * 0.1); // Less than 10% of capacity
}

std::map<int, int> CashDispenser::calculateDenominations(double amount) const {
    std::map<int, int> result;
    double remaining = amount;
    
    // Try to dispense using available denominations
    std::vector<int> bills = {100, 50, 20, 10, 5, 1};
    
    for (int bill : bills) {
        if (remaining <= 0) break;
        
        int available = denominationCount.at(bill);
        int needed = static_cast<int>(remaining / bill);
        int toDispense = std::min(available, needed);
        
        if (toDispense > 0) {
            result[bill] = toDispense;
            remaining -= toDispense * bill;
        }
    }
    
    // If we couldn't dispense the exact amount, return empty
    if (remaining > 0.01) { // Allow for small rounding errors
        return {};
    }
    
    return result;
}

bool CashDispenser::validateDenomination(int denomination) const {
    return denominationCount.find(denomination) != denominationCount.end();
} 