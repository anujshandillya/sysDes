#include "../include/Driver.hpp"
#include "../include/Trip.hpp"

Driver::Driver(const std::string& id, const std::string& name, const std::string& email,
               const std::string& phone, const std::string& password, const std::string& license)
    : User(id, name, email, phone, password), driverLicense(license), isOnline(false),
      currentLatitude(0.0), currentLongitude(0.0), totalEarnings(0.0), currentStatus("OFFLINE"),
      experienceYears(0) {}

std::string Driver::getUserType() const {
    return "DRIVER";
}

void Driver::setDriverLicense(const std::string& license) {
    driverLicense = license;
}

std::string Driver::getDriverLicense() const {
    return driverLicense;
}

void Driver::assignVehicle(const std::string& vehicleId) {
    this->vehicleId = vehicleId;
}

std::string Driver::getVehicleId() const {
    return vehicleId;
}

void Driver::setOnlineStatus(bool online) {
    isOnline = online;
    currentStatus = online ? "AVAILABLE" : "OFFLINE";
}

bool Driver::getIsOnline() const {
    return isOnline;
}

void Driver::updateLocation(double lat, double lon) {
    currentLatitude = lat;
    currentLongitude = lon;
}

double Driver::getCurrentLatitude() const {
    return currentLatitude;
}

double Driver::getCurrentLongitude() const {
    return currentLongitude;
}

void Driver::addCompletedTrip(std::shared_ptr<Trip> trip) {
    completedTrips.push_back(trip);
}

std::vector<std::shared_ptr<Trip>> Driver::getCompletedTrips() const {
    return completedTrips;
}

void Driver::addEarnings(double amount) {
    totalEarnings += amount;
}

double Driver::getTotalEarnings() const {
    return totalEarnings;
}

void Driver::setCurrentStatus(const std::string& status) {
    currentStatus = status;
}

std::string Driver::getCurrentStatus() const {
    return currentStatus;
}

void Driver::setExperienceYears(int years) {
    experienceYears = years;
}

int Driver::getExperienceYears() const {
    return experienceYears;
}

bool Driver::canAcceptRide() const {
    return getIsActive() && isOnline && currentStatus == "AVAILABLE" && !vehicleId.empty();
}

void Driver::updateProfile(const std::string& name, const std::string& email, const std::string& phone) {
    User::updateProfile(name, email, phone);
}

double Driver::getAverageRating() const {
    if (completedTrips.empty()) {
        return 0.0;
    }
    
    double totalRating = 0.0;
    int ratedTrips = 0;
    
    for (const auto& trip : completedTrips) {
        if (trip->getDriverRating() > 0) {
            totalRating += trip->getDriverRating();
            ratedTrips++;
        }
    }
    
    return ratedTrips > 0 ? totalRating / ratedTrips : 0.0;
} 