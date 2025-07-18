#include "../include/Database.hpp"
#include <algorithm>

Database::Database() {
}

bool Database::addUrl(std::shared_ptr<URL> url) {
    std::lock_guard<std::mutex> lock(urlMutex);
    
    if (urlMap.find(url->getShortCode()) != urlMap.end()) {
        return false; // URL already exists
    }
    
    urlMap[url->getShortCode()] = url;
    
    // Add to user's created URLs if creator exists
    if (url->getCreator()) {
        url->getCreator()->addCreatedUrl(url);
    }
    
    return true;
}

bool Database::removeUrl(const std::string& shortCode) {
    std::lock_guard<std::mutex> lock(urlMutex);
    
    auto it = urlMap.find(shortCode);
    if (it == urlMap.end()) {
        return false;
    }
    
    // Remove from user's created URLs if creator exists
    if (it->second->getCreator()) {
        it->second->getCreator()->removeCreatedUrl(shortCode);
    }
    
    urlMap.erase(it);
    return true;
}

std::shared_ptr<URL> Database::getUrl(const std::string& shortCode) {
    std::lock_guard<std::mutex> lock(urlMutex);
    
    auto it = urlMap.find(shortCode);
    if (it != urlMap.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<URL>> Database::getAllUrls() {
    std::lock_guard<std::mutex> lock(urlMutex);
    
    std::vector<std::shared_ptr<URL>> urls;
    urls.reserve(urlMap.size());
    
    for (const auto& pair : urlMap) {
        urls.push_back(pair.second);
    }
    
    return urls;
}

std::vector<std::shared_ptr<URL>> Database::getUrlsByUser(const std::string& userId) {
    std::lock_guard<std::mutex> lock(urlMutex);
    
    std::vector<std::shared_ptr<URL>> userUrls;
    
    for (const auto& pair : urlMap) {
        if (pair.second->getCreator() && pair.second->getCreator()->getUserId() == userId) {
            userUrls.push_back(pair.second);
        }
    }
    
    return userUrls;
}

bool Database::urlExists(const std::string& shortCode) {
    std::lock_guard<std::mutex> lock(urlMutex);
    return urlMap.find(shortCode) != urlMap.end();
}

int Database::getTotalUrls() {
    std::lock_guard<std::mutex> lock(urlMutex);
    return static_cast<int>(urlMap.size());
}

bool Database::addUser(std::shared_ptr<User> user) {
    std::lock_guard<std::mutex> lock(userMutex);
    
    if (userMap.find(user->getUserId()) != userMap.end() ||
        userByEmail.find(user->getEmail()) != userByEmail.end() ||
        userByUsername.find(user->getUsername()) != userByUsername.end()) {
        return false; // User already exists
    }
    
    userMap[user->getUserId()] = user;
    userByEmail[user->getEmail()] = user;
    userByUsername[user->getUsername()] = user;
    
    return true;
}

bool Database::removeUser(const std::string& userId) {
    std::lock_guard<std::mutex> lock(userMutex);
    
    auto it = userMap.find(userId);
    if (it == userMap.end()) {
        return false;
    }
    
    std::shared_ptr<User> user = it->second;
    userByEmail.erase(user->getEmail());
    userByUsername.erase(user->getUsername());
    userMap.erase(it);
    
    return true;
}

std::shared_ptr<User> Database::getUser(const std::string& userId) {
    std::lock_guard<std::mutex> lock(userMutex);
    
    auto it = userMap.find(userId);
    if (it != userMap.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<User> Database::getUserByEmail(const std::string& email) {
    std::lock_guard<std::mutex> lock(userMutex);
    
    auto it = userByEmail.find(email);
    if (it != userByEmail.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<User> Database::getUserByUsername(const std::string& username) {
    std::lock_guard<std::mutex> lock(userMutex);
    
    auto it = userByUsername.find(username);
    if (it != userByUsername.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<User>> Database::getAllUsers() {
    std::lock_guard<std::mutex> lock(userMutex);
    
    std::vector<std::shared_ptr<User>> users;
    users.reserve(userMap.size());
    
    for (const auto& pair : userMap) {
        users.push_back(pair.second);
    }
    
    return users;
}

bool Database::userExists(const std::string& userId) {
    std::lock_guard<std::mutex> lock(userMutex);
    return userMap.find(userId) != userMap.end();
}

bool Database::emailExists(const std::string& email) {
    std::lock_guard<std::mutex> lock(userMutex);
    return userByEmail.find(email) != userByEmail.end();
}

bool Database::usernameExists(const std::string& username) {
    std::lock_guard<std::mutex> lock(userMutex);
    return userByUsername.find(username) != userByUsername.end();
}

int Database::getTotalUsers() {
    std::lock_guard<std::mutex> lock(userMutex);
    return static_cast<int>(userMap.size());
}

std::vector<std::shared_ptr<URL>> Database::getMostClickedUrls(int limit) {
    std::lock_guard<std::mutex> lock(urlMutex);
    
    std::vector<std::shared_ptr<URL>> urls;
    urls.reserve(urlMap.size());
    
    for (const auto& pair : urlMap) {
        urls.push_back(pair.second);
    }
    
    // Sort by click count (descending)
    std::sort(urls.begin(), urls.end(),
              [](const std::shared_ptr<URL>& a, const std::shared_ptr<URL>& b) {
                  return a->getClickCount() > b->getClickCount();
              });
    
    if (urls.size() > static_cast<size_t>(limit)) {
        urls.resize(limit);
    }
    
    return urls;
}

std::vector<std::shared_ptr<URL>> Database::getRecentlyCreatedUrls(int limit) {
    std::lock_guard<std::mutex> lock(urlMutex);
    
    std::vector<std::shared_ptr<URL>> urls;
    urls.reserve(urlMap.size());
    
    for (const auto& pair : urlMap) {
        urls.push_back(pair.second);
    }
    
    // Sort by creation time (descending)
    std::sort(urls.begin(), urls.end(),
              [](const std::shared_ptr<URL>& a, const std::shared_ptr<URL>& b) {
                  return a->getCreatedAt() > b->getCreatedAt();
              });
    
    if (urls.size() > static_cast<size_t>(limit)) {
        urls.resize(limit);
    }
    
    return urls;
}

std::vector<std::shared_ptr<URL>> Database::getExpiredUrls() {
    std::lock_guard<std::mutex> lock(urlMutex);
    
    std::vector<std::shared_ptr<URL>> expiredUrls;
    
    for (const auto& pair : urlMap) {
        if (pair.second->isExpired()) {
            expiredUrls.push_back(pair.second);
        }
    }
    
    return expiredUrls;
}

void Database::cleanupExpiredUrls() {
    std::lock_guard<std::mutex> lock(urlMutex);
    
    auto it = urlMap.begin();
    while (it != urlMap.end()) {
        if (it->second->isExpired()) {
            it = urlMap.erase(it);
        } else {
            ++it;
        }
    }
}

void Database::clearAll() {
    std::lock_guard<std::mutex> urlLock(urlMutex);
    std::lock_guard<std::mutex> userLock(userMutex);
    
    urlMap.clear();
    userMap.clear();
    userByEmail.clear();
    userByUsername.clear();
} 