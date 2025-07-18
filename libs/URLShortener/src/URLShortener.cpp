#include "../include/URLShortener.hpp"
#include <regex>
#include <functional>
#include <cctype>

URLShortener::URLShortener(const std::string& baseUrl) : baseUrl(baseUrl) {
    database = std::make_unique<Database>();
    hashGenerator = std::make_unique<HashGenerator>();
    analytics = std::make_unique<Analytics>();
}

ShortenResponse URLShortener::shortenUrl(const ShortenRequest& request) {
    ShortenResponse response;
    
    // Validate URL
    if (!isValidUrl(request.originalUrl)) {
        response.success = false;
        response.message = "Invalid URL format";
        return response;
    }
    
    // Sanitize URL
    std::string sanitizedUrl = sanitizeUrl(request.originalUrl);
    
    // Get user if provided
    std::shared_ptr<User> user = nullptr;
    if (!request.userId.empty()) {
        user = database->getUser(request.userId);
        if (!user) {
            response.success = false;
            response.message = "User not found";
            return response;
        }
    }
    
    // Generate short code
    std::string shortCode;
    if (!request.customCode.empty()) {
        if (!isValidCustomCode(request.customCode)) {
            response.success = false;
            response.message = "Invalid custom code format";
            return response;
        }
        
        if (database->urlExists(request.customCode)) {
            response.success = false;
            response.message = "Custom code already exists";
            return response;
        }
        
        shortCode = request.customCode;
        hashGenerator->addUsedCode(shortCode);
    } else {
        shortCode = hashGenerator->generateShortCode();
    }
    
    // Create URL object
    auto url = std::make_shared<URL>(sanitizedUrl, shortCode, user);
    
    // Set additional properties
    if (!request.title.empty()) {
        url->setTitle(request.title);
    }
    if (!request.description.empty()) {
        url->setDescription(request.description);
    }
    if (request.expirationDays > 0) {
        auto expirationTime = std::chrono::system_clock::now() + 
                            std::chrono::hours(24 * request.expirationDays);
        url->setExpiresAt(expirationTime);
    }
    
    // Store in database
    if (!database->addUrl(url)) {
        response.success = false;
        response.message = "Failed to store URL";
        return response;
    }
    
    // Prepare response
    response.success = true;
    response.shortCode = shortCode;
    response.shortUrl = url->getFullShortUrl(baseUrl);
    response.message = "URL shortened successfully";
    response.url = url;
    
    return response;
}

ShortenResponse URLShortener::shortenUrl(const std::string& originalUrl, const std::string& userId) {
    ShortenRequest request;
    request.originalUrl = originalUrl;
    request.userId = userId;
    return shortenUrl(request);
}

std::string URLShortener::expandUrl(const std::string& shortCode, const std::string& ipAddress) {
    auto url = database->getUrl(shortCode);
    if (!url) {
        return "";
    }
    
    // Check if URL is active and not expired
    if (!url->getIsActive() || url->isExpired()) {
        return "";
    }
    
    // Record analytics
    if (!ipAddress.empty()) {
        analytics->recordClick(shortCode, ipAddress);
    }
    
    // Increment click count
    url->incrementClickCount();
    
    return url->getOriginalUrl();
}

std::shared_ptr<User> URLShortener::createUser(const std::string& username, const std::string& email, 
                                              const std::string& password) {
    // Validate input
    if (username.empty() || email.empty() || password.empty()) {
        return nullptr;
    }
    
    // Check if user already exists
    if (database->emailExists(email) || database->usernameExists(username)) {
        return nullptr;
    }
    
    // Simple password hashing (in production, use proper hashing)
    std::hash<std::string> hasher;
    std::string passwordHash = std::to_string(hasher(password));
    
    // Create user
    auto user = std::make_shared<User>(username, email, passwordHash);
    
    // Store in database
    if (!database->addUser(user)) {
        return nullptr;
    }
    
    return user;
}

std::shared_ptr<User> URLShortener::authenticateUser(const std::string& email, const std::string& password) {
    auto user = database->getUserByEmail(email);
    if (!user) {
        return nullptr;
    }
    
    // Simple password verification (in production, use proper hashing)
    std::hash<std::string> hasher;
    std::string passwordHash = std::to_string(hasher(password));
    
    if (user->getPasswordHash() != passwordHash) {
        return nullptr;
    }
    
    // Update last login time
    user->setLastLoginAt(std::chrono::system_clock::now());
    
    return user;
}

bool URLShortener::updateUser(const std::string& userId, const std::string& username, const std::string& email) {
    auto user = database->getUser(userId);
    if (!user) {
        return false;
    }
    
    // Check if new username/email already exists
    if (!username.empty() && username != user->getUsername() && database->usernameExists(username)) {
        return false;
    }
    if (!email.empty() && email != user->getEmail() && database->emailExists(email)) {
        return false;
    }
    
    // Update user
    if (!username.empty()) {
        user->setUsername(username);
    }
    if (!email.empty()) {
        user->setEmail(email);
    }
    
    return true;
}

bool URLShortener::deleteUser(const std::string& userId) {
    return database->removeUser(userId);
}

bool URLShortener::updateUrl(const std::string& shortCode, const std::string& title, 
                           const std::string& description, int expirationDays) {
    auto url = database->getUrl(shortCode);
    if (!url) {
        return false;
    }
    
    if (!title.empty()) {
        url->setTitle(title);
    }
    if (!description.empty()) {
        url->setDescription(description);
    }
    if (expirationDays > 0) {
        auto expirationTime = std::chrono::system_clock::now() + 
                            std::chrono::hours(24 * expirationDays);
        url->setExpiresAt(expirationTime);
    }
    
    return true;
}

bool URLShortener::deleteUrl(const std::string& shortCode, const std::string& userId) {
    auto url = database->getUrl(shortCode);
    if (!url) {
        return false;
    }
    
    // Check if user owns the URL
    if (!userId.empty() && url->getCreator() && url->getCreator()->getUserId() != userId) {
        return false;
    }
    
    // Remove from analytics
    analytics->clearUrlStats(shortCode);
    
    // Remove from hash generator
    hashGenerator->removeUsedCode(shortCode);
    
    return database->removeUrl(shortCode);
}

std::vector<std::shared_ptr<URL>> URLShortener::getUserUrls(const std::string& userId) {
    return database->getUrlsByUser(userId);
}

UrlStats URLShortener::getUrlAnalytics(const std::string& shortCode) {
    return analytics->getUrlStats(shortCode);
}

std::vector<std::pair<std::string, int>> URLShortener::getTopUrls(int limit) {
    return analytics->getTopUrlsByClicks(limit);
}

std::vector<std::pair<std::string, int>> URLShortener::getClicksByCountry(const std::string& shortCode) {
    return analytics->getClicksByCountry(shortCode);
}

std::vector<std::pair<std::string, int>> URLShortener::getClicksByDevice(const std::string& shortCode) {
    return analytics->getClicksByDevice(shortCode);
}

void URLShortener::cleanupExpiredUrls() {
    database->cleanupExpiredUrls();
}

void URLShortener::cleanupOldAnalytics(int daysToKeep) {
    analytics->cleanupOldEvents(daysToKeep);
}

int URLShortener::getTotalUrls() {
    return database->getTotalUrls();
}

int URLShortener::getTotalUsers() {
    return database->getTotalUsers();
}

int URLShortener::getTotalClicks() {
    int totalClicks = 0;
    auto urls = database->getAllUrls();
    for (const auto& url : urls) {
        totalClicks += url->getClickCount();
    }
    return totalClicks;
}

bool URLShortener::isValidUrl(const std::string& url) {
    if (url.empty()) {
        return false;
    }
    
    // Simple URL validation regex
    std::regex urlPattern(R"(^(https?:\/\/)?([\da-z\.-]+)\.([a-z\.]{2,6})([\/\w \.-]*)*\/?$)");
    return std::regex_match(url, urlPattern);
}

bool URLShortener::isValidCustomCode(const std::string& code) {
    if (code.empty() || code.length() > 20) {
        return false;
    }
    
    // Check if code contains only valid characters
    const std::string validChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (char c : code) {
        if (validChars.find(c) == std::string::npos) {
            return false;
        }
    }
    
    return true;
}

std::string URLShortener::sanitizeUrl(const std::string& url) {
    std::string sanitized = url;
    
    // Add http:// if no protocol specified
    if (sanitized.find("http://") != 0 && sanitized.find("https://") != 0) {
        sanitized = "http://" + sanitized;
    }
    
    // Remove trailing slash
    if (sanitized.back() == '/') {
        sanitized.pop_back();
    }
    
    return sanitized;
} 