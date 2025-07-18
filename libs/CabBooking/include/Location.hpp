#pragma once

#include <string>

class Location {
private:
    std::string locationId;
    std::string address;
    double latitude;
    double longitude;
    std::string city;
    std::string state;
    std::string country;
    std::string postalCode;

public:
    Location(const std::string& id, const std::string& address, double lat, double lon,
             const std::string& city, const std::string& state, const std::string& country,
             const std::string& postalCode);
    
    // Getters
    std::string getLocationId() const;
    std::string getAddress() const;
    double getLatitude() const;
    double getLongitude() const;
    std::string getCity() const;
    std::string getState() const;
    std::string getCountry() const;
    std::string getPostalCode() const;
    
    // Setters
    void setAddress(const std::string& address);
    void setLatitude(double lat);
    void setLongitude(double lon);
    void setCity(const std::string& city);
    void setState(const std::string& state);
    void setCountry(const std::string& country);
    void setPostalCode(const std::string& postalCode);
    
    // Utility methods
    double calculateDistance(const Location& other) const;
    std::string getFullAddress() const;
    bool isValid() const;
}; 