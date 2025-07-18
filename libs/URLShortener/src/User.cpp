#include "../include/User.hpp"
#include "../include/URL.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

User::User(const std::string& username, const std::string& email, const std::string& passwordHash)
    : username(username), email(email), passwordHash(passwordHash),
      createdAt(std::chrono::system_clock::now()), isActive(true) {
    // Generate a simple user ID based on username and timestamp
    auto time_t_created = std::chrono::system_clock::to_time_t(createdAt);
    std::ostringstream oss;
    oss << "user_" << username << "_" << time_t_created;
    userId = oss.str();
    
    lastLoginAt = createdAt;
}

std::string User::getUserId() const {
    return userId;
}

std::string User::getUsername() const {
    return username;
}

std::string User::getEmail() const {
    return email;
}

std::string User::getPasswordHash() const {
    return passwordHash;
}

std::chrono::system_clock::time_point User::getCreatedAt() const {
    return createdAt;
}

std::chrono::system_clock::time_point User::getLastLoginAt() const {
    return lastLoginAt;
}

bool User::getIsActive() const {
    return isActive;
}

std::vector<std::shared_ptr<URL>> User::getCreatedUrls() const {
    return createdUrls;
}

void User::setUsername(const std::string& username) {
    this->username = username;
}

void User::setEmail(const std::string& email) {
    this->email = email;
}

void User::setPasswordHash(const std::string& passwordHash) {
    this->passwordHash = passwordHash;
}

void User::setLastLoginAt(const std::chrono::system_clock::time_point& lastLoginAt) {
    this->lastLoginAt = lastLoginAt;
}

void User::setIsActive(bool isActive) {
    this->isActive = isActive;
}

void User::addCreatedUrl(std::shared_ptr<URL> url) {
    // Check if URL already exists
    auto it = std::find_if(createdUrls.begin(), createdUrls.end(),
                          [&url](const std::shared_ptr<URL>& existingUrl) {
                              return existingUrl->getShortCode() == url->getShortCode();
                          });
    
    if (it == createdUrls.end()) {
        createdUrls.push_back(url);
    }
}

void User::removeCreatedUrl(const std::string& shortCode) {
    createdUrls.erase(
        std::remove_if(createdUrls.begin(), createdUrls.end(),
                      [&shortCode](const std::shared_ptr<URL>& url) {
                          return url->getShortCode() == shortCode;
                      }),
        createdUrls.end()
    );
}

int User::getTotalUrlsCreated() const {
    return static_cast<int>(createdUrls.size());
}

int User::getTotalClicks() const {
    int totalClicks = 0;
    for (const auto& url : createdUrls) {
        totalClicks += url->getClickCount();
    }
    return totalClicks;
}

std::string User::toString() const {
    std::ostringstream oss;
    auto time_t_created = std::chrono::system_clock::to_time_t(createdAt);
    auto time_t_last_login = std::chrono::system_clock::to_time_t(lastLoginAt);
    
    oss << "User Details:\n";
    oss << "  User ID: " << userId << "\n";
    oss << "  Username: " << username << "\n";
    oss << "  Email: " << email << "\n";
    oss << "  Created: " << std::ctime(&time_t_created);
    oss << "  Last Login: " << std::ctime(&time_t_last_login);
    oss << "  Active: " << (isActive ? "Yes" : "No") << "\n";
    oss << "  URLs Created: " << getTotalUrlsCreated() << "\n";
    oss << "  Total Clicks: " << getTotalClicks() << "\n";
    
    return oss.str();
} 