#pragma once

#include "User.hpp"
#include <vector>
#include <memory>

class Trip;

class Driver : public User {
private:
    std::string driverLicense;
    std::string vehicleId;
    bool isOnline;
    double currentLatitude;
    double currentLongitude;
    std::vector<std::shared_ptr<Trip>> completedTrips;
    double totalEarnings;
    std::string currentStatus; // "AVAILABLE", "BUSY", "OFFLINE"
    int experienceYears;

public:
    Driver(const std::string& id, const std::string& name, const std::string& email,
           const std::string& phone, const std::string& password, const std::string& license);
    
    // Driver-specific methods
    std::string getUserType() const override;
    void setDriverLicense(const std::string& license);
    std::string getDriverLicense() const;
    
    void assignVehicle(const std::string& vehicleId);
    std::string getVehicleId() const;
    
    void setOnlineStatus(bool online);
    bool getIsOnline() const;
    
    void updateLocation(double lat, double lon);
    double getCurrentLatitude() const;
    double getCurrentLongitude() const;
    
    void addCompletedTrip(std::shared_ptr<Trip> trip);
    std::vector<std::shared_ptr<Trip>> getCompletedTrips() const;
    
    void addEarnings(double amount);
    double getTotalEarnings() const;
    
    void setCurrentStatus(const std::string& status);
    std::string getCurrentStatus() const;
    
    void setExperienceYears(int years);
    int getExperienceYears() const;
    
    // Utility methods
    bool canAcceptRide() const;
    void updateProfile(const std::string& name, const std::string& email, const std::string& phone) override;
    double getAverageRating() const;
}; 