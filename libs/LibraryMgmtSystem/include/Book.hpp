#pragma once

#include <string>

enum class BookStatus {
    AVAILABLE,
    BORROWED,
    RESERVED,
    MAINTENANCE
};

enum class BookCategory {
    FICTION,
    NON_FICTION,
    SCIENCE,
    TECHNOLOGY,
    HISTORY,
    PHILOSOPHY,
    LITERATURE,
    CHILDREN,
    ACADEMIC,
    OTHER
};

class Book {
private:
    std::string isbn;
    std::string title;
    std::string author;
    std::string publisher;
    int publicationYear;
    BookCategory category;
    BookStatus status;
    int totalCopies;
    int availableCopies;
    std::string location; // Shelf location
    double price;
    std::string description;

public:
    Book(const std::string& isbn, const std::string& title, const std::string& author,
         const std::string& publisher, int publicationYear, BookCategory category,
         int totalCopies = 1, const std::string& location = "", double price = 0.0,
         const std::string& description = "");

    // Getters
    std::string getIsbn() const { return isbn; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getPublisher() const { return publisher; }
    int getPublicationYear() const { return publicationYear; }
    BookCategory getCategory() const { return category; }
    BookStatus getStatus() const { return status; }
    int getTotalCopies() const { return totalCopies; }
    int getAvailableCopies() const { return availableCopies; }
    std::string getLocation() const { return location; }
    double getPrice() const { return price; }
    std::string getDescription() const { return description; }

    // Setters
    void setStatus(BookStatus newStatus) { status = newStatus; }
    void setAvailableCopies(int copies) { availableCopies = copies; }
    void setLocation(const std::string& loc) { location = loc; }
    void setPrice(double newPrice) { price = newPrice; }
    void setDescription(const std::string& desc) { description = desc; }

    // Utility methods
    bool isAvailable() const { return availableCopies > 0 && status == BookStatus::AVAILABLE; }
    void incrementCopies() { totalCopies++; availableCopies++; }
    void decrementCopies() { 
        if (totalCopies > 0) totalCopies--; 
        if (availableCopies > 0) availableCopies--; 
    }
    std::string getCategoryString() const;
    std::string getStatusString() const;
}; 