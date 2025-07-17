#pragma once

#include <string>
#include <chrono>

enum class TransactionType {
    BORROW,
    RETURN,
    RENEW,
    RESERVE,
    CANCEL_RESERVATION
};

enum class TransactionStatus {
    PENDING,
    COMPLETED,
    CANCELLED,
    OVERDUE
};

class Transaction {
private:
    std::string transactionId;
    std::string memberId;
    std::string bookIsbn;
    std::string librarianId;
    TransactionType type;
    TransactionStatus status;
    std::chrono::system_clock::time_point transactionDate;
    std::chrono::system_clock::time_point dueDate;
    std::chrono::system_clock::time_point returnDate;
    double fineAmount;
    std::string notes;

public:
    Transaction(const std::string& transactionId, const std::string& memberId,
               const std::string& bookIsbn, const std::string& librarianId,
               TransactionType type);

    // Getters
    std::string getTransactionId() const { return transactionId; }
    std::string getMemberId() const { return memberId; }
    std::string getBookIsbn() const { return bookIsbn; }
    std::string getLibrarianId() const { return librarianId; }
    TransactionType getType() const { return type; }
    TransactionStatus getStatus() const { return status; }
    std::chrono::system_clock::time_point getTransactionDate() const { return transactionDate; }
    std::chrono::system_clock::time_point getDueDate() const { return dueDate; }
    std::chrono::system_clock::time_point getReturnDate() const { return returnDate; }
    double getFineAmount() const { return fineAmount; }
    std::string getNotes() const { return notes; }

    // Setters
    void setStatus(TransactionStatus newStatus) { status = newStatus; }
    void setDueDate(const std::chrono::system_clock::time_point& date) { dueDate = date; }
    void setReturnDate(const std::chrono::system_clock::time_point& date) { returnDate = date; }
    void setFineAmount(double amount) { fineAmount = amount; }
    void setNotes(const std::string& note) { notes = note; }

    // Utility methods
    bool isOverdue() const;
    int getDaysOverdue() const;
    double calculateFine(double dailyRate = 1.0) const;
    std::string getTypeString() const;
    std::string getStatusString() const;
}; 