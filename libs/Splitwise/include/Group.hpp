#pragma once

#include <string>
#include <vector>
#include <unordered_map>

enum class GroupType {
    TRIP,
    HOUSE,
    COUPLE,
    EVENT,
    PROJECT,
    OTHER
};

enum class SplitType {
    EQUAL,
    PERCENTAGE,
    SHARES,
    CUSTOM
};

class Group {
private:
    std::string groupId;
    std::string name;
    std::string description;
    std::string createdBy;
    GroupType type;
    std::vector<std::string> memberIds;
    std::unordered_map<std::string, double> memberBalances;
    std::vector<std::string> expenseIds;
    std::string currency;
    std::string createdAt;
    std::string updatedAt;
    bool isActive;
    std::string groupPicture;

public:
    Group(const std::string& groupId, const std::string& name, const std::string& createdBy,
          GroupType type = GroupType::OTHER, const std::string& description = "",
          const std::string& currency = "USD");

    // Getters
    std::string getGroupId() const { return groupId; }
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    std::string getCreatedBy() const { return createdBy; }
    GroupType getType() const { return type; }
    std::vector<std::string> getMemberIds() const { return memberIds; }
    std::unordered_map<std::string, double> getMemberBalances() const { return memberBalances; }
    std::vector<std::string> getExpenseIds() const { return expenseIds; }
    std::string getCurrency() const { return currency; }
    std::string getCreatedAt() const { return createdAt; }
    std::string getUpdatedAt() const { return updatedAt; }
    bool getIsActive() const { return isActive; }
    std::string getGroupPicture() const { return groupPicture; }

    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setDescription(const std::string& desc) { description = desc; }
    void setType(GroupType newType) { type = newType; }
    void setCurrency(const std::string& curr) { currency = curr; }
    void setIsActive(bool active) { isActive = active; }
    void setGroupPicture(const std::string& picture) { groupPicture = picture; }

    // Member management
    void addMember(const std::string& userId);
    void removeMember(const std::string& userId);
    bool hasMember(const std::string& userId) const;
    int getMemberCount() const { return memberIds.size(); }

    // Balance management
    void updateMemberBalance(const std::string& userId, double amount);
    double getMemberBalance(const std::string& userId) const;
    double getTotalGroupBalance() const;
    void resetAllBalances();

    // Expense management
    void addExpense(const std::string& expenseId);
    void removeExpense(const std::string& expenseId);
    bool hasExpense(const std::string& expenseId) const;
    int getExpenseCount() const { return expenseIds.size(); }

    // Utility methods
    std::string getTypeString() const;
    std::vector<std::string> getDebtors() const;
    std::vector<std::string> getCreditors() const;
    bool isBalanced() const;
    void updateTimestamp();
}; 