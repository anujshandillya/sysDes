#include "../include/Librarian.hpp"

Librarian::Librarian(const std::string& employeeId, const std::string& name, 
                     const std::string& email, const std::string& phone, 
                     LibrarianRole role, const std::string& department)
    : employeeId(employeeId), name(name), email(email), phone(phone),
      role(role), department(department), isActive(true) {}

bool Librarian::canManageBooks() const {
    return isActive && (role == LibrarianRole::JUNIOR || 
                       role == LibrarianRole::SENIOR || 
                       role == LibrarianRole::MANAGER || 
                       role == LibrarianRole::ADMIN);
}

bool Librarian::canManageMembers() const {
    return isActive && (role == LibrarianRole::SENIOR || 
                       role == LibrarianRole::MANAGER || 
                       role == LibrarianRole::ADMIN);
}

bool Librarian::canManageFines() const {
    return isActive && (role == LibrarianRole::SENIOR || 
                       role == LibrarianRole::MANAGER || 
                       role == LibrarianRole::ADMIN);
}

std::string Librarian::getRoleString() const {
    switch (role) {
        case LibrarianRole::JUNIOR: return "Junior";
        case LibrarianRole::SENIOR: return "Senior";
        case LibrarianRole::MANAGER: return "Manager";
        case LibrarianRole::ADMIN: return "Admin";
        default: return "Unknown";
    }
} 