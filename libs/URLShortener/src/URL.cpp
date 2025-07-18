#include "../include/URL.hpp"
#include "../include/User.hpp"
#include <sstream>

URL::URL(const std::string& originalUrl, const std::string& shortCode, std::shared_ptr<User> creator)
    : originalUrl(originalUrl), shortCode(shortCode), creator(creator), 
      createdAt(std::chrono::system_clock::now()), isActive(true), clickCount(0) {
    // Set default expiration to 1 year from now
    expiresAt = createdAt + std::chrono::hours(24 * 365);
}

std::string URL::getOriginalUrl() const {
    return originalUrl;
}

std::string URL::getShortCode() const {
    return shortCode;
}

std::string URL::getTitle() const {
    return title;
}

std::string URL::getDescription() const {
    return description;
}

std::shared_ptr<User> URL::getCreator() const {
    return creator;
}

std::chrono::system_clock::time_point URL::getCreatedAt() const {
    return createdAt;
}

std::chrono::system_clock::time_point URL::getExpiresAt() const {
    return expiresAt;
}

bool URL::getIsActive() const {
    return isActive;
}

int URL::getClickCount() const {
    return clickCount;
}

void URL::setTitle(const std::string& title) {
    this->title = title;
}

void URL::setDescription(const std::string& description) {
    this->description = description;
}

void URL::setExpiresAt(const std::chrono::system_clock::time_point& expiresAt) {
    this->expiresAt = expiresAt;
}

void URL::setIsActive(bool isActive) {
    this->isActive = isActive;
}

void URL::incrementClickCount() {
    clickCount++;
}

bool URL::isExpired() const {
    return std::chrono::system_clock::now() > expiresAt;
}

std::string URL::getFullShortUrl(const std::string& baseUrl) const {
    return baseUrl + "/" + shortCode;
}

std::string URL::toString() const {
    std::ostringstream oss;
    auto time_t_created = std::chrono::system_clock::to_time_t(createdAt);
    auto time_t_expires = std::chrono::system_clock::to_time_t(expiresAt);
    
    oss << "URL Details:\n";
    oss << "  Original URL: " << originalUrl << "\n";
    oss << "  Short Code: " << shortCode << "\n";
    oss << "  Title: " << title << "\n";
    oss << "  Description: " << description << "\n";
    oss << "  Created: " << std::ctime(&time_t_created);
    oss << "  Expires: " << std::ctime(&time_t_expires);
    oss << "  Active: " << (isActive ? "Yes" : "No") << "\n";
    oss << "  Click Count: " << clickCount << "\n";
    oss << "  Expired: " << (isExpired() ? "Yes" : "No") << "\n";
    
    if (creator) {
        oss << "  Creator: " << creator->getUsername() << "\n";
    }
    
    return oss.str();
} 