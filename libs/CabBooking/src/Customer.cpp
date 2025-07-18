#include "../include/Customer.hpp"

Customer::Customer(const std::string& id, const std::string& name, const std::string& email, 
                   const std::string& phone, const std::string& password)
    : User(id, name, email, phone, password), hasActiveBooking(false) {}

std::string Customer::getUserType() const {
    return "CUSTOMER";
}

void Customer::addFavoriteLocation(std::shared_ptr<Location> location) {
    favoriteLocations.push_back(location);
}

void Customer::removeFavoriteLocation(const std::string& locationId) {
    favoriteLocations.erase(
        std::remove_if(favoriteLocations.begin(), favoriteLocations.end(),
                      [&locationId](const std::shared_ptr<Location>& loc) {
                          return loc->getLocationId() == locationId;
                      }),
        favoriteLocations.end()
    );
}

std::vector<std::shared_ptr<Location>> Customer::getFavoriteLocations() const {
    return favoriteLocations;
}

void Customer::addTripToHistory(std::shared_ptr<Trip> trip) {
    tripHistory.push_back(trip);
}

std::vector<std::shared_ptr<Trip>> Customer::getTripHistory() const {
    return tripHistory;
}

void Customer::setPreferredPaymentMethod(const std::string& method) {
    preferredPaymentMethod = method;
}

std::string Customer::getPreferredPaymentMethod() const {
    return preferredPaymentMethod;
}

void Customer::setHasActiveBooking(bool hasBooking) {
    hasActiveBooking = hasBooking;
}

bool Customer::getHasActiveBooking() const {
    return hasActiveBooking;
}

bool Customer::canBookRide() const {
    return getIsActive() && !hasActiveBooking;
}

void Customer::updateProfile(const std::string& name, const std::string& email, const std::string& phone) {
    User::updateProfile(name, email, phone);
} 