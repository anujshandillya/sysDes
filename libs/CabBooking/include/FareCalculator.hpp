#pragma once

#include "Location.hpp"
#include "Vehicle.hpp"
#include <memory>
#include <map>

class FareCalculator {
private:
    // Base rates for different vehicle types
    std::map<std::string, double> baseRates;
    std::map<std::string, double> perKmRates;
    
    // Dynamic pricing factors
    double peakHourMultiplier;
    double nightMultiplier;
    double weatherMultiplier;
    double demandMultiplier;
    double distanceMultiplier;
    
    // Additional charges
    double waitingCharge; // per minute
    double cancellationCharge;
    double platformFee; // percentage
    
    // Time-based pricing
    int peakHourStart;
    int peakHourEnd;
    int nightHourStart;
    int nightHourEnd;

public:
    FareCalculator();
    
    // Core calculation methods
    double calculateFare(std::shared_ptr<Location> pickup, std::shared_ptr<Location> drop,
                        std::shared_ptr<Vehicle> vehicle, int currentHour = -1);
    
    double calculateDistance(std::shared_ptr<Location> pickup, std::shared_ptr<Location> drop);
    double calculateBaseFare(double distance, std::shared_ptr<Vehicle> vehicle);
    double calculateDynamicPricing(double baseFare, int currentHour, double demandLevel);
    double calculateAdditionalCharges(double baseFare, int waitingMinutes = 0);
    
    // Configuration methods
    void setBaseRate(const std::string& vehicleType, double rate);
    void setPerKmRate(const std::string& vehicleType, double rate);
    void setPeakHourMultiplier(double multiplier);
    void setNightMultiplier(double multiplier);
    void setWeatherMultiplier(double multiplier);
    void setDemandMultiplier(double multiplier);
    void setDistanceMultiplier(double multiplier);
    void setWaitingCharge(double charge);
    void setCancellationCharge(double charge);
    void setPlatformFee(double fee);
    void setPeakHours(int start, int end);
    void setNightHours(int start, int end);
    
    // Getters
    double getBaseRate(const std::string& vehicleType) const;
    double getPerKmRate(const std::string& vehicleType) const;
    double getPeakHourMultiplier() const;
    double getNightMultiplier() const;
    double getWeatherMultiplier() const;
    double getDemandMultiplier() const;
    double getDistanceMultiplier() const;
    double getWaitingCharge() const;
    double getCancellationCharge() const;
    double getPlatformFee() const;
    
    // Utility methods
    bool isPeakHour(int hour) const;
    bool isNightHour(int hour) const;
    double getCurrentMultiplier(int hour, double demandLevel) const;
    std::string generateFareBreakdown(double distance, double baseFare, double dynamicPricing, 
                                     double additionalCharges) const;
}; 