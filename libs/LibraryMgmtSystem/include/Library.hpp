#pragma once

#include "Book.hpp"
#include "Member.hpp"
#include "Librarian.hpp"
#include "Transaction.hpp"
#include "Reservation.hpp"
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

class Library {
private:
    std::string libraryId;
    std::string name;
    std::string address;
    std::string phone;
    std::string email;
    
    // Collections
    std::unordered_map<std::string, std::shared_ptr<Book>> books;
    std::unordered_map<std::string, std::shared_ptr<Member>> members;
    std::unordered_map<std::string, std::shared_ptr<Librarian>> librarians;
    std::vector<std::shared_ptr<Transaction>> transactions;
    std::vector<std::shared_ptr<Reservation>> reservations;

    // Configuration
    int maxBooksPerMember;
    int loanPeriodDays;
    double dailyFineRate;
    int reservationExpiryDays;

public:
    Library(const std::string& libraryId, const std::string& name, 
            const std::string& address, const std::string& phone, 
            const std::string& email);

    // Book management
    bool addBook(const std::shared_ptr<Book>& book);
    bool removeBook(const std::string& isbn);
    std::shared_ptr<Book> findBook(const std::string& isbn);
    std::vector<std::shared_ptr<Book>> searchBooks(const std::string& query);
    std::vector<std::shared_ptr<Book>> getBooksByCategory(BookCategory category);
    std::vector<std::shared_ptr<Book>> getAvailableBooks();

    // Member management
    bool addMember(const std::shared_ptr<Member>& member);
    bool removeMember(const std::string& memberId);
    std::shared_ptr<Member> findMember(const std::string& memberId);
    std::vector<std::shared_ptr<Member>> getAllMembers();

    // Librarian management
    bool addLibrarian(const std::shared_ptr<Librarian>& librarian);
    bool removeLibrarian(const std::string& employeeId);
    std::shared_ptr<Librarian> findLibrarian(const std::string& employeeId);

    // Transaction management
    std::shared_ptr<Transaction> borrowBook(const std::string& memberId, 
                                           const std::string& bookIsbn, 
                                           const std::string& librarianId);
    bool returnBook(const std::string& memberId, const std::string& bookIsbn, 
                   const std::string& librarianId);
    bool renewBook(const std::string& memberId, const std::string& bookIsbn, 
                  const std::string& librarianId);
    std::vector<std::shared_ptr<Transaction>> getMemberTransactions(const std::string& memberId);
    std::vector<std::shared_ptr<Transaction>> getOverdueTransactions();

    // Reservation management
    std::shared_ptr<Reservation> reserveBook(const std::string& memberId, 
                                            const std::string& bookIsbn);
    bool cancelReservation(const std::string& reservationId);
    std::vector<std::shared_ptr<Reservation>> getMemberReservations(const std::string& memberId);

    // Fine management
    double calculateFine(const std::string& memberId);
    bool payFine(const std::string& memberId, double amount);
    std::vector<std::shared_ptr<Transaction>> getTransactionsWithFines();

    // Reporting
    std::vector<std::shared_ptr<Book>> getMostPopularBooks(int limit = 10);
    std::vector<std::shared_ptr<Member>> getTopBorrowers(int limit = 10);
    int getTotalBooks() const { return books.size(); }
    int getTotalMembers() const { return members.size(); }
    int getTotalTransactions() const { return transactions.size(); }

    // Configuration
    void setMaxBooksPerMember(int max) { maxBooksPerMember = max; }
    void setLoanPeriodDays(int days) { loanPeriodDays = days; }
    void setDailyFineRate(double rate) { dailyFineRate = rate; }
    void setReservationExpiryDays(int days) { reservationExpiryDays = days; }

    // Getters
    std::string getLibraryId() const { return libraryId; }
    std::string getName() const { return name; }
    std::string getAddress() const { return address; }
    std::string getPhone() const { return phone; }
    std::string getEmail() const { return email; }
    int getMaxBooksPerMember() const { return maxBooksPerMember; }
    int getLoanPeriodDays() const { return loanPeriodDays; }
    double getDailyFineRate() const { return dailyFineRate; }
    int getReservationExpiryDays() const { return reservationExpiryDays; }

private:
    std::string generateTransactionId();
    std::string generateReservationId();
    bool validateBorrowRequest(const std::string& memberId, const std::string& bookIsbn);
    void updateBookAvailability(const std::string& isbn, bool available);
    void processOverdueBooks();
}; 