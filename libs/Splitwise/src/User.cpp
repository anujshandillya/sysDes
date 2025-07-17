#include "../include/User.hpp"
#include <algorithm>
#include <ctime>
#include <sstream>
#include <iomanip>

User::User(const std::string& userId, const std::string& name, const std::string& email,
           const std::string& phone, const std::string& profilePicture, Currency preferredCurrency)
    : userId(userId), name(name), email(email), phone(phone), profilePicture(profilePicture),
      status(UserStatus::ACTIVE), preferredCurrency(preferredCurrency), totalBalance(0.0) {
    
    // Set creation timestamp
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    createdAt = oss.str();
    lastActive = createdAt;
}

void User::addGroup(const std::string& groupId) {
    if (std::find(groupIds.begin(), groupIds.end(), groupId) == groupIds.end()) {
        groupIds.push_back(groupId);
    }
}

void User::removeGroup(const std::string& groupId) {
    groupIds.erase(std::remove(groupIds.begin(), groupIds.end(), groupId), groupIds.end());
}

void User::addFriend(const std::string& friendId) {
    if (std::find(friendIds.begin(), friendIds.end(), friendId) == friendIds.end()) {
        friendIds.push_back(friendId);
    }
}

void User::removeFriend(const std::string& friendId) {
    friendIds.erase(std::remove(friendIds.begin(), friendIds.end(), friendId), friendIds.end());
}

bool User::isInGroup(const std::string& groupId) const {
    return std::find(groupIds.begin(), groupIds.end(), groupId) != groupIds.end();
}

bool User::isFriend(const std::string& friendId) const {
    return std::find(friendIds.begin(), friendIds.end(), friendId) != friendIds.end();
}

void User::updateBalance(double amount) {
    totalBalance += amount;
}

std::string User::getStatusString() const {
    switch (status) {
        case UserStatus::ACTIVE: return "Active";
        case UserStatus::INACTIVE: return "Inactive";
        case UserStatus::SUSPENDED: return "Suspended";
        default: return "Unknown";
    }
}

std::string User::getCurrencyString() const {
    switch (preferredCurrency) {
        case Currency::USD: return "USD";
        case Currency::EUR: return "EUR";
        case Currency::GBP: return "GBP";
        case Currency::INR: return "INR";
        case Currency::CAD: return "CAD";
        case Currency::AUD: return "AUD";
        case Currency::JPY: return "JPY";
        case Currency::CNY: return "CNY";
        default: return "USD";
    }
} 