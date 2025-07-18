#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
#include <memory>
#include <chrono>

class URL;

class User {
private:
    std::string userId;
    std::string username;
    std::string email;
    std::string passwordHash;
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point lastLoginAt;
    bool isActive;
    std::vector<std::shared_ptr<URL>> createdUrls;

public:
    User(const std::string& username, const std::string& email, const std::string& passwordHash);
    
    // Getters
    std::string getUserId() const;
    std::string getUsername() const;
    std::string getEmail() const;
    std::string getPasswordHash() const;
    std::chrono::system_clock::time_point getCreatedAt() const;
    std::chrono::system_clock::time_point getLastLoginAt() const;
    bool getIsActive() const;
    std::vector<std::shared_ptr<URL>> getCreatedUrls() const;
    
    // Setters
    void setUsername(const std::string& username);
    void setEmail(const std::string& email);
    void setPasswordHash(const std::string& passwordHash);
    void setLastLoginAt(const std::chrono::system_clock::time_point& lastLoginAt);
    void setIsActive(bool isActive);
    
    // Methods
    void addCreatedUrl(std::shared_ptr<URL> url);
    void removeCreatedUrl(const std::string& shortCode);
    int getTotalUrlsCreated() const;
    int getTotalClicks() const;
    std::string toString() const;
};

#endif // USER_HPP 