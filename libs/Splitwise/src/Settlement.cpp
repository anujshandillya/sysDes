#include "../include/Settlement.hpp"
#include <ctime>
#include <sstream>
#include <iomanip>

Settlement::Settlement(const std::string& settlementId, const std::string& fromUserId,
                       const std::string& toUserId, const std::string& groupId, double amount,
                       const std::string& currency, SettlementMethod method, const std::string& description)
    : settlementId(settlementId), fromUserId(fromUserId), toUserId(toUserId), groupId(groupId),
      amount(amount), currency(currency), status(SettlementStatus::PENDING), method(method),
      description(description) {
    
    // Set creation timestamp
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    createdAt = oss.str();
    date = createdAt;
}

void Settlement::markAsCompleted() {
    if (status == SettlementStatus::PENDING) {
        status = SettlementStatus::COMPLETED;
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        completedAt = oss.str();
    }
}

void Settlement::markAsCancelled() {
    if (status == SettlementStatus::PENDING) {
        status = SettlementStatus::CANCELLED;
    }
}

void Settlement::markAsExpired() {
    if (status == SettlementStatus::PENDING) {
        status = SettlementStatus::EXPIRED;
    }
}

std::string Settlement::getStatusString() const {
    switch (status) {
        case SettlementStatus::PENDING: return "Pending";
        case SettlementStatus::COMPLETED: return "Completed";
        case SettlementStatus::CANCELLED: return "Cancelled";
        case SettlementStatus::EXPIRED: return "Expired";
        default: return "Unknown";
    }
}

std::string Settlement::getMethodString() const {
    switch (method) {
        case SettlementMethod::CASH: return "Cash";
        case SettlementMethod::BANK_TRANSFER: return "Bank Transfer";
        case SettlementMethod::DIGITAL_WALLET: return "Digital Wallet";
        case SettlementMethod::CHECK: return "Check";
        case SettlementMethod::OTHER: return "Other";
        default: return "Unknown";
    }
}

bool Settlement::canBeCancelled() const {
    return status == SettlementStatus::PENDING;
}

bool Settlement::canBeCompleted() const {
    return status == SettlementStatus::PENDING;
}

void Settlement::updateTimestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    date = oss.str();
}

std::string Settlement::getFormattedAmount() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << amount;
    return currency + " " + oss.str();
} 