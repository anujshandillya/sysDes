#ifndef URL_SHORTENER_HPP
#define URL_SHORTENER_HPP

#include <string>
#include <memory>
#include <vector>
#include "URL.hpp"
#include "User.hpp"
#include "Database.hpp"
#include "HashGenerator.hpp"
#include "Analytics.hpp"

struct ShortenRequest {
    std::string originalUrl;
    std::string customCode;
    std::string title;
    std::string description;
    std::string userId;
    int expirationDays;
};

struct ShortenResponse {
    bool success;
    std::string shortCode;
    std::string shortUrl;
    std::string message;
    std::shared_ptr<URL> url;
};

class URLShortener {
private:
    std::unique_ptr<Database> database;
    std::unique_ptr<HashGenerator> hashGenerator;
    std::unique_ptr<Analytics> analytics;
    std::string baseUrl;

public:
    URLShortener(const std::string& baseUrl = "http://short.url");
    
    // URL shortening
    ShortenResponse shortenUrl(const ShortenRequest& request);
    ShortenResponse shortenUrl(const std::string& originalUrl, const std::string& userId = "");
    std::string expandUrl(const std::string& shortCode, const std::string& ipAddress = "");
    
    // User management
    std::shared_ptr<User> createUser(const std::string& username, const std::string& email, 
                                   const std::string& password);
    std::shared_ptr<User> authenticateUser(const std::string& email, const std::string& password);
    bool updateUser(const std::string& userId, const std::string& username, const std::string& email);
    bool deleteUser(const std::string& userId);
    
    // URL management
    bool updateUrl(const std::string& shortCode, const std::string& title, 
                  const std::string& description, int expirationDays = 0);
    bool deleteUrl(const std::string& shortCode, const std::string& userId);
    std::vector<std::shared_ptr<URL>> getUserUrls(const std::string& userId);
    
    // Analytics
    UrlStats getUrlAnalytics(const std::string& shortCode);
    std::vector<std::pair<std::string, int>> getTopUrls(int limit = 10);
    std::vector<std::pair<std::string, int>> getClicksByCountry(const std::string& shortCode);
    std::vector<std::pair<std::string, int>> getClicksByDevice(const std::string& shortCode);
    
    // System management
    void cleanupExpiredUrls();
    void cleanupOldAnalytics(int daysToKeep = 90);
    int getTotalUrls();
    int getTotalUsers();
    int getTotalClicks();
    
    // Validation
    bool isValidUrl(const std::string& url);
    bool isValidCustomCode(const std::string& code);
    std::string sanitizeUrl(const std::string& url);
};

#endif // URL_SHORTENER_HPP 