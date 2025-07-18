#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>

enum class UserType {
    REGULAR,
    PREMIUM,
    VIP
};

class User {
private:
    std::string id;
    std::string name;
    std::string email;
    std::string phone;
    std::string password; // In real system, this would be hashed
    UserType type;
    std::vector<std::string> bookingIds; // List of booking IDs by this user
    bool isActive;

public:
    User(const std::string& id, const std::string& name, const std::string& email,
         const std::string& phone, const std::string& password, UserType type = UserType::REGULAR);
    
    // Getters
    std::string getId() const;
    std::string getName() const;
    std::string getEmail() const;
    std::string getPhone() const;
    std::string getPassword() const;
    UserType getType() const;
    std::vector<std::string> getBookingIds() const;
    bool getIsActive() const;
    
    // Setters
    void setName(const std::string& name);
    void setEmail(const std::string& email);
    void setPhone(const std::string& phone);
    void setPassword(const std::string& password);
    void setType(UserType type);
    void setIsActive(bool isActive);
    
    // Booking management
    void addBooking(const std::string& bookingId);
    void removeBooking(const std::string& bookingId);
    bool hasBooking(const std::string& bookingId) const;
    int getBookingCount() const;
    
    // Utility methods
    std::string getTypeString() const;
    bool isPremium() const;
    bool isVIP() const;
    std::string getUserInfo() const;
    bool validateEmail() const;
    bool validatePhone() const;
};

#endif // USER_HPP 