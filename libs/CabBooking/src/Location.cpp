#include "../include/Location.hpp"
#include <sstream>
#include <cmath>

Location::Location(const std::string& id, const std::string& address, double lat, double lon,
                   const std::string& city, const std::string& state, const std::string& country,
                   const std::string& postalCode)
    : locationId(id), address(address), latitude(lat), longitude(lon),
      city(city), state(state), country(country), postalCode(postalCode) {}

std::string Location::getLocationId() const {
    return locationId;
}

std::string Location::getAddress() const {
    return address;
}

double Location::getLatitude() const {
    return latitude;
}

double Location::getLongitude() const {
    return longitude;
}

std::string Location::getCity() const {
    return city;
}

std::string Location::getState() const {
    return state;
}

std::string Location::getCountry() const {
    return country;
}

std::string Location::getPostalCode() const {
    return postalCode;
}

void Location::setAddress(const std::string& address) {
    this->address = address;
}

void Location::setLatitude(double lat) {
    latitude = lat;
}

void Location::setLongitude(double lon) {
    longitude = lon;
}

void Location::setCity(const std::string& city) {
    this->city = city;
}

void Location::setState(const std::string& state) {
    this->state = state;
}

void Location::setCountry(const std::string& country) {
    this->country = country;
}

void Location::setPostalCode(const std::string& postalCode) {
    this->postalCode = postalCode;
}

double Location::calculateDistance(const Location& other) const {
    // Haversine formula to calculate distance between two points on Earth
    const double R = 6371.0; // Earth's radius in kilometers
    
    double lat1Rad = latitude * M_PI / 180.0;
    double lat2Rad = other.latitude * M_PI / 180.0;
    double deltaLat = (other.latitude - latitude) * M_PI / 180.0;
    double deltaLon = (other.longitude - longitude) * M_PI / 180.0;
    
    double a = sin(deltaLat / 2) * sin(deltaLat / 2) +
               cos(lat1Rad) * cos(lat2Rad) *
               sin(deltaLon / 2) * sin(deltaLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    
    return R * c;
}

std::string Location::getFullAddress() const {
    std::ostringstream oss;
    oss << address << ", " << city << ", " << state << " " << postalCode << ", " << country;
    return oss.str();
}

bool Location::isValid() const {
    return !locationId.empty() && !address.empty() && 
           latitude >= -90.0 && latitude <= 90.0 &&
           longitude >= -180.0 && longitude <= 180.0;
} 