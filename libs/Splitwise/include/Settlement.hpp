#pragma once

#include <string>

enum class SettlementStatus {
    PENDING,
    COMPLETED,
    CANCELLED,
    EXPIRED
};

enum class SettlementMethod {
    CASH,
    BANK_TRANSFER,
    DIGITAL_WALLET,
    CHECK,
    OTHER
};

class Settlement {
private:
    std::string settlementId;
    std::string fromUserId;
    std::string toUserId;
    std::string groupId;
    double amount;
    std::string currency;
    SettlementStatus status;
    SettlementMethod method;
    std::string description;
    std::string date;
    std::string createdAt;
    std::string completedAt;
    std::string notes;
    std::string transactionReference;

public:
    Settlement(const std::string& settlementId, const std::string& fromUserId,
               const std::string& toUserId, const std::string& groupId, double amount,
               const std::string& currency = "USD", SettlementMethod method = SettlementMethod::CASH,
               const std::string& description = "");

    // Getters
    std::string getSettlementId() const { return settlementId; }
    std::string getFromUserId() const { return fromUserId; }
    std::string getToUserId() const { return toUserId; }
    std::string getGroupId() const { return groupId; }
    double getAmount() const { return amount; }
    std::string getCurrency() const { return currency; }
    SettlementStatus getStatus() const { return status; }
    SettlementMethod getMethod() const { return method; }
    std::string getDescription() const { return description; }
    std::string getDate() const { return date; }
    std::string getCreatedAt() const { return createdAt; }
    std::string getCompletedAt() const { return completedAt; }
    std::string getNotes() const { return notes; }
    std::string getTransactionReference() const { return transactionReference; }

    // Setters
    void setAmount(double amt) { amount = amt; }
    void setStatus(SettlementStatus newStatus) { status = newStatus; }
    void setMethod(SettlementMethod newMethod) { method = newMethod; }
    void setDescription(const std::string& desc) { description = desc; }
    void setDate(const std::string& settlementDate) { date = settlementDate; }
    void setNotes(const std::string& settlementNotes) { notes = settlementNotes; }
    void setTransactionReference(const std::string& ref) { transactionReference = ref; }

    // Status management
    void markAsCompleted();
    void markAsCancelled();
    void markAsExpired();
    bool isPending() const { return status == SettlementStatus::PENDING; }
    bool isCompleted() const { return status == SettlementStatus::COMPLETED; }
    bool isCancelled() const { return status == SettlementStatus::CANCELLED; }
    bool isExpired() const { return status == SettlementStatus::EXPIRED; }

    // Utility methods
    std::string getStatusString() const;
    std::string getMethodString() const;
    bool canBeCancelled() const;
    bool canBeCompleted() const;
    void updateTimestamp();
    std::string getFormattedAmount() const;
}; 