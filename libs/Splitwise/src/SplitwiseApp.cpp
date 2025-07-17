#include "../include/SplitwiseApp.hpp"
#include <algorithm>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdexcept>

SplitwiseApp::SplitwiseApp(const std::string& name, const std::string& ver, const std::string& defaultCurr)
    : appName(name), version(ver), defaultCurrency(defaultCurr), nextUserId(1), nextGroupId(1),
      nextExpenseId(1), nextSettlementId(1) {}

// User management
std::shared_ptr<User> SplitwiseApp::createUser(const std::string& name, const std::string& email,
                                              const std::string& phone, Currency preferredCurrency) {
    std::string userId = generateUserId();
    auto user = std::make_shared<User>(userId, name, email, phone, "", preferredCurrency);
    users[userId] = user;
    return user;
}

std::shared_ptr<User> SplitwiseApp::findUser(const std::string& userId) {
    auto it = users.find(userId);
    return (it != users.end()) ? it->second : nullptr;
}

bool SplitwiseApp::deleteUser(const std::string& userId) {
    auto it = users.find(userId);
    if (it != users.end()) {
        users.erase(it);
        return true;
    }
    return false;
}

std::vector<std::shared_ptr<User>> SplitwiseApp::getAllUsers() {
    std::vector<std::shared_ptr<User>> result;
    for (const auto& pair : users) {
        result.push_back(pair.second);
    }
    return result;
}

std::vector<std::shared_ptr<User>> SplitwiseApp::searchUsers(const std::string& query) {
    std::vector<std::shared_ptr<User>> result;
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
    
    for (const auto& pair : users) {
        std::string name = pair.second->getName();
        std::string email = pair.second->getEmail();
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        std::transform(email.begin(), email.end(), email.begin(), ::tolower);
        
        if (name.find(lowerQuery) != std::string::npos || 
            email.find(lowerQuery) != std::string::npos) {
            result.push_back(pair.second);
        }
    }
    return result;
}

// Group management
std::shared_ptr<Group> SplitwiseApp::createGroup(const std::string& name, const std::string& createdBy,
                                                GroupType type, const std::string& description) {
    validateUserExists(createdBy);
    std::string groupId = generateGroupId();
    auto group = std::make_shared<Group>(groupId, name, createdBy, type, description, defaultCurrency);
    groups[groupId] = group;
    
    // Add group to user's group list
    auto user = findUser(createdBy);
    if (user) {
        user->addGroup(groupId);
    }
    
    return group;
}

std::shared_ptr<Group> SplitwiseApp::findGroup(const std::string& groupId) {
    auto it = groups.find(groupId);
    return (it != groups.end()) ? it->second : nullptr;
}

bool SplitwiseApp::deleteGroup(const std::string& groupId) {
    auto it = groups.find(groupId);
    if (it != groups.end()) {
        // Remove group from all users
        for (auto& userPair : users) {
            userPair.second->removeGroup(groupId);
        }
        groups.erase(it);
        return true;
    }
    return false;
}

std::vector<std::shared_ptr<Group>> SplitwiseApp::getAllGroups() {
    std::vector<std::shared_ptr<Group>> result;
    for (const auto& pair : groups) {
        result.push_back(pair.second);
    }
    return result;
}

std::vector<std::shared_ptr<Group>> SplitwiseApp::getGroupsForUser(const std::string& userId) {
    std::vector<std::shared_ptr<Group>> result;
    auto user = findUser(userId);
    if (user) {
        for (const auto& groupId : user->getGroupIds()) {
            auto group = findGroup(groupId);
            if (group) {
                result.push_back(group);
            }
        }
    }
    return result;
}

bool SplitwiseApp::addUserToGroup(const std::string& userId, const std::string& groupId) {
    validateUserExists(userId);
    validateGroupExists(groupId);
    
    auto user = findUser(userId);
    auto group = findGroup(groupId);
    
    if (user && group) {
        group->addMember(userId);
        user->addGroup(groupId);
        return true;
    }
    return false;
}

bool SplitwiseApp::removeUserFromGroup(const std::string& userId, const std::string& groupId) {
    auto user = findUser(userId);
    auto group = findGroup(groupId);
    
    if (user && group) {
        group->removeMember(userId);
        user->removeGroup(groupId);
        return true;
    }
    return false;
}

// Expense management
std::shared_ptr<Expense> SplitwiseApp::createExpense(const std::string& description, double amount,
                                                    const std::string& paidBy, const std::string& groupId,
                                                    ExpenseCategory category) {
    validateUserExists(paidBy);
    validateGroupExists(groupId);
    
    if (!isUserInGroup(paidBy, groupId)) {
        throw std::runtime_error("User is not a member of the group");
    }
    
    std::string expenseId = generateExpenseId();
    auto expense = std::make_shared<Expense>(expenseId, description, amount, paidBy, groupId, category);
    expenses[expenseId] = expense;
    
    // Add expense to group
    auto group = findGroup(groupId);
    if (group) {
        group->addExpense(expenseId);
    }
    
    return expense;
}

std::shared_ptr<Expense> SplitwiseApp::findExpense(const std::string& expenseId) {
    auto it = expenses.find(expenseId);
    return (it != expenses.end()) ? it->second : nullptr;
}

bool SplitwiseApp::deleteExpense(const std::string& expenseId) {
    auto it = expenses.find(expenseId);
    if (it != expenses.end()) {
        // Remove from group
        auto group = findGroup(it->second->getGroupId());
        if (group) {
            group->removeExpense(expenseId);
        }
        expenses.erase(it);
        return true;
    }
    return false;
}

std::vector<std::shared_ptr<Expense>> SplitwiseApp::getExpensesForGroup(const std::string& groupId) {
    std::vector<std::shared_ptr<Expense>> result;
    for (const auto& pair : expenses) {
        if (pair.second->getGroupId() == groupId) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Expense>> SplitwiseApp::getExpensesForUser(const std::string& userId) {
    std::vector<std::shared_ptr<Expense>> result;
    for (const auto& pair : expenses) {
        if (pair.second->getPaidBy() == userId || pair.second->hasUserSplit(userId)) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Expense>> SplitwiseApp::getExpensesByCategory(const std::string& groupId, ExpenseCategory category) {
    std::vector<std::shared_ptr<Expense>> result;
    for (const auto& pair : expenses) {
        if (pair.second->getGroupId() == groupId && pair.second->getCategory() == category) {
            result.push_back(pair.second);
        }
    }
    return result;
}

// Settlement management
std::shared_ptr<Settlement> SplitwiseApp::createSettlement(const std::string& fromUserId,
                                                          const std::string& toUserId, const std::string& groupId,
                                                          double amount, SettlementMethod method) {
    validateUserExists(fromUserId);
    validateUserExists(toUserId);
    validateGroupExists(groupId);
    
    std::string settlementId = generateSettlementId();
    auto settlement = std::make_shared<Settlement>(settlementId, fromUserId, toUserId, groupId, amount, defaultCurrency, method);
    settlements[settlementId] = settlement;
    
    return settlement;
}

std::shared_ptr<Settlement> SplitwiseApp::findSettlement(const std::string& settlementId) {
    auto it = settlements.find(settlementId);
    return (it != settlements.end()) ? it->second : nullptr;
}

bool SplitwiseApp::deleteSettlement(const std::string& settlementId) {
    auto it = settlements.find(settlementId);
    if (it != settlements.end()) {
        settlements.erase(it);
        return true;
    }
    return false;
}

std::vector<std::shared_ptr<Settlement>> SplitwiseApp::getSettlementsForGroup(const std::string& groupId) {
    std::vector<std::shared_ptr<Settlement>> result;
    for (const auto& pair : settlements) {
        if (pair.second->getGroupId() == groupId) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Settlement>> SplitwiseApp::getSettlementsForUser(const std::string& userId) {
    std::vector<std::shared_ptr<Settlement>> result;
    for (const auto& pair : settlements) {
        if (pair.second->getFromUserId() == userId || pair.second->getToUserId() == userId) {
            result.push_back(pair.second);
        }
    }
    return result;
}

bool SplitwiseApp::completeSettlement(const std::string& settlementId) {
    auto settlement = findSettlement(settlementId);
    if (settlement && settlement->canBeCompleted()) {
        settlement->markAsCompleted();
        return true;
    }
    return false;
}

// Balance and debt management
void SplitwiseApp::calculateBalances(const std::string& groupId) {
    auto group = findGroup(groupId);
    if (!group) return;
    
    // Reset all balances
    group->resetAllBalances();
    
    // Calculate balances from expenses
    for (const auto& pair : expenses) {
        auto expense = pair.second;
        if (expense->getGroupId() == groupId) {
            // Add the full amount to the person who paid
            group->updateMemberBalance(expense->getPaidBy(), expense->getAmount());
            
            // Subtract each person's share
            for (const auto& split : expense->getSplits()) {
                group->updateMemberBalance(split.userId, -split.amount);
            }
        }
    }
    
    // Apply settlements
    for (const auto& pair : settlements) {
        auto settlement = pair.second;
        if (settlement->getGroupId() == groupId && settlement->isCompleted()) {
            group->updateMemberBalance(settlement->getFromUserId(), -settlement->getAmount());
            group->updateMemberBalance(settlement->getToUserId(), settlement->getAmount());
        }
    }
}

std::vector<DebtSummary> SplitwiseApp::getDebtSummary(const std::string& groupId) {
    calculateBalances(groupId);
    auto group = findGroup(groupId);
    if (!group) return {};
    
    std::vector<DebtSummary> debts;
    auto balances = group->getMemberBalances();
    
    for (const auto& balance : balances) {
        if (balance.second < 0) { // Debtor
            for (const auto& otherBalance : balances) {
                if (otherBalance.second > 0) { // Creditor
                    double debtAmount = std::min(-balance.second, otherBalance.second);
                    if (debtAmount > 0.01) { // Avoid tiny amounts
                        debts.emplace_back(balance.first, otherBalance.first, debtAmount, group->getCurrency());
                    }
                }
            }
        }
    }
    
    return debts;
}

std::vector<DebtSummary> SplitwiseApp::getDebtSummaryForUser(const std::string& userId) {
    std::vector<DebtSummary> allDebts;
    auto userGroups = getGroupsForUser(userId);
    
    for (const auto& group : userGroups) {
        auto groupDebts = getDebtSummary(group->getGroupId());
        for (const auto& debt : groupDebts) {
            if (debt.fromUserId == userId || debt.toUserId == userId) {
                allDebts.push_back(debt);
            }
        }
    }
    
    return allDebts;
}

double SplitwiseApp::getUserBalance(const std::string& userId, const std::string& groupId) {
    auto group = findGroup(groupId);
    if (!group) return 0.0;
    
    calculateBalances(groupId);
    return group->getMemberBalance(userId);
}

double SplitwiseApp::getTotalUserBalance(const std::string& userId) {
    double total = 0.0;
    auto userGroups = getGroupsForUser(userId);
    
    for (const auto& group : userGroups) {
        total += getUserBalance(userId, group->getGroupId());
    }
    
    return total;
}

// Reporting and analytics
GroupSummary SplitwiseApp::getGroupSummary(const std::string& groupId) {
    auto group = findGroup(groupId);
    if (!group) return GroupSummary("", "", 0, 0, 0, 0, 0);
    
    double totalExpenses = 0.0;
    double totalSettlements = 0.0;
    
    // Calculate total expenses
    for (const auto& pair : expenses) {
        if (pair.second->getGroupId() == groupId) {
            totalExpenses += pair.second->getAmount();
        }
    }
    
    // Calculate total settlements
    for (const auto& pair : settlements) {
        if (pair.second->getGroupId() == groupId && pair.second->isCompleted()) {
            totalSettlements += pair.second->getAmount();
        }
    }
    
    calculateBalances(groupId);
    double netBalance = group->getTotalGroupBalance();
    
    return GroupSummary(groupId, group->getName(), totalExpenses, totalSettlements, 
                       netBalance, group->getMemberCount(), group->getExpenseCount());
}

std::vector<GroupSummary> SplitwiseApp::getAllGroupSummaries() {
    std::vector<GroupSummary> summaries;
    for (const auto& pair : groups) {
        summaries.push_back(getGroupSummary(pair.first));
    }
    return summaries;
}

std::vector<std::shared_ptr<Expense>> SplitwiseApp::getTopExpenses(const std::string& groupId, int limit) {
    auto groupExpenses = getExpensesForGroup(groupId);
    std::sort(groupExpenses.begin(), groupExpenses.end(),
              [](const std::shared_ptr<Expense>& a, const std::shared_ptr<Expense>& b) {
                  return a->getAmount() > b->getAmount();
              });
    
    if (groupExpenses.size() > static_cast<size_t>(limit)) {
        groupExpenses.resize(limit);
    }
    
    return groupExpenses;
}

std::unordered_map<ExpenseCategory, double> SplitwiseApp::getExpenseBreakdown(const std::string& groupId) {
    std::unordered_map<ExpenseCategory, double> breakdown;
    auto groupExpenses = getExpensesForGroup(groupId);
    
    for (const auto& expense : groupExpenses) {
        breakdown[expense->getCategory()] += expense->getAmount();
    }
    
    return breakdown;
}

std::vector<std::pair<std::string, double>> SplitwiseApp::getTopSpenders(const std::string& groupId, int limit) {
    std::unordered_map<std::string, double> spending;
    auto groupExpenses = getExpensesForGroup(groupId);
    
    for (const auto& expense : groupExpenses) {
        spending[expense->getPaidBy()] += expense->getAmount();
    }
    
    std::vector<std::pair<std::string, double>> result;
    for (const auto& pair : spending) {
        result.push_back(pair);
    }
    
    std::sort(result.begin(), result.end(),
              [](const std::pair<std::string, double>& a, const std::pair<std::string, double>& b) {
                  return a.second > b.second;
              });
    
    if (result.size() > static_cast<size_t>(limit)) {
        result.resize(limit);
    }
    
    return result;
}

// Utility methods
std::string SplitwiseApp::generateUserId() {
    return "U" + std::to_string(nextUserId++);
}

std::string SplitwiseApp::generateGroupId() {
    return "G" + std::to_string(nextGroupId++);
}

std::string SplitwiseApp::generateExpenseId() {
    return "E" + std::to_string(nextExpenseId++);
}

std::string SplitwiseApp::generateSettlementId() {
    return "S" + std::to_string(nextSettlementId++);
}

std::string SplitwiseApp::getCurrentTimestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void SplitwiseApp::validateUserExists(const std::string& userId) {
    if (!findUser(userId)) {
        throw std::runtime_error("User not found: " + userId);
    }
}

void SplitwiseApp::validateGroupExists(const std::string& groupId) {
    if (!findGroup(groupId)) {
        throw std::runtime_error("Group not found: " + groupId);
    }
}

void SplitwiseApp::validateExpenseExists(const std::string& expenseId) {
    if (!findExpense(expenseId)) {
        throw std::runtime_error("Expense not found: " + expenseId);
    }
}

void SplitwiseApp::validateSettlementExists(const std::string& settlementId) {
    if (!findSettlement(settlementId)) {
        throw std::runtime_error("Settlement not found: " + settlementId);
    }
}

bool SplitwiseApp::isUserInGroup(const std::string& userId, const std::string& groupId) {
    auto group = findGroup(groupId);
    return group ? group->hasMember(userId) : false;
} 