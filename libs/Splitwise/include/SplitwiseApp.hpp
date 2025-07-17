#pragma once

#include "User.hpp"
#include "Group.hpp"
#include "Expense.hpp"
#include "Settlement.hpp"
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

struct DebtSummary {
    std::string fromUserId;
    std::string toUserId;
    double amount;
    std::string currency;
    
    DebtSummary(const std::string& from, const std::string& to, double amt, const std::string& curr = "USD")
        : fromUserId(from), toUserId(to), amount(amt), currency(curr) {}
};

struct GroupSummary {
    std::string groupId;
    std::string groupName;
    double totalExpenses;
    double totalSettlements;
    double netBalance;
    int memberCount;
    int expenseCount;
    
    GroupSummary(const std::string& id, const std::string& name, double expenses = 0.0, 
                 double settlements = 0.0, double balance = 0.0, int members = 0, int expenses_count = 0)
        : groupId(id), groupName(name), totalExpenses(expenses), totalSettlements(settlements),
          netBalance(balance), memberCount(members), expenseCount(expenses_count) {}
};

class SplitwiseApp {
private:
    std::unordered_map<std::string, std::shared_ptr<User>> users;
    std::unordered_map<std::string, std::shared_ptr<Group>> groups;
    std::unordered_map<std::string, std::shared_ptr<Expense>> expenses;
    std::unordered_map<std::string, std::shared_ptr<Settlement>> settlements;
    
    std::string appName;
    std::string version;
    std::string defaultCurrency;
    int nextUserId;
    int nextGroupId;
    int nextExpenseId;
    int nextSettlementId;

public:
    SplitwiseApp(const std::string& name = "Splitwise", const std::string& ver = "1.0.0",
                 const std::string& defaultCurr = "USD");

    // User management
    std::shared_ptr<User> createUser(const std::string& name, const std::string& email,
                                    const std::string& phone = "", Currency preferredCurrency = Currency::USD);
    std::shared_ptr<User> findUser(const std::string& userId);
    bool deleteUser(const std::string& userId);
    std::vector<std::shared_ptr<User>> getAllUsers();
    std::vector<std::shared_ptr<User>> searchUsers(const std::string& query);

    // Group management
    std::shared_ptr<Group> createGroup(const std::string& name, const std::string& createdBy,
                                      GroupType type = GroupType::OTHER, const std::string& description = "");
    std::shared_ptr<Group> findGroup(const std::string& groupId);
    bool deleteGroup(const std::string& groupId);
    std::vector<std::shared_ptr<Group>> getAllGroups();
    std::vector<std::shared_ptr<Group>> getGroupsForUser(const std::string& userId);
    bool addUserToGroup(const std::string& userId, const std::string& groupId);
    bool removeUserFromGroup(const std::string& userId, const std::string& groupId);

    // Expense management
    std::shared_ptr<Expense> createExpense(const std::string& description, double amount,
                                          const std::string& paidBy, const std::string& groupId,
                                          ExpenseCategory category = ExpenseCategory::OTHER);
    std::shared_ptr<Expense> findExpense(const std::string& expenseId);
    bool deleteExpense(const std::string& expenseId);
    std::vector<std::shared_ptr<Expense>> getExpensesForGroup(const std::string& groupId);
    std::vector<std::shared_ptr<Expense>> getExpensesForUser(const std::string& userId);
    std::vector<std::shared_ptr<Expense>> getExpensesByCategory(const std::string& groupId, ExpenseCategory category);

    // Settlement management
    std::shared_ptr<Settlement> createSettlement(const std::string& fromUserId,
                                                 const std::string& toUserId, const std::string& groupId,
                                                 double amount, SettlementMethod method = SettlementMethod::CASH);
    std::shared_ptr<Settlement> findSettlement(const std::string& settlementId);
    bool deleteSettlement(const std::string& settlementId);
    std::vector<std::shared_ptr<Settlement>> getSettlementsForGroup(const std::string& groupId);
    std::vector<std::shared_ptr<Settlement>> getSettlementsForUser(const std::string& userId);
    bool completeSettlement(const std::string& settlementId);

    // Balance and debt management
    void calculateBalances(const std::string& groupId);
    std::vector<DebtSummary> getDebtSummary(const std::string& groupId);
    std::vector<DebtSummary> getDebtSummaryForUser(const std::string& userId);
    double getUserBalance(const std::string& userId, const std::string& groupId);
    double getTotalUserBalance(const std::string& userId);
    std::vector<std::string> getSimplifiedDebts(const std::string& groupId);

    // Reporting and analytics
    GroupSummary getGroupSummary(const std::string& groupId);
    std::vector<GroupSummary> getAllGroupSummaries();
    std::vector<std::shared_ptr<Expense>> getTopExpenses(const std::string& groupId, int limit = 10);
    std::unordered_map<ExpenseCategory, double> getExpenseBreakdown(const std::string& groupId);
    std::vector<std::pair<std::string, double>> getTopSpenders(const std::string& groupId, int limit = 5);

    // Utility methods
    std::string generateUserId();
    std::string generateGroupId();
    std::string generateExpenseId();
    std::string generateSettlementId();
    std::string getCurrentTimestamp();
    void validateUserExists(const std::string& userId);
    void validateGroupExists(const std::string& groupId);
    void validateExpenseExists(const std::string& expenseId);
    void validateSettlementExists(const std::string& settlementId);
    bool isUserInGroup(const std::string& userId, const std::string& groupId);

    // Getters
    std::string getAppName() const { return appName; }
    std::string getVersion() const { return version; }
    std::string getDefaultCurrency() const { return defaultCurrency; }
    int getUserCount() const { return users.size(); }
    int getGroupCount() const { return groups.size(); }
    int getExpenseCount() const { return expenses.size(); }
    int getSettlementCount() const { return settlements.size(); }
}; 