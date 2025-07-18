#include "../include/User.hpp"
#include <algorithm>
#include <regex>
#include <sstream>

User::User(const std::string& id, const std::string& name, const std::string& email,
           const std::string& phone, const std::string& password, UserType type)
    : id(id), name(name), email(email), phone(phone), password(password), type(type), isActive(true) {}

std::string User::getId() const { return id; }
std::string User::getName() const { return name; }
std::string User::getEmail() const { return email; }
std::string User::getPhone() const { return phone; }
std::string User::getPassword() const { return password; }
UserType User::getType() const { return type; }
std::vector<std::string> User::getBookingIds() const { return bookingIds; }
bool User::getIsActive() const { return isActive; }

void User::setName(const std::string& name) { this->name = name; }
void User::setEmail(const std::string& email) { this->email = email; }
void User::setPhone(const std::string& phone) { this->phone = phone; }
void User::setPassword(const std::string& password) { this->password = password; }
void User::setType(UserType type) { this->type = type; }
void User::setIsActive(bool isActive) { this->isActive = isActive; }

void User::addBooking(const std::string& bookingId) {
    if (std::find(bookingIds.begin(), bookingIds.end(), bookingId) == bookingIds.end()) {
        bookingIds.push_back(bookingId);
    }
}

void User::removeBooking(const std::string& bookingId) {
    bookingIds.erase(std::remove(bookingIds.begin(), bookingIds.end(), bookingId), bookingIds.end());
}

bool User::hasBooking(const std::string& bookingId) const {
    return std::find(bookingIds.begin(), bookingIds.end(), bookingId) != bookingIds.end();
}

int User::getBookingCount() const { return bookingIds.size(); }

std::string User::getTypeString() const {
    switch (type) {
        case UserType::REGULAR: return "Regular";
        case UserType::PREMIUM: return "Premium";
        case UserType::VIP: return "VIP";
        default: return "Unknown";
    }
}

bool User::isPremium() const { return type == UserType::PREMIUM; }
bool User::isVIP() const { return type == UserType::VIP; }

std::string User::getUserInfo() const {
    std::ostringstream oss;
    oss << "User ID: " << id << "\n";
    oss << "Name: " << name << "\n";
    oss << "Email: " << email << "\n";
    oss << "Phone: " << phone << "\n";
    oss << "Type: " << getTypeString() << "\n";
    oss << "Bookings: " << getBookingCount() << "\n";
    oss << "Status: " << (isActive ? "Active" : "Inactive");
    return oss.str();
}

bool User::validateEmail() const {
    std::regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return std::regex_match(email, emailRegex);
}

bool User::validatePhone() const {
    std::regex phoneRegex(R"(\+?[1-9]\d{1,14})");
    return std::regex_match(phone, phoneRegex);
} 