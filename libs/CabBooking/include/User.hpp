#pragma once

#include <string>

class User {
protected:
    std::string userId;
    std::string name;
    std::string email;
    std::string phoneNumber;
    std::string password;
    bool isActive;
    double rating;
    int totalTrips;

public:
    User(const std::string& id, const std::string& name, const std::string& email, 
         const std::string& phone, const std::string& password);
    virtual ~User() = default;

    // Getters
    std::string getUserId() const;
    std::string getName() const;
    std::string getEmail() const;
    std::string getPhoneNumber() const;
    bool getIsActive() const;
    double getRating() const;
    int getTotalTrips() const;

    // Setters
    void setName(const std::string& name);
    void setEmail(const std::string& email);
    void setPhoneNumber(const std::string& phone);
    void setPassword(const std::string& password);
    void setIsActive(bool active);
    void setRating(double rating);
    void incrementTotalTrips();

    // Virtual methods
    virtual std::string getUserType() const = 0;
    virtual void updateProfile(const std::string& name, const std::string& email, const std::string& phone);
    virtual bool authenticate(const std::string& password) const;
}; 