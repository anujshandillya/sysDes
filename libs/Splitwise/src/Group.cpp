#include "../include/Group.hpp"
#include <algorithm>
#include <ctime>
#include <sstream>
#include <iomanip>

Group::Group(const std::string& groupId, const std::string& name, const std::string& createdBy,
             GroupType type, const std::string& description, const std::string& currency)
    : groupId(groupId), name(name), description(description), createdBy(createdBy), type(type),
      currency(currency), isActive(true) {
    
    // Set creation timestamp
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    createdAt = oss.str();
    updatedAt = createdAt;
    
    // Add creator as first member
    addMember(createdBy);
}

void Group::addMember(const std::string& userId) {
    if (!hasMember(userId)) {
        memberIds.push_back(userId);
        memberBalances[userId] = 0.0;
    }
}

void Group::removeMember(const std::string& userId) {
    memberIds.erase(std::remove(memberIds.begin(), memberIds.end(), userId), memberIds.end());
    memberBalances.erase(userId);
}

bool Group::hasMember(const std::string& userId) const {
    return std::find(memberIds.begin(), memberIds.end(), userId) != memberIds.end();
}

void Group::updateMemberBalance(const std::string& userId, double amount) {
    if (hasMember(userId)) {
        memberBalances[userId] += amount;
    }
}

double Group::getMemberBalance(const std::string& userId) const {
    auto it = memberBalances.find(userId);
    return (it != memberBalances.end()) ? it->second : 0.0;
}

double Group::getTotalGroupBalance() const {
    double total = 0.0;
    for (const auto& balance : memberBalances) {
        total += balance.second;
    }
    return total;
}

void Group::resetAllBalances() {
    for (auto& balance : memberBalances) {
        balance.second = 0.0;
    }
}

void Group::addExpense(const std::string& expenseId) {
    if (std::find(expenseIds.begin(), expenseIds.end(), expenseId) == expenseIds.end()) {
        expenseIds.push_back(expenseId);
    }
}

void Group::removeExpense(const std::string& expenseId) {
    expenseIds.erase(std::remove(expenseIds.begin(), expenseIds.end(), expenseId), expenseIds.end());
}

bool Group::hasExpense(const std::string& expenseId) const {
    return std::find(expenseIds.begin(), expenseIds.end(), expenseId) != expenseIds.end();
}

std::string Group::getTypeString() const {
    switch (type) {
        case GroupType::TRIP: return "Trip";
        case GroupType::HOUSE: return "House";
        case GroupType::COUPLE: return "Couple";
        case GroupType::EVENT: return "Event";
        case GroupType::PROJECT: return "Project";
        case GroupType::OTHER: return "Other";
        default: return "Unknown";
    }
}

std::vector<std::string> Group::getDebtors() const {
    std::vector<std::string> debtors;
    for (const auto& balance : memberBalances) {
        if (balance.second < 0) {
            debtors.push_back(balance.first);
        }
    }
    return debtors;
}

std::vector<std::string> Group::getCreditors() const {
    std::vector<std::string> creditors;
    for (const auto& balance : memberBalances) {
        if (balance.second > 0) {
            creditors.push_back(balance.first);
        }
    }
    return creditors;
}

bool Group::isBalanced() const {
    return getTotalGroupBalance() == 0.0;
}

void Group::updateTimestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    updatedAt = oss.str();
} 