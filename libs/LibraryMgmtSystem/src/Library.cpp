#include "../include/Library.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>

Library::Library(const std::string& libraryId, const std::string& name, 
                 const std::string& address, const std::string& phone, 
                 const std::string& email)
    : libraryId(libraryId), name(name), address(address), phone(phone), email(email),
      maxBooksPerMember(5), loanPeriodDays(14), dailyFineRate(1.0), reservationExpiryDays(7) {}

bool Library::addBook(const std::shared_ptr<Book>& book) {
    if (books.find(book->getIsbn()) != books.end()) {
        return false; // Book already exists
    }
    books[book->getIsbn()] = book;
    return true;
}

bool Library::removeBook(const std::string& isbn) {
    auto it = books.find(isbn);
    if (it != books.end()) {
        books.erase(it);
        return true;
    }
    return false;
}

std::shared_ptr<Book> Library::findBook(const std::string& isbn) {
    auto it = books.find(isbn);
    return (it != books.end()) ? it->second : nullptr;
}

std::vector<std::shared_ptr<Book>> Library::searchBooks(const std::string& query) {
    std::vector<std::shared_ptr<Book>> results;
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
    
    for (const auto& pair : books) {
        const auto& book = pair.second;
        std::string title = book->getTitle();
        std::string author = book->getAuthor();
        std::transform(title.begin(), title.end(), title.begin(), ::tolower);
        std::transform(author.begin(), author.end(), author.begin(), ::tolower);
        
        if (title.find(lowerQuery) != std::string::npos || 
            author.find(lowerQuery) != std::string::npos) {
            results.push_back(book);
        }
    }
    return results;
}

std::vector<std::shared_ptr<Book>> Library::getBooksByCategory(BookCategory category) {
    std::vector<std::shared_ptr<Book>> results;
    for (const auto& pair : books) {
        if (pair.second->getCategory() == category) {
            results.push_back(pair.second);
        }
    }
    return results;
}

std::vector<std::shared_ptr<Book>> Library::getAvailableBooks() {
    std::vector<std::shared_ptr<Book>> results;
    for (const auto& pair : books) {
        if (pair.second->isAvailable()) {
            results.push_back(pair.second);
        }
    }
    return results;
}

bool Library::addMember(const std::shared_ptr<Member>& member) {
    if (members.find(member->getMemberId()) != members.end()) {
        return false; // Member already exists
    }
    members[member->getMemberId()] = member;
    return true;
}

bool Library::removeMember(const std::string& memberId) {
    auto it = members.find(memberId);
    if (it != members.end()) {
        members.erase(it);
        return true;
    }
    return false;
}

std::shared_ptr<Member> Library::findMember(const std::string& memberId) {
    auto it = members.find(memberId);
    return (it != members.end()) ? it->second : nullptr;
}

std::vector<std::shared_ptr<Member>> Library::getAllMembers() {
    std::vector<std::shared_ptr<Member>> results;
    for (const auto& pair : members) {
        results.push_back(pair.second);
    }
    return results;
}

bool Library::addLibrarian(const std::shared_ptr<Librarian>& librarian) {
    if (librarians.find(librarian->getEmployeeId()) != librarians.end()) {
        return false; // Librarian already exists
    }
    librarians[librarian->getEmployeeId()] = librarian;
    return true;
}

bool Library::removeLibrarian(const std::string& employeeId) {
    auto it = librarians.find(employeeId);
    if (it != librarians.end()) {
        librarians.erase(it);
        return true;
    }
    return false;
}

std::shared_ptr<Librarian> Library::findLibrarian(const std::string& employeeId) {
    auto it = librarians.find(employeeId);
    return (it != librarians.end()) ? it->second : nullptr;
}

std::shared_ptr<Transaction> Library::borrowBook(const std::string& memberId, 
                                                 const std::string& bookIsbn, 
                                                 const std::string& librarianId) {
    if (!validateBorrowRequest(memberId, bookIsbn)) {
        return nullptr;
    }
    
    auto transaction = std::make_shared<Transaction>(
        generateTransactionId(), memberId, bookIsbn, librarianId, TransactionType::BORROW
    );
    
    // Update book availability
    auto book = findBook(bookIsbn);
    if (book) {
        book->setAvailableCopies(book->getAvailableCopies() - 1);
        if (book->getAvailableCopies() == 0) {
            book->setStatus(BookStatus::BORROWED);
        }
    }
    
    // Update member's borrowed books
    auto member = findMember(memberId);
    if (member) {
        member->addBorrowedBook(bookIsbn);
    }
    
    transactions.push_back(transaction);
    return transaction;
}

bool Library::returnBook(const std::string& memberId, const std::string& bookIsbn, 
                         const std::string& librarianId) {
    auto member = findMember(memberId);
    auto book = findBook(bookIsbn);
    
    if (!member || !book) {
        return false;
    }
    
    // Find the borrow transaction
    for (auto& transaction : transactions) {
        if (transaction->getMemberId() == memberId && 
            transaction->getBookIsbn() == bookIsbn && 
            transaction->getType() == TransactionType::BORROW &&
            transaction->getStatus() != TransactionStatus::COMPLETED) {
            
            transaction->setStatus(TransactionStatus::COMPLETED);
            transaction->setReturnDate(std::chrono::system_clock::now());
            
            // Calculate and apply fines if overdue
            if (transaction->isOverdue()) {
                double fine = transaction->calculateFine(dailyFineRate);
                member->addFine(fine);
                transaction->setFineAmount(fine);
            }
            
            // Update book availability
            book->setAvailableCopies(book->getAvailableCopies() + 1);
            if (book->getStatus() == BookStatus::BORROWED) {
                book->setStatus(BookStatus::AVAILABLE);
            }
            
            // Update member's borrowed books
            member->removeBorrowedBook(bookIsbn);
            
            return true;
        }
    }
    
    return false;
}

bool Library::renewBook(const std::string& memberId, const std::string& bookIsbn, 
                        const std::string& librarianId) {
    auto member = findMember(memberId);
    if (!member || member->getFineBalance() > 0) {
        return false; // Cannot renew if member has fines
    }
    
    // Find the current borrow transaction
    for (auto& transaction : transactions) {
        if (transaction->getMemberId() == memberId && 
            transaction->getBookIsbn() == bookIsbn && 
            transaction->getType() == TransactionType::BORROW &&
            transaction->getStatus() != TransactionStatus::COMPLETED) {
            
            // Create a renew transaction
            auto renewTransaction = std::make_shared<Transaction>(
                generateTransactionId(), memberId, bookIsbn, librarianId, TransactionType::RENEW
            );
            
            // Extend the due date
            auto newDueDate = transaction->getDueDate() + std::chrono::hours(24 * loanPeriodDays);
            renewTransaction->setDueDate(newDueDate);
            
            transactions.push_back(renewTransaction);
            return true;
        }
    }
    
    return false;
}

std::vector<std::shared_ptr<Transaction>> Library::getMemberTransactions(const std::string& memberId) {
    std::vector<std::shared_ptr<Transaction>> results;
    for (const auto& transaction : transactions) {
        if (transaction->getMemberId() == memberId) {
            results.push_back(transaction);
        }
    }
    return results;
}

std::vector<std::shared_ptr<Transaction>> Library::getOverdueTransactions() {
    std::vector<std::shared_ptr<Transaction>> results;
    for (const auto& transaction : transactions) {
        if (transaction->isOverdue()) {
            results.push_back(transaction);
        }
    }
    return results;
}

std::shared_ptr<Reservation> Library::reserveBook(const std::string& memberId, 
                                                  const std::string& bookIsbn) {
    auto member = findMember(memberId);
    auto book = findBook(bookIsbn);
    
    if (!member || !book || !member->canBorrowBook()) {
        return nullptr;
    }
    
    // Check if book is available for reservation
    if (book->getStatus() != BookStatus::BORROWED) {
        return nullptr;
    }
    
    auto reservation = std::make_shared<Reservation>(
        generateReservationId(), memberId, bookIsbn
    );
    
    reservations.push_back(reservation);
    return reservation;
}

bool Library::cancelReservation(const std::string& reservationId) {
    for (auto it = reservations.begin(); it != reservations.end(); ++it) {
        if ((*it)->getReservationId() == reservationId) {
            (*it)->setStatus(ReservationStatus::CANCELLED);
            return true;
        }
    }
    return false;
}

std::vector<std::shared_ptr<Reservation>> Library::getMemberReservations(const std::string& memberId) {
    std::vector<std::shared_ptr<Reservation>> results;
    for (const auto& reservation : reservations) {
        if (reservation->getMemberId() == memberId) {
            results.push_back(reservation);
        }
    }
    return results;
}

double Library::calculateFine(const std::string& memberId) {
    auto member = findMember(memberId);
    if (!member) {
        return 0.0;
    }
    
    double totalFine = 0.0;
    for (const auto& transaction : transactions) {
        if (transaction->getMemberId() == memberId && 
            transaction->getType() == TransactionType::BORROW &&
            transaction->isOverdue()) {
            totalFine += transaction->calculateFine(dailyFineRate);
        }
    }
    
    return totalFine;
}

bool Library::payFine(const std::string& memberId, double amount) {
    auto member = findMember(memberId);
    if (!member) {
        return false;
    }
    
    member->payFine(amount);
    return true;
}

std::vector<std::shared_ptr<Transaction>> Library::getTransactionsWithFines() {
    std::vector<std::shared_ptr<Transaction>> results;
    for (const auto& transaction : transactions) {
        if (transaction->getFineAmount() > 0) {
            results.push_back(transaction);
        }
    }
    return results;
}

std::vector<std::shared_ptr<Book>> Library::getMostPopularBooks(int limit) {
    // This is a simplified implementation
    // In a real system, you'd track borrow counts
    std::vector<std::shared_ptr<Book>> results;
    for (const auto& pair : books) {
        results.push_back(pair.second);
        if (results.size() >= static_cast<size_t>(limit)) {
            break;
        }
    }
    return results;
}

std::vector<std::shared_ptr<Member>> Library::getTopBorrowers(int limit) {
    std::vector<std::shared_ptr<Member>> results;
    for (const auto& pair : members) {
        if (pair.second->getCurrentBooksBorrowed() > 0) {
            results.push_back(pair.second);
            if (results.size() >= static_cast<size_t>(limit)) {
                break;
            }
        }
    }
    return results;
}

std::string Library::generateTransactionId() {
    static int counter = 0;
    std::ostringstream oss;
    oss << "TXN" << std::setfill('0') << std::setw(6) << ++counter;
    return oss.str();
}

std::string Library::generateReservationId() {
    static int counter = 0;
    std::ostringstream oss;
    oss << "RES" << std::setfill('0') << std::setw(6) << ++counter;
    return oss.str();
}

bool Library::validateBorrowRequest(const std::string& memberId, const std::string& bookIsbn) {
    auto member = findMember(memberId);
    auto book = findBook(bookIsbn);
    
    if (!member || !book) {
        return false;
    }
    
    if (!member->canBorrowBook()) {
        return false;
    }
    
    if (!book->isAvailable()) {
        return false;
    }
    
    return true;
}

void Library::updateBookAvailability(const std::string& isbn, bool available) {
    auto book = findBook(isbn);
    if (book) {
        if (available) {
            book->setStatus(BookStatus::AVAILABLE);
        } else {
            book->setStatus(BookStatus::BORROWED);
        }
    }
}

void Library::processOverdueBooks() {
    for (auto& transaction : transactions) {
        if (transaction->isOverdue() && 
            transaction->getStatus() != TransactionStatus::COMPLETED) {
            transaction->setStatus(TransactionStatus::OVERDUE);
        }
    }
} 