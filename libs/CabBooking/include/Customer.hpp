#pragma once

#include "User.hpp"
#include "Location.hpp"
#include <vector>
#include <memory>

class Trip;
class Payment;

class Customer : public User {
private:
    std::vector<std::shared_ptr<Location>> favoriteLocations;
    std::vector<std::shared_ptr<Trip>> tripHistory;
    std::string preferredPaymentMethod;
    bool hasActiveBooking;

public:
    Customer(const std::string& id, const std::string& name, const std::string& email, 
             const std::string& phone, const std::string& password);
    
    // Customer-specific methods
    std::string getUserType() const override;
    void addFavoriteLocation(std::shared_ptr<Location> location);
    void removeFavoriteLocation(const std::string& locationId);
    std::vector<std::shared_ptr<Location>> getFavoriteLocations() const;
    
    void addTripToHistory(std::shared_ptr<Trip> trip);
    std::vector<std::shared_ptr<Trip>> getTripHistory() const;
    
    void setPreferredPaymentMethod(const std::string& method);
    std::string getPreferredPaymentMethod() const;
    
    void setHasActiveBooking(bool hasBooking);
    bool getHasActiveBooking() const;
    
    // Booking-related methods
    bool canBookRide() const;
    void updateProfile(const std::string& name, const std::string& email, const std::string& phone) override;
}; 