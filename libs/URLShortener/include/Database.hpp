#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include <mutex>
#include "URL.hpp"
#include "User.hpp"

class Database {
private:
    std::unordered_map<std::string, std::shared_ptr<URL>> urlMap; // shortCode -> URL
    std::unordered_map<std::string, std::shared_ptr<User>> userMap; // userId -> User
    std::unordered_map<std::string, std::shared_ptr<User>> userByEmail; // email -> User
    std::unordered_map<std::string, std::shared_ptr<User>> userByUsername; // username -> User
    mutable std::mutex urlMutex;
    mutable std::mutex userMutex;

public:
    Database();
    
    // URL operations
    bool addUrl(std::shared_ptr<URL> url);
    bool removeUrl(const std::string& shortCode);
    std::shared_ptr<URL> getUrl(const std::string& shortCode);
    std::vector<std::shared_ptr<URL>> getAllUrls();
    std::vector<std::shared_ptr<URL>> getUrlsByUser(const std::string& userId);
    bool urlExists(const std::string& shortCode);
    int getTotalUrls();
    
    // User operations
    bool addUser(std::shared_ptr<User> user);
    bool removeUser(const std::string& userId);
    std::shared_ptr<User> getUser(const std::string& userId);
    std::shared_ptr<User> getUserByEmail(const std::string& email);
    std::shared_ptr<User> getUserByUsername(const std::string& username);
    std::vector<std::shared_ptr<User>> getAllUsers();
    bool userExists(const std::string& userId);
    bool emailExists(const std::string& email);
    bool usernameExists(const std::string& username);
    int getTotalUsers();
    
    // Analytics
    std::vector<std::shared_ptr<URL>> getMostClickedUrls(int limit = 10);
    std::vector<std::shared_ptr<URL>> getRecentlyCreatedUrls(int limit = 10);
    std::vector<std::shared_ptr<URL>> getExpiredUrls();
    
    // Cleanup
    void cleanupExpiredUrls();
    void clearAll();
};

#endif // DATABASE_HPP 