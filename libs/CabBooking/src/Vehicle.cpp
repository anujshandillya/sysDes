#include "../include/Vehicle.hpp"

Vehicle::Vehicle(const std::string& id, const std::string& licensePlate, const std::string& model,
                 const std::string& brand, int year, VehicleType type, int capacity,
                 double baseFare, double perKmRate, bool isAC)
    : vehicleId(id), licensePlate(licensePlate), model(model), brand(brand), year(year),
      type(type), status(VehicleStatus::AVAILABLE), capacity(capacity), baseFare(baseFare),
      perKmRate(perKmRate), isAC(isAC), currentLatitude(0.0), currentLongitude(0.0), fuelLevel(100.0) {}

std::string Vehicle::getVehicleId() const {
    return vehicleId;
}

std::string Vehicle::getLicensePlate() const {
    return licensePlate;
}

std::string Vehicle::getModel() const {
    return model;
}

std::string Vehicle::getBrand() const {
    return brand;
}

int Vehicle::getYear() const {
    return year;
}

VehicleType Vehicle::getType() const {
    return type;
}

VehicleStatus Vehicle::getStatus() const {
    return status;
}

int Vehicle::getCapacity() const {
    return capacity;
}

double Vehicle::getBaseFare() const {
    return baseFare;
}

double Vehicle::getPerKmRate() const {
    return perKmRate;
}

std::string Vehicle::getDriverId() const {
    return driverId;
}

double Vehicle::getCurrentLatitude() const {
    return currentLatitude;
}

double Vehicle::getCurrentLongitude() const {
    return currentLongitude;
}

double Vehicle::getFuelLevel() const {
    return fuelLevel;
}

bool Vehicle::getIsAC() const {
    return isAC;
}

void Vehicle::setStatus(VehicleStatus status) {
    this->status = status;
}

void Vehicle::setDriverId(const std::string& driverId) {
    this->driverId = driverId;
}

void Vehicle::setCurrentLocation(double lat, double lon) {
    currentLatitude = lat;
    currentLongitude = lon;
}

void Vehicle::setFuelLevel(double level) {
    fuelLevel = level;
}

std::string Vehicle::getVehicleTypeString() const {
    switch (type) {
        case VehicleType::SEDAN: return "SEDAN";
        case VehicleType::SUV: return "SUV";
        case VehicleType::HATCHBACK: return "HATCHBACK";
        case VehicleType::BIKE: return "BIKE";
        case VehicleType::AUTO: return "AUTO";
        case VehicleType::PREMIUM: return "PREMIUM";
        default: return "UNKNOWN";
    }
}

std::string Vehicle::getStatusString() const {
    switch (status) {
        case VehicleStatus::AVAILABLE: return "AVAILABLE";
        case VehicleStatus::BUSY: return "BUSY";
        case VehicleStatus::OFFLINE: return "OFFLINE";
        case VehicleStatus::MAINTENANCE: return "MAINTENANCE";
        default: return "UNKNOWN";
    }
}

bool Vehicle::isAvailable() const {
    return status == VehicleStatus::AVAILABLE && fuelLevel > 10.0;
}

double Vehicle::calculateFare(double distance) const {
    return baseFare + (distance * perKmRate);
}

void Vehicle::updateLocation(double lat, double lon) {
    currentLatitude = lat;
    currentLongitude = lon;
} 