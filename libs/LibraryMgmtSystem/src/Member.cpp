#include "../include/Member.hpp"
#include <chrono>
#include <algorithm>

Member::Member(const std::string& memberId, const std::string& name, const std::string& email,
               const std::string& phone, const std::string& address, MemberType type)
    : memberId(memberId), name(name), email(email), phone(phone), address(address),
      type(type), status(MemberStatus::ACTIVE), registrationDate(std::chrono::system_clock::now()),
      currentBooksBorrowed(0), fineBalance(0.0) {
    
    // Set expiry date to 1 year from registration
    auto now = std::chrono::system_clock::now();
    auto oneYear = std::chrono::hours(24 * 365);
    expiryDate = now + oneYear;
    
    // Set max books based on member type
    switch (type) {
        case MemberType::STUDENT:
            maxBooksAllowed = 5;
            break;
        case MemberType::FACULTY:
            maxBooksAllowed = 10;
            break;
        case MemberType::STAFF:
            maxBooksAllowed = 8;
            break;
        case MemberType::GUEST:
            maxBooksAllowed = 3;
            break;
        case MemberType::PREMIUM:
            maxBooksAllowed = 15;
            break;
    }
}

bool Member::canBorrowBook() const {
    return status == MemberStatus::ACTIVE && 
           currentBooksBorrowed < maxBooksAllowed && 
           fineBalance <= 10.0; // Allow borrowing if fine is $10 or less
}

bool Member::hasOverdueBooks() const {
    // This would need to be implemented with actual overdue checking logic
    return false; // Placeholder
}

void Member::addBorrowedBook(const std::string& isbn) {
    if (currentBooksBorrowed < maxBooksAllowed) {
        borrowedBooks.push_back(isbn);
        currentBooksBorrowed++;
    }
}

void Member::removeBorrowedBook(const std::string& isbn) {
    auto it = std::find(borrowedBooks.begin(), borrowedBooks.end(), isbn);
    if (it != borrowedBooks.end()) {
        borrowedBooks.erase(it);
        currentBooksBorrowed--;
    }
}

void Member::payFine(double amount) {
    if (amount <= fineBalance) {
        fineBalance -= amount;
    }
}

std::string Member::getTypeString() const {
    switch (type) {
        case MemberType::STUDENT: return "Student";
        case MemberType::FACULTY: return "Faculty";
        case MemberType::STAFF: return "Staff";
        case MemberType::GUEST: return "Guest";
        case MemberType::PREMIUM: return "Premium";
        default: return "Unknown";
    }
}

std::string Member::getStatusString() const {
    switch (status) {
        case MemberStatus::ACTIVE: return "Active";
        case MemberStatus::SUSPENDED: return "Suspended";
        case MemberStatus::EXPIRED: return "Expired";
        case MemberStatus::BLACKLISTED: return "Blacklisted";
        default: return "Unknown";
    }
}