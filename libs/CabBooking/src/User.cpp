#include "../include/User.hpp"

User::User(const std::string& id, const std::string& name, const std::string& email, 
           const std::string& phone, const std::string& password)
    : userId(id), name(name), email(email), phoneNumber(phone), password(password),
      isActive(true), rating(0.0), totalTrips(0) {}

std::string User::getUserId() const {
    return userId;
}

std::string User::getName() const {
    return name;
}

std::string User::getEmail() const {
    return email;
}

std::string User::getPhoneNumber() const {
    return phoneNumber;
}

bool User::getIsActive() const {
    return isActive;
}

double User::getRating() const {
    return rating;
}

int User::getTotalTrips() const {
    return totalTrips;
}

void User::setName(const std::string& name) {
    this->name = name;
}

void User::setEmail(const std::string& email) {
    this->email = email;
}

void User::setPhoneNumber(const std::string& phone) {
    phoneNumber = phone;
}

void User::setPassword(const std::string& password) {
    this->password = password;
}

void User::setIsActive(bool active) {
    isActive = active;
}

void User::setRating(double rating) {
    this->rating = rating;
}

void User::incrementTotalTrips() {
    totalTrips++;
}

void User::updateProfile(const std::string& name, const std::string& email, const std::string& phone) {
    this->name = name;
    this->email = email;
    phoneNumber = phone;
}

bool User::authenticate(const std::string& password) const {
    return this->password == password && isActive;
} 