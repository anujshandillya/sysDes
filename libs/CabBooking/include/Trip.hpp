#pragma once

#include "Location.hpp"
#include <string>
#include <memory>
#include <chrono>

enum class TripStatus {
    REQUESTED,
    ACCEPTED,
    STARTED,
    IN_PROGRESS,
    COMPLETED,
    CANCELLED
};

class Trip {
private:
    std::string tripId;
    std::string customerId;
    std::string driverId;
    std::string vehicleId;
    std::shared_ptr<Location> pickupLocation;
    std::shared_ptr<Location> dropLocation;
    TripStatus status;
    double distance;
    double fare;
    double actualFare;
    std::chrono::system_clock::time_point requestTime;
    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point endTime;
    std::string paymentMethod;
    bool isPaid;
    double customerRating;
    double driverRating;
    std::string customerFeedback;
    std::string driverFeedback;

public:
    Trip(const std::string& id, const std::string& customerId, 
         std::shared_ptr<Location> pickup, std::shared_ptr<Location> drop);
    
    // Getters
    std::string getTripId() const;
    std::string getCustomerId() const;
    std::string getDriverId() const;
    std::string getVehicleId() const;
    std::shared_ptr<Location> getPickupLocation() const;
    std::shared_ptr<Location> getDropLocation() const;
    TripStatus getStatus() const;
    double getDistance() const;
    double getFare() const;
    double getActualFare() const;
    std::chrono::system_clock::time_point getRequestTime() const;
    std::chrono::system_clock::time_point getStartTime() const;
    std::chrono::system_clock::time_point getEndTime() const;
    std::string getPaymentMethod() const;
    bool getIsPaid() const;
    double getCustomerRating() const;
    double getDriverRating() const;
    std::string getCustomerFeedback() const;
    std::string getDriverFeedback() const;
    
    // Setters
    void setDriverId(const std::string& driverId);
    void setVehicleId(const std::string& vehicleId);
    void setStatus(TripStatus status);
    void setDistance(double distance);
    void setFare(double fare);
    void setActualFare(double actualFare);
    void setStartTime(const std::chrono::system_clock::time_point& time);
    void setEndTime(const std::chrono::system_clock::time_point& time);
    void setPaymentMethod(const std::string& method);
    void setIsPaid(bool paid);
    void setCustomerRating(double rating);
    void setDriverRating(double rating);
    void setCustomerFeedback(const std::string& feedback);
    void setDriverFeedback(const std::string& feedback);
    
    // Utility methods
    std::string getStatusString() const;
    double getDuration() const; // in minutes
    bool isActive() const;
    void calculateDistance();
    void startTrip();
    void completeTrip();
    void cancelTrip();
}; 