#pragma once

#include <string>
#include <vector>
#include <chrono>

enum class MemberType {
    STUDENT,
    FACULTY,
    STAFF,
    GUEST,
    PREMIUM
};

enum class MemberStatus {
    ACTIVE,
    SUSPENDED,
    EXPIRED,
    BLACKLISTED
};

class Member {
private:
    std::string memberId;
    std::string name;
    std::string email;
    std::string phone;
    std::string address;
    MemberType type;
    MemberStatus status;
    std::chrono::system_clock::time_point registrationDate;
    std::chrono::system_clock::time_point expiryDate;
    int maxBooksAllowed;
    int currentBooksBorrowed;
    double fineBalance;
    std::vector<std::string> borrowedBooks; // ISBNs of borrowed books

public:
    Member(const std::string& memberId, const std::string& name, const std::string& email,
           const std::string& phone, const std::string& address, MemberType type);

    // Getters
    std::string getMemberId() const { return memberId; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    std::string getAddress() const { return address; }
    MemberType getType() const { return type; }
    MemberStatus getStatus() const { return status; }
    std::chrono::system_clock::time_point getRegistrationDate() const { return registrationDate; }
    std::chrono::system_clock::time_point getExpiryDate() const { return expiryDate; }
    int getMaxBooksAllowed() const { return maxBooksAllowed; }
    int getCurrentBooksBorrowed() const { return currentBooksBorrowed; }
    double getFineBalance() const { return fineBalance; }
    std::vector<std::string> getBorrowedBooks() const { return borrowedBooks; }

    // Setters
    void setStatus(MemberStatus newStatus) { status = newStatus; }
    void setExpiryDate(const std::chrono::system_clock::time_point& date) { expiryDate = date; }
    void setFineBalance(double balance) { fineBalance = balance; }

    // Utility methods
    bool canBorrowBook() const;
    bool hasOverdueBooks() const;
    void addBorrowedBook(const std::string& isbn);
    void removeBorrowedBook(const std::string& isbn);
    void addFine(double amount) { fineBalance += amount; }
    void payFine(double amount);
    bool isActive() const { return status == MemberStatus::ACTIVE; }
    std::string getTypeString() const;
    std::string getStatusString() const;
}; 