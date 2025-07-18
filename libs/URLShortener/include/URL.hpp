#ifndef URL_HPP
#define URL_HPP

#include <string>
#include <chrono>
#include <memory>

class User;

class URL {
private:
    std::string originalUrl;
    std::string shortCode;
    std::string title;
    std::string description;
    std::shared_ptr<User> creator;
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point expiresAt;
    bool isActive;
    int clickCount;

public:
    URL(const std::string& originalUrl, const std::string& shortCode, 
        std::shared_ptr<User> creator = nullptr);
    
    // Getters
    std::string getOriginalUrl() const;
    std::string getShortCode() const;
    std::string getTitle() const;
    std::string getDescription() const;
    std::shared_ptr<User> getCreator() const;
    std::chrono::system_clock::time_point getCreatedAt() const;
    std::chrono::system_clock::time_point getExpiresAt() const;
    bool getIsActive() const;
    int getClickCount() const;
    
    // Setters
    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setExpiresAt(const std::chrono::system_clock::time_point& expiresAt);
    void setIsActive(bool isActive);
    
    // Methods
    void incrementClickCount();
    bool isExpired() const;
    std::string getFullShortUrl(const std::string& baseUrl) const;
    std::string toString() const;
};

#endif // URL_HPP 