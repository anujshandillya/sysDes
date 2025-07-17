# Library Management System

A comprehensive C++ implementation of a Library Management System designed for educational institutions, public libraries, and research facilities. This system provides complete book management, member tracking, transaction processing, and administrative capabilities.

## 🏗️ System Architecture

The Library Management System is built using object-oriented design principles with the following core components:

### Core Classes

1. **`Book`** - Manages book information and availability
2. **`Member`** - Handles library members with different types and privileges
3. **`Librarian`** - Represents library staff with role-based permissions
4. **`Transaction`** - Tracks all book borrowing/returning activities
5. **`Reservation`** - Manages book reservations for unavailable books
6. **`Library`** - Main orchestrator class that manages all operations

## 📁 Project Structure

```
libs/LibraryMgmtSystem/
├── include/
│   ├── Book.hpp              # Book class definition
│   ├── Member.hpp            # Member class definition
│   ├── Librarian.hpp         # Librarian class definition
│   ├── Transaction.hpp       # Transaction class definition
│   ├── Reservation.hpp       # Reservation class definition
│   └── Library.hpp           # Main Library class definition
├── src/
│   ├── Book.cpp              # Book class implementation
│   ├── Member.cpp            # Member class implementation
│   ├── Librarian.cpp         # Librarian class implementation
│   ├── Transaction.cpp       # Transaction class implementation
│   ├── Reservation.cpp       # Reservation class implementation
│   └── Library.cpp           # Main Library class implementation
└── README.md                 # This file
```

## 🚀 Features

### Book Management
- **Add/Remove Books**: Complete book catalog management
- **Search Functionality**: Search by title, author, or ISBN
- **Category Classification**: Fiction, Non-Fiction, Science, Technology, etc.
- **Availability Tracking**: Real-time book availability status
- **Multiple Copies**: Support for multiple copies of the same book
- **Location Tracking**: Shelf location management

### Member Management
- **Member Types**: Student, Faculty, Staff, Guest, Premium
- **Borrowing Limits**: Different limits based on member type
- **Fine Tracking**: Automatic fine calculation and management
- **Status Management**: Active, Suspended, Expired, Blacklisted
- **Registration System**: Member registration with expiry dates

### Transaction System
- **Borrow/Return**: Complete transaction tracking
- **Renewal**: Book renewal functionality
- **Overdue Detection**: Automatic overdue calculation
- **Fine Calculation**: Daily fine rate application
- **Audit Trail**: Complete transaction history

### Reservation System
- **Book Reservations**: Reserve unavailable books
- **Expiry Management**: Automatic reservation expiry
- **Status Tracking**: Pending, Ready, Cancelled, Expired
- **Queue Management**: First-come-first-served reservations

### Librarian Management
- **Role-Based Access**: Junior, Senior, Manager, Admin roles
- **Permission System**: Different capabilities per role
- **Department Management**: Librarian department assignment
- **Activity Tracking**: Transaction attribution to librarians

### Reporting & Analytics
- **Popular Books**: Most borrowed books tracking
- **Top Borrowers**: Member borrowing statistics
- **Overdue Reports**: Overdue book management
- **Fine Reports**: Fine collection tracking
- **Transaction History**: Complete audit trails

## 🛠️ Implementation Details

### Design Patterns Used

1. **Strategy Pattern**: Different member types with varying borrowing limits
2. **Observer Pattern**: Transaction tracking for fine calculations
3. **Factory Pattern**: ID generation for transactions and reservations
4. **Singleton-like**: Central Library class managing all operations

### Data Structures

- **Unordered Maps**: For O(1) lookups of books, members, and librarians
- **Vectors**: For transaction and reservation history
- **Smart Pointers**: Memory management using `std::shared_ptr`
- **Chrono Library**: Time-based operations and date management

### Key Algorithms

- **Search Algorithm**: Case-insensitive string matching
- **Fine Calculation**: Daily rate × overdue days
- **Availability Check**: Real-time book status validation
- **Transaction Validation**: Multi-step validation process

## 📋 Usage Examples

### Basic Library Operations

```cpp
#include "Library.hpp"
#include <memory>

int main() {
    // Create library instance
    auto library = std::make_shared<Library>("LIB001", "Central Library", 
                                            "123 Main St", "555-0123", "lib@example.com");
    
    // Add books
    auto book1 = std::make_shared<Book>("978-0-7475-3269-9", "Harry Potter and the Philosopher's Stone",
                                       "J.K. Rowling", "Bloomsbury", 1997, BookCategory::FICTION);
    library->addBook(book1);
    
    // Add members
    auto member1 = std::make_shared<Member>("M001", "John Doe", "john@example.com",
                                           "555-0001", "456 Oak St", MemberType::STUDENT);
    library->addMember(member1);
    
    // Add librarians
    auto librarian1 = std::make_shared<Librarian>("L001", "Jane Smith", "jane@library.com",
                                                 "555-1001", LibrarianRole::SENIOR, "Circulation");
    library->addLibrarian(librarian1);
    
    // Borrow a book
    auto transaction = library->borrowBook("M001", "978-0-7475-3269-9", "L001");
    
    // Return a book
    library->returnBook("M001", "978-0-7475-3269-9", "L001");
    
    return 0;
}
```

### Advanced Operations

```cpp
// Search for books
auto results = library->searchBooks("Harry Potter");

// Get books by category
auto fictionBooks = library->getBooksByCategory(BookCategory::FICTION);

// Check member status
auto member = library->findMember("M001");
if (member && member->canBorrowBook()) {
    // Member can borrow books
}

// Calculate fines
double fine = library->calculateFine("M001");

// Get overdue transactions
auto overdue = library->getOverdueTransactions();

// Reserve a book
auto reservation = library->reserveBook("M001", "978-0-7475-3269-9");
```

## 🔧 Configuration

The Library Management System supports various configuration options:

```cpp
// Set borrowing limits
library->setMaxBooksPerMember(10);

// Set loan period
library->setLoanPeriodDays(21);

// Set fine rate
library->setDailyFineRate(0.50);

// Set reservation expiry
library->setReservationExpiryDays(5);
```

## 📊 Member Types and Limits

| Member Type | Max Books | Loan Period | Fine Rate |
|-------------|-----------|-------------|-----------|
| Student     | 5         | 14 days     | $1.00/day |
| Faculty     | 10        | 30 days     | $0.50/day |
| Staff       | 8         | 21 days     | $0.75/day |
| Guest       | 3         | 7 days      | $2.00/day |
| Premium     | 15        | 45 days     | $0.25/day |

## 🔐 Role-Based Permissions

| Role    | Manage Books | Manage Members | Manage Fines |
|---------|--------------|----------------|--------------|
| Junior  | ✅           | ❌             | ❌           |
| Senior  | ✅           | ✅             | ✅           |
| Manager | ✅           | ✅             | ✅           |
| Admin   | ✅           | ✅             | ✅           |

## 🧪 Testing

To test the Library Management System:

1. **Compile the code** using your preferred C++ compiler
2. **Create test instances** of all classes
3. **Perform operations** like borrowing, returning, and reserving books
4. **Verify fine calculations** and overdue detection
5. **Test edge cases** like invalid operations and error conditions

## 🔮 Future Enhancements

Potential improvements for the Library Management System:

1. **Database Integration**: Persistent storage using SQL/NoSQL databases
2. **Web Interface**: REST API and web-based user interface
3. **Email Notifications**: Automated overdue and reservation notifications
4. **Barcode Integration**: Physical book tracking with barcode scanners
5. **Advanced Analytics**: Machine learning for book recommendations
6. **Multi-branch Support**: Support for multiple library locations
7. **E-book Integration**: Digital book management capabilities
8. **Payment Processing**: Online fine payment integration

## 📝 Requirements

- **C++17** or later
- **Standard Library**: `<string>`, `<vector>`, `<unordered_map>`, `<memory>`, `<chrono>`
- **Compiler**: GCC 7+, Clang 5+, or MSVC 2017+

## 🤝 Contributing

To contribute to this project:

1. Fork the repository
2. Create a feature branch
3. Implement your changes
4. Add appropriate tests
5. Submit a pull request

## 📄 License

This project is open source and available under the MIT License.

## 👥 Authors

- **System Design Implementation**: Library Management System Team
- **C++ Implementation**: Based on system design principles

## 📞 Support

For questions, issues, or contributions, please:

1. Check the existing documentation
2. Review the code examples
3. Create an issue with detailed information
4. Contact the development team

---

**Note**: This Library Management System is designed for educational purposes and demonstrates object-oriented design principles, system architecture patterns, and C++ best practices. For production use, additional security, performance, and scalability considerations should be implemented. 