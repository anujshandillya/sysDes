#include "../include/Expense.hpp"
#include <algorithm>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdexcept>

Expense::Expense(const std::string& expenseId, const std::string& description, double amount,
                 const std::string& paidBy, const std::string& groupId,
                 ExpenseCategory category, PaymentMethod paymentMethod, const std::string& currency)
    : expenseId(expenseId), description(description), amount(amount), paidBy(paidBy), groupId(groupId),
      category(category), paymentMethod(paymentMethod), currency(currency), isSettled(false) {
    
    // Set creation timestamp
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    createdAt = oss.str();
    updatedAt = createdAt;
    date = createdAt;
}

void Expense::addSplit(const SplitDetail& split) {
    // Remove existing split for this user if exists
    removeSplit(split.userId);
    splits.push_back(split);
}

void Expense::removeSplit(const std::string& userId) {
    splits.erase(std::remove_if(splits.begin(), splits.end(),
                               [&userId](const SplitDetail& split) { return split.userId == userId; }),
                 splits.end());
}

void Expense::updateSplit(const std::string& userId, double amount, double percentage, int shares) {
    auto split = getSplitForUser(userId);
    if (split) {
        split->amount = amount;
        split->percentage = percentage;
        split->shares = shares;
    }
}

SplitDetail* Expense::getSplitForUser(const std::string& userId) {
    for (auto& split : splits) {
        if (split.userId == userId) {
            return &split;
        }
    }
    return nullptr;
}

const SplitDetail* Expense::getSplitForUser(const std::string& userId) const {
    for (const auto& split : splits) {
        if (split.userId == userId) {
            return &split;
        }
    }
    return nullptr;
}

double Expense::getAmountForUser(const std::string& userId) const {
    const auto* split = getSplitForUser(userId);
    return split ? split->amount : 0.0;
}

bool Expense::hasUserSplit(const std::string& userId) const {
    return getSplitForUser(userId) != nullptr;
}

void Expense::splitEqually(const std::vector<std::string>& userIds) {
    if (userIds.empty()) return;
    
    splits.clear();
    double amountPerPerson = amount / userIds.size();
    
    for (const auto& userId : userIds) {
        splits.emplace_back(userId, amountPerPerson, 100.0 / userIds.size(), 1, false);
    }
}

void Expense::splitByPercentage(const std::vector<std::string>& userIds, const std::vector<double>& percentages) {
    if (userIds.size() != percentages.size()) {
        throw std::invalid_argument("User IDs and percentages must have the same size");
    }
    
    splits.clear();
    for (size_t i = 0; i < userIds.size(); ++i) {
        double userAmount = amount * (percentages[i] / 100.0);
        splits.emplace_back(userIds[i], userAmount, percentages[i], 1, false);
    }
}

void Expense::splitByShares(const std::vector<std::string>& userIds, const std::vector<int>& shares) {
    if (userIds.size() != shares.size()) {
        throw std::invalid_argument("User IDs and shares must have the same size");
    }
    
    int totalShares = 0;
    for (int share : shares) {
        totalShares += share;
    }
    
    splits.clear();
    for (size_t i = 0; i < userIds.size(); ++i) {
        double userAmount = amount * (static_cast<double>(shares[i]) / totalShares);
        splits.emplace_back(userIds[i], userAmount, 0.0, shares[i], false);
    }
}

void Expense::splitCustom(const std::vector<SplitDetail>& customSplits) {
    splits = customSplits;
}

std::string Expense::getCategoryString() const {
    switch (category) {
        case ExpenseCategory::FOOD: return "Food";
        case ExpenseCategory::TRANSPORT: return "Transport";
        case ExpenseCategory::ACCOMMODATION: return "Accommodation";
        case ExpenseCategory::ENTERTAINMENT: return "Entertainment";
        case ExpenseCategory::SHOPPING: return "Shopping";
        case ExpenseCategory::UTILITIES: return "Utilities";
        case ExpenseCategory::HEALTH: return "Health";
        case ExpenseCategory::EDUCATION: return "Education";
        case ExpenseCategory::TRAVEL: return "Travel";
        case ExpenseCategory::OTHER: return "Other";
        default: return "Unknown";
    }
}

std::string Expense::getPaymentMethodString() const {
    switch (paymentMethod) {
        case PaymentMethod::CASH: return "Cash";
        case PaymentMethod::CARD: return "Card";
        case PaymentMethod::BANK_TRANSFER: return "Bank Transfer";
        case PaymentMethod::DIGITAL_WALLET: return "Digital Wallet";
        case PaymentMethod::CHECK: return "Check";
        case PaymentMethod::OTHER: return "Other";
        default: return "Unknown";
    }
}

double Expense::getTotalSplitAmount() const {
    double total = 0.0;
    for (const auto& split : splits) {
        total += split.amount;
    }
    return total;
}

bool Expense::isFullySplit() const {
    return std::abs(getTotalSplitAmount() - amount) < 0.01; // Allow small floating point differences
}

std::vector<std::string> Expense::getInvolvedUsers() const {
    std::vector<std::string> users;
    for (const auto& split : splits) {
        users.push_back(split.userId);
    }
    return users;
}

void Expense::markAsPaid(const std::string& userId) {
    auto split = getSplitForUser(userId);
    if (split) {
        split->isPaid = true;
    }
}

void Expense::markAsUnpaid(const std::string& userId) {
    auto split = getSplitForUser(userId);
    if (split) {
        split->isPaid = false;
    }
}

bool Expense::isUserPaid(const std::string& userId) const {
    const auto* split = getSplitForUser(userId);
    return split ? split->isPaid : false;
}

void Expense::updateTimestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    updatedAt = oss.str();
}

void Expense::validateSplits() const {
    if (!isFullySplit()) {
        throw std::runtime_error("Total split amount does not match expense amount");
    }
} 