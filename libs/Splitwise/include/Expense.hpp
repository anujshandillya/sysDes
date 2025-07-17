#pragma once

#include <string>
#include <vector>

enum class ExpenseCategory {
    FOOD,
    TRANSPORT,
    ACCOMMODATION,
    ENTERTAINMENT,
    SHOPPING,
    UTILITIES,
    HEALTH,
    EDUCATION,
    TRAVEL,
    OTHER
};

enum class PaymentMethod {
    CASH,
    CARD,
    BANK_TRANSFER,
    DIGITAL_WALLET,
    CHECK,
    OTHER
};

struct SplitDetail {
    std::string userId;
    double amount;
    double percentage;
    int shares;
    bool isPaid;
    
    SplitDetail(const std::string& uid, double amt, double pct = 0.0, int shr = 1, bool paid = false)
        : userId(uid), amount(amt), percentage(pct), shares(shr), isPaid(paid) {}
};

class Expense {
private:
    std::string expenseId;
    std::string description;
    double amount;
    std::string paidBy;
    std::string groupId;
    ExpenseCategory category;
    PaymentMethod paymentMethod;
    std::string currency;
    std::string date;
    std::string createdAt;
    std::string updatedAt;
    std::vector<SplitDetail> splits;
    std::string receiptImage;
    std::string notes;
    bool isSettled;

public:
    Expense(const std::string& expenseId, const std::string& description, double amount,
            const std::string& paidBy, const std::string& groupId,
            ExpenseCategory category = ExpenseCategory::OTHER,
            PaymentMethod paymentMethod = PaymentMethod::CASH,
            const std::string& currency = "USD");

    // Getters
    std::string getExpenseId() const { return expenseId; }
    std::string getDescription() const { return description; }
    double getAmount() const { return amount; }
    std::string getPaidBy() const { return paidBy; }
    std::string getGroupId() const { return groupId; }
    ExpenseCategory getCategory() const { return category; }
    PaymentMethod getPaymentMethod() const { return paymentMethod; }
    std::string getCurrency() const { return currency; }
    std::string getDate() const { return date; }
    std::string getCreatedAt() const { return createdAt; }
    std::string getUpdatedAt() const { return updatedAt; }
    std::vector<SplitDetail> getSplits() const { return splits; }
    std::string getReceiptImage() const { return receiptImage; }
    std::string getNotes() const { return notes; }
    bool getIsSettled() const { return isSettled; }

    // Setters
    void setDescription(const std::string& desc) { description = desc; }
    void setAmount(double amt) { amount = amt; }
    void setCategory(ExpenseCategory cat) { category = cat; }
    void setPaymentMethod(PaymentMethod method) { paymentMethod = method; }
    void setCurrency(const std::string& curr) { currency = curr; }
    void setDate(const std::string& expenseDate) { date = expenseDate; }
    void setReceiptImage(const std::string& image) { receiptImage = image; }
    void setNotes(const std::string& expenseNotes) { notes = expenseNotes; }
    void setIsSettled(bool settled) { isSettled = settled; }

    // Split management
    void addSplit(const SplitDetail& split);
    void removeSplit(const std::string& userId);
    void updateSplit(const std::string& userId, double amount, double percentage = 0.0, int shares = 1);
    SplitDetail* getSplitForUser(const std::string& userId);
    const SplitDetail* getSplitForUser(const std::string& userId) const;
    double getAmountForUser(const std::string& userId) const;
    bool hasUserSplit(const std::string& userId) const;

    // Split calculation methods
    void splitEqually(const std::vector<std::string>& userIds);
    void splitByPercentage(const std::vector<std::string>& userIds, const std::vector<double>& percentages);
    void splitByShares(const std::vector<std::string>& userIds, const std::vector<int>& shares);
    void splitCustom(const std::vector<SplitDetail>& customSplits);

    // Utility methods
    std::string getCategoryString() const;
    std::string getPaymentMethodString() const;
    double getTotalSplitAmount() const;
    bool isFullySplit() const;
    std::vector<std::string> getInvolvedUsers() const;
    void markAsPaid(const std::string& userId);
    void markAsUnpaid(const std::string& userId);
    bool isUserPaid(const std::string& userId) const;
    void updateTimestamp();
    void validateSplits() const;
}; 