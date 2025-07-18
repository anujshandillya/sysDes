#include "../include/FareCalculator.hpp"
#include <sstream>
#include <iomanip>

FareCalculator::FareCalculator() 
    : peakHourMultiplier(1.5), nightMultiplier(1.3), weatherMultiplier(1.0),
      demandMultiplier(1.0), distanceMultiplier(1.0), waitingCharge(1.0),
      cancellationCharge(50.0), platformFee(0.15), peakHourStart(7), peakHourEnd(9),
      nightHourStart(22), nightHourEnd(6) {
    
    // Initialize base rates for different vehicle types
    baseRates["SEDAN"] = 50.0;
    baseRates["SUV"] = 80.0;
    baseRates["HATCHBACK"] = 40.0;
    baseRates["BIKE"] = 20.0;
    baseRates["AUTO"] = 30.0;
    baseRates["PREMIUM"] = 120.0;
    
    // Initialize per km rates
    perKmRates["SEDAN"] = 12.0;
    perKmRates["SUV"] = 18.0;
    perKmRates["HATCHBACK"] = 10.0;
    perKmRates["BIKE"] = 6.0;
    perKmRates["AUTO"] = 8.0;
    perKmRates["PREMIUM"] = 25.0;
}

double FareCalculator::calculateFare(std::shared_ptr<Location> pickup, std::shared_ptr<Location> drop,
                                    std::shared_ptr<Vehicle> vehicle, int currentHour) {
    double distance = calculateDistance(pickup, drop);
    double baseFare = calculateBaseFare(distance, vehicle);
    
    // Get current hour if not provided
    if (currentHour == -1) {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::tm* tm = std::localtime(&time_t);
        currentHour = tm->tm_hour;
    }
    
    double dynamicPricing = calculateDynamicPricing(baseFare, currentHour, demandMultiplier);
    double additionalCharges = calculateAdditionalCharges(baseFare);
    
    return baseFare + dynamicPricing + additionalCharges;
}

double FareCalculator::calculateDistance(std::shared_ptr<Location> pickup, std::shared_ptr<Location> drop) {
    if (!pickup || !drop) {
        return 0.0;
    }
    return pickup->calculateDistance(*drop);
}

double FareCalculator::calculateBaseFare(double distance, std::shared_ptr<Vehicle> vehicle) {
    if (!vehicle) {
        return 0.0;
    }
    
    std::string vehicleType = vehicle->getVehicleTypeString();
    double baseRate = getBaseRate(vehicleType);
    double perKmRate = getPerKmRate(vehicleType);
    
    return baseRate + (distance * perKmRate);
}

double FareCalculator::calculateDynamicPricing(double baseFare, int currentHour, double demandLevel) {
    double multiplier = getCurrentMultiplier(currentHour, demandLevel);
    return baseFare * (multiplier - 1.0); // Only the additional amount
}

double FareCalculator::calculateAdditionalCharges(double baseFare, int waitingMinutes) {
    double charges = 0.0;
    
    // Waiting charges
    charges += waitingMinutes * waitingCharge;
    
    // Platform fee
    charges += baseFare * platformFee;
    
    return charges;
}

void FareCalculator::setBaseRate(const std::string& vehicleType, double rate) {
    baseRates[vehicleType] = rate;
}

void FareCalculator::setPerKmRate(const std::string& vehicleType, double rate) {
    perKmRates[vehicleType] = rate;
}

void FareCalculator::setPeakHourMultiplier(double multiplier) {
    peakHourMultiplier = multiplier;
}

void FareCalculator::setNightMultiplier(double multiplier) {
    nightMultiplier = multiplier;
}

void FareCalculator::setWeatherMultiplier(double multiplier) {
    weatherMultiplier = multiplier;
}

void FareCalculator::setDemandMultiplier(double multiplier) {
    demandMultiplier = multiplier;
}

void FareCalculator::setDistanceMultiplier(double multiplier) {
    distanceMultiplier = multiplier;
}

void FareCalculator::setWaitingCharge(double charge) {
    waitingCharge = charge;
}

void FareCalculator::setCancellationCharge(double charge) {
    cancellationCharge = charge;
}

void FareCalculator::setPlatformFee(double fee) {
    platformFee = fee;
}

void FareCalculator::setPeakHours(int start, int end) {
    peakHourStart = start;
    peakHourEnd = end;
}

void FareCalculator::setNightHours(int start, int end) {
    nightHourStart = start;
    nightHourEnd = end;
}

double FareCalculator::getBaseRate(const std::string& vehicleType) const {
    auto it = baseRates.find(vehicleType);
    return it != baseRates.end() ? it->second : 50.0;
}

double FareCalculator::getPerKmRate(const std::string& vehicleType) const {
    auto it = perKmRates.find(vehicleType);
    return it != perKmRates.end() ? it->second : 12.0;
}

double FareCalculator::getPeakHourMultiplier() const {
    return peakHourMultiplier;
}

double FareCalculator::getNightMultiplier() const {
    return nightMultiplier;
}

double FareCalculator::getWeatherMultiplier() const {
    return weatherMultiplier;
}

double FareCalculator::getDemandMultiplier() const {
    return demandMultiplier;
}

double FareCalculator::getDistanceMultiplier() const {
    return distanceMultiplier;
}

double FareCalculator::getWaitingCharge() const {
    return waitingCharge;
}

double FareCalculator::getCancellationCharge() const {
    return cancellationCharge;
}

double FareCalculator::getPlatformFee() const {
    return platformFee;
}

bool FareCalculator::isPeakHour(int hour) const {
    if (peakHourStart <= peakHourEnd) {
        return hour >= peakHourStart && hour <= peakHourEnd;
    } else {
        // Handles cases where peak hours span midnight
        return hour >= peakHourStart || hour <= peakHourEnd;
    }
}

bool FareCalculator::isNightHour(int hour) const {
    if (nightHourStart <= nightHourEnd) {
        return hour >= nightHourStart && hour <= nightHourEnd;
    } else {
        // Handles cases where night hours span midnight
        return hour >= nightHourStart || hour <= nightHourEnd;
    }
}

double FareCalculator::getCurrentMultiplier(int hour, double demandLevel) const {
    double multiplier = 1.0;
    
    if (isPeakHour(hour)) {
        multiplier *= peakHourMultiplier;
    }
    
    if (isNightHour(hour)) {
        multiplier *= nightMultiplier;
    }
    
    multiplier *= weatherMultiplier;
    multiplier *= demandLevel;
    multiplier *= distanceMultiplier;
    
    return multiplier;
}

std::string FareCalculator::generateFareBreakdown(double distance, double baseFare, double dynamicPricing, 
                                                 double additionalCharges) const {
    std::ostringstream oss;
    oss << "=== FARE BREAKDOWN ===\n";
    oss << "Distance: " << std::fixed << std::setprecision(2) << distance << " km\n";
    oss << "Base Fare: $" << baseFare << "\n";
    oss << "Dynamic Pricing: $" << dynamicPricing << "\n";
    oss << "Additional Charges: $" << additionalCharges << "\n";
    oss << "Total Fare: $" << (baseFare + dynamicPricing + additionalCharges) << "\n";
    oss << "=====================";
    return oss.str();
} 