#include "../include/Book.hpp"

Book::Book(const std::string& isbn, const std::string& title, const std::string& author,
           const std::string& publisher, int publicationYear, BookCategory category,
           int totalCopies, const std::string& location, double price,
           const std::string& description)
    : isbn(isbn), title(title), author(author), publisher(publisher),
      publicationYear(publicationYear), category(category), status(BookStatus::AVAILABLE),
      totalCopies(totalCopies), availableCopies(totalCopies), location(location),
      price(price), description(description) {}

std::string Book::getCategoryString() const {
    switch (category) {
        case BookCategory::FICTION: return "Fiction";
        case BookCategory::NON_FICTION: return "Non-Fiction";
        case BookCategory::SCIENCE: return "Science";
        case BookCategory::TECHNOLOGY: return "Technology";
        case BookCategory::HISTORY: return "History";
        case BookCategory::PHILOSOPHY: return "Philosophy";
        case BookCategory::LITERATURE: return "Literature";
        case BookCategory::CHILDREN: return "Children";
        case BookCategory::ACADEMIC: return "Academic";
        case BookCategory::OTHER: return "Other";
        default: return "Unknown";
    }
}

std::string Book::getStatusString() const {
    switch (status) {
        case BookStatus::AVAILABLE: return "Available";
        case BookStatus::BORROWED: return "Borrowed";
        case BookStatus::RESERVED: return "Reserved";
        case BookStatus::MAINTENANCE: return "Maintenance";
        default: return "Unknown";
    }
} 