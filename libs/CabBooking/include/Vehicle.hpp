#pragma once

#include <string>

enum class VehicleType {
    SEDAN,
    SUV,
    HATCHBACK,
    BIKE,
    AUTO,
    PREMIUM
};

enum class VehicleStatus {
    AVAILABLE,
    BUSY,
    OFFLINE,
    MAINTENANCE
};

class Vehicle {
private:
    std::string vehicleId;
    std::string licensePlate;
    std::string model;
    std::string brand;
    int year;
    VehicleType type;
    VehicleStatus status;
    int capacity;
    double baseFare;
    double perKmRate;
    std::string driverId;
    double currentLatitude;
    double currentLongitude;
    double fuelLevel;
    bool isAC;

public:
    Vehicle(const std::string& id, const std::string& licensePlate, const std::string& model,
            const std::string& brand, int year, VehicleType type, int capacity,
            double baseFare, double perKmRate, bool isAC = true);
    
    // Getters
    std::string getVehicleId() const;
    std::string getLicensePlate() const;
    std::string getModel() const;
    std::string getBrand() const;
    int getYear() const;
    VehicleType getType() const;
    VehicleStatus getStatus() const;
    int getCapacity() const;
    double getBaseFare() const;
    double getPerKmRate() const;
    std::string getDriverId() const;
    double getCurrentLatitude() const;
    double getCurrentLongitude() const;
    double getFuelLevel() const;
    bool getIsAC() const;
    
    // Setters
    void setStatus(VehicleStatus status);
    void setDriverId(const std::string& driverId);
    void setCurrentLocation(double lat, double lon);
    void setFuelLevel(double level);
    
    // Utility methods
    std::string getVehicleTypeString() const;
    std::string getStatusString() const;
    bool isAvailable() const;
    double calculateFare(double distance) const;
    void updateLocation(double lat, double lon);
}; 