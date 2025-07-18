#pragma once

#include "Trip.hpp"
#include "Customer.hpp"
#include "Driver.hpp"
#include <memory>
#include <chrono>

enum class BookingStatus {
    PENDING,
    CONFIRMED,
    ASSIGNED,
    CANCELLED,
    EXPIRED
};

class Booking {
private:
    std::string bookingId;
    std::string customerId;
    std::string driverId;
    std::string vehicleId;
    std::shared_ptr<Location> pickupLocation;
    std::shared_ptr<Location> dropLocation;
    BookingStatus status;
    std::chrono::system_clock::time_point requestTime;
    std::chrono::system_clock::time_point expiryTime;
    double estimatedFare;
    std::string vehicleType;
    int estimatedWaitTime; // in minutes
    std::string specialInstructions;
    std::shared_ptr<Trip> trip;

public:
    Booking(const std::string& id, const std::string& customerId,
            std::shared_ptr<Location> pickup, std::shared_ptr<Location> drop,
            const std::string& vehicleType, double estimatedFare);
    
    // Getters
    std::string getBookingId() const;
    std::string getCustomerId() const;
    std::string getDriverId() const;
    std::string getVehicleId() const;
    std::shared_ptr<Location> getPickupLocation() const;
    std::shared_ptr<Location> getDropLocation() const;
    BookingStatus getStatus() const;
    std::chrono::system_clock::time_point getRequestTime() const;
    std::chrono::system_clock::time_point getExpiryTime() const;
    double getEstimatedFare() const;
    std::string getVehicleType() const;
    int getEstimatedWaitTime() const;
    std::string getSpecialInstructions() const;
    std::shared_ptr<Trip> getTrip() const;
    
    // Setters
    void setDriverId(const std::string& driverId);
    void setVehicleId(const std::string& vehicleId);
    void setStatus(BookingStatus status);
    void setEstimatedWaitTime(int minutes);
    void setSpecialInstructions(const std::string& instructions);
    void setTrip(std::shared_ptr<Trip> trip);
    
    // Utility methods
    std::string getStatusString() const;
    bool isExpired() const;
    bool canBeCancelled() const;
    void confirmBooking(const std::string& driverId, const std::string& vehicleId);
    void cancelBooking();
    void assignDriver(const std::string& driverId, const std::string& vehicleId);
    int getTimeUntilExpiry() const; // in minutes
}; 