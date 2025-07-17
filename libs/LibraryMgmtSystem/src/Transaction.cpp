#include "../include/Transaction.hpp"
#include <chrono>

Transaction::Transaction(const std::string& transactionId, const std::string& memberId,
                       const std::string& bookIsbn, const std::string& librarianId,
                       TransactionType type)
    : transactionId(transactionId), memberId(memberId), bookIsbn(bookIsbn),
      librarianId(librarianId), type(type), status(TransactionStatus::PENDING),
      transactionDate(std::chrono::system_clock::now()), fineAmount(0.0) {
    
    // Set due date to 14 days from transaction date for borrow transactions
    if (type == TransactionType::BORROW) {
        auto twoWeeks = std::chrono::hours(24 * 14);
        dueDate = transactionDate + twoWeeks;
    }
}

bool Transaction::isOverdue() const {
    if (type != TransactionType::BORROW || status == TransactionStatus::COMPLETED) {
        return false;
    }
    
    auto now = std::chrono::system_clock::now();
    return now > dueDate;
}

int Transaction::getDaysOverdue() const {
    if (!isOverdue()) {
        return 0;
    }
    
    auto now = std::chrono::system_clock::now();
    auto duration = now - dueDate;
    auto days = std::chrono::duration_cast<std::chrono::hours>(duration).count() / 24;
    return static_cast<int>(days);
}

double Transaction::calculateFine(double dailyRate) const {
    if (!isOverdue()) {
        return 0.0;
    }
    
    int daysOverdue = getDaysOverdue();
    return daysOverdue * dailyRate;
}

std::string Transaction::getTypeString() const {
    switch (type) {
        case TransactionType::BORROW: return "Borrow";
        case TransactionType::RETURN: return "Return";
        case TransactionType::RENEW: return "Renew";
        case TransactionType::RESERVE: return "Reserve";
        case TransactionType::CANCEL_RESERVATION: return "Cancel Reservation";
        default: return "Unknown";
    }
}

std::string Transaction::getStatusString() const {
    switch (status) {
        case TransactionStatus::PENDING: return "Pending";
        case TransactionStatus::COMPLETED: return "Completed";
        case TransactionStatus::CANCELLED: return "Cancelled";
        case TransactionStatus::OVERDUE: return "Overdue";
        default: return "Unknown";
    }
} 