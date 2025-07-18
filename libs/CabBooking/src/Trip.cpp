#include "../include/Trip.hpp"
#include <chrono>

Trip::Trip(const std::string& id, const std::string& customerId, 
           std::shared_ptr<Location> pickup, std::shared_ptr<Location> drop)
    : tripId(id), customerId(customerId), pickupLocation(pickup), dropLocation(drop),
      status(TripStatus::REQUESTED), distance(0.0), fare(0.0), actualFare(0.0),
      requestTime(std::chrono::system_clock::now()), isPaid(false), customerRating(0.0), driverRating(0.0) {
    calculateDistance();
}

std::string Trip::getTripId() const {
    return tripId;
}

std::string Trip::getCustomerId() const {
    return customerId;
}

std::string Trip::getDriverId() const {
    return driverId;
}

std::string Trip::getVehicleId() const {
    return vehicleId;
}

std::shared_ptr<Location> Trip::getPickupLocation() const {
    return pickupLocation;
}

std::shared_ptr<Location> Trip::getDropLocation() const {
    return dropLocation;
}

TripStatus Trip::getStatus() const {
    return status;
}

double Trip::getDistance() const {
    return distance;
}

double Trip::getFare() const {
    return fare;
}

double Trip::getActualFare() const {
    return actualFare;
}

std::chrono::system_clock::time_point Trip::getRequestTime() const {
    return requestTime;
}

std::chrono::system_clock::time_point Trip::getStartTime() const {
    return startTime;
}

std::chrono::system_clock::time_point Trip::getEndTime() const {
    return endTime;
}

std::string Trip::getPaymentMethod() const {
    return paymentMethod;
}

bool Trip::getIsPaid() const {
    return isPaid;
}

double Trip::getCustomerRating() const {
    return customerRating;
}

double Trip::getDriverRating() const {
    return driverRating;
}

std::string Trip::getCustomerFeedback() const {
    return customerFeedback;
}

std::string Trip::getDriverFeedback() const {
    return driverFeedback;
}

void Trip::setDriverId(const std::string& driverId) {
    this->driverId = driverId;
}

void Trip::setVehicleId(const std::string& vehicleId) {
    this->vehicleId = vehicleId;
}

void Trip::setStatus(TripStatus status) {
    this->status = status;
}

void Trip::setDistance(double distance) {
    this->distance = distance;
}

void Trip::setFare(double fare) {
    this->fare = fare;
}

void Trip::setActualFare(double actualFare) {
    this->actualFare = actualFare;
}

void Trip::setStartTime(const std::chrono::system_clock::time_point& time) {
    startTime = time;
}

void Trip::setEndTime(const std::chrono::system_clock::time_point& time) {
    endTime = time;
}

void Trip::setPaymentMethod(const std::string& method) {
    paymentMethod = method;
}

void Trip::setIsPaid(bool paid) {
    isPaid = paid;
}

void Trip::setCustomerRating(double rating) {
    customerRating = rating;
}

void Trip::setDriverRating(double rating) {
    driverRating = rating;
}

void Trip::setCustomerFeedback(const std::string& feedback) {
    customerFeedback = feedback;
}

void Trip::setDriverFeedback(const std::string& feedback) {
    driverFeedback = feedback;
}

std::string Trip::getStatusString() const {
    switch (status) {
        case TripStatus::REQUESTED: return "REQUESTED";
        case TripStatus::ACCEPTED: return "ACCEPTED";
        case TripStatus::STARTED: return "STARTED";
        case TripStatus::IN_PROGRESS: return "IN_PROGRESS";
        case TripStatus::COMPLETED: return "COMPLETED";
        case TripStatus::CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}

double Trip::getDuration() const {
    if (startTime.time_since_epoch().count() == 0 || endTime.time_since_epoch().count() == 0) {
        return 0.0;
    }
    
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(endTime - startTime);
    return duration.count();
}

bool Trip::isActive() const {
    return status == TripStatus::STARTED || status == TripStatus::IN_PROGRESS;
}

void Trip::calculateDistance() {
    if (pickupLocation && dropLocation) {
        distance = pickupLocation->calculateDistance(*dropLocation);
    }
}

void Trip::startTrip() {
    status = TripStatus::STARTED;
    startTime = std::chrono::system_clock::now();
}

void Trip::completeTrip() {
    status = TripStatus::COMPLETED;
    endTime = std::chrono::system_clock::now();
}

void Trip::cancelTrip() {
    status = TripStatus::CANCELLED;
    endTime = std::chrono::system_clock::now();
} 