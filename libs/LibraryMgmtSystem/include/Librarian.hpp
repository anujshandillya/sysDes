#pragma once

#include <string>

enum class LibrarianRole {
    JUNIOR,
    SENIOR,
    MANAGER,
    ADMIN
};

class Librarian {
private:
    std::string employeeId;
    std::string name;
    std::string email;
    std::string phone;
    LibrarianRole role;
    std::string department;
    bool isActive;

public:
    Librarian(const std::string& employeeId, const std::string& name, 
              const std::string& email, const std::string& phone, 
              LibrarianRole role, const std::string& department);

    // Getters
    std::string getEmployeeId() const { return employeeId; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    LibrarianRole getRole() const { return role; }
    std::string getDepartment() const { return department; }
    bool getIsActive() const { return isActive; }

    // Setters
    void setRole(LibrarianRole newRole) { role = newRole; }
    void setDepartment(const std::string& dept) { department = dept; }
    void setIsActive(bool active) { isActive = active; }

    // Utility methods
    bool canManageBooks() const;
    bool canManageMembers() const;
    bool canManageFines() const;
    std::string getRoleString() const;
}; 