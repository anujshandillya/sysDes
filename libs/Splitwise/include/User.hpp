#pragma once

#include <string>
#include <vector>

enum class UserStatus {
    ACTIVE,
    INACTIVE,
    SUSPENDED
};

enum class Currency {
    USD,
    EUR,
    GBP,
    INR,
    CAD,
    AUD,
    JPY,
    CNY
};

class User {
private:
    std::string userId;
    std::string name;
    std::string email;
    std::string phone;
    std::string profilePicture;
    UserStatus status;
    Currency preferredCurrency;
    double totalBalance;
    std::vector<std::string> groupIds;
    std::vector<std::string> friendIds;
    std::string createdAt;
    std::string lastActive;

public:
    User(const std::string& userId, const std::string& name, const std::string& email,
         const std::string& phone = "", const std::string& profilePicture = "",
         Currency preferredCurrency = Currency::USD);

    // Getters
    std::string getUserId() const { return userId; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    std::string getProfilePicture() const { return profilePicture; }
    UserStatus getStatus() const { return status; }
    Currency getPreferredCurrency() const { return preferredCurrency; }
    double getTotalBalance() const { return totalBalance; }
    std::vector<std::string> getGroupIds() const { return groupIds; }
    std::vector<std::string> getFriendIds() const { return friendIds; }
    std::string getCreatedAt() const { return createdAt; }
    std::string getLastActive() const { return lastActive; }

    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setEmail(const std::string& newEmail) { email = newEmail; }
    void setPhone(const std::string& newPhone) { phone = newPhone; }
    void setProfilePicture(const std::string& picture) { profilePicture = picture; }
    void setStatus(UserStatus newStatus) { status = newStatus; }
    void setPreferredCurrency(Currency currency) { preferredCurrency = currency; }
    void setTotalBalance(double balance) { totalBalance = balance; }
    void setLastActive(const std::string& timestamp) { lastActive = timestamp; }

    // Utility methods
    void addGroup(const std::string& groupId);
    void removeGroup(const std::string& groupId);
    void addFriend(const std::string& friendId);
    void removeFriend(const std::string& friendId);
    bool isInGroup(const std::string& groupId) const;
    bool isFriend(const std::string& friendId) const;
    void updateBalance(double amount);
    std::string getStatusString() const;
    std::string getCurrencyString() const;
    bool isActive() const { return status == UserStatus::ACTIVE; }
}; 