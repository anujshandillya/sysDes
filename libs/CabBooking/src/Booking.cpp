#include "../include/Booking.hpp"
#include <chrono>

Booking::Booking(const std::string& id, const std::string& customerId,
                 std::shared_ptr<Location> pickup, std::shared_ptr<Location> drop,
                 const std::string& vehicleType, double estimatedFare)
    : bookingId(id), customerId(customerId), pickupLocation(pickup), dropLocation(drop),
      status(BookingStatus::PENDING), requestTime(std::chrono::system_clock::now()),
      estimatedFare(estimatedFare), vehicleType(vehicleType), estimatedWaitTime(0) {
    
    // Set expiry time to 5 minutes from request time
    expiryTime = requestTime + std::chrono::minutes(5);
}

std::string Booking::getBookingId() const {
    return bookingId;
}

std::string Booking::getCustomerId() const {
    return customerId;
}

std::string Booking::getDriverId() const {
    return driverId;
}

std::string Booking::getVehicleId() const {
    return vehicleId;
}

std::shared_ptr<Location> Booking::getPickupLocation() const {
    return pickupLocation;
}

std::shared_ptr<Location> Booking::getDropLocation() const {
    return dropLocation;
}

BookingStatus Booking::getStatus() const {
    return status;
}

std::chrono::system_clock::time_point Booking::getRequestTime() const {
    return requestTime;
}

std::chrono::system_clock::time_point Booking::getExpiryTime() const {
    return expiryTime;
}

double Booking::getEstimatedFare() const {
    return estimatedFare;
}

std::string Booking::getVehicleType() const {
    return vehicleType;
}

int Booking::getEstimatedWaitTime() const {
    return estimatedWaitTime;
}

std::string Booking::getSpecialInstructions() const {
    return specialInstructions;
}

std::shared_ptr<Trip> Booking::getTrip() const {
    return trip;
}

void Booking::setDriverId(const std::string& driverId) {
    this->driverId = driverId;
}

void Booking::setVehicleId(const std::string& vehicleId) {
    this->vehicleId = vehicleId;
}

void Booking::setStatus(BookingStatus status) {
    this->status = status;
}

void Booking::setEstimatedWaitTime(int minutes) {
    estimatedWaitTime = minutes;
}

void Booking::setSpecialInstructions(const std::string& instructions) {
    specialInstructions = instructions;
}

void Booking::setTrip(std::shared_ptr<Trip> trip) {
    this->trip = trip;
}

std::string Booking::getStatusString() const {
    switch (status) {
        case BookingStatus::PENDING: return "PENDING";
        case BookingStatus::CONFIRMED: return "CONFIRMED";
        case BookingStatus::ASSIGNED: return "ASSIGNED";
        case BookingStatus::CANCELLED: return "CANCELLED";
        case BookingStatus::EXPIRED: return "EXPIRED";
        default: return "UNKNOWN";
    }
}

bool Booking::isExpired() const {
    return std::chrono::system_clock::now() > expiryTime;
}

bool Booking::canBeCancelled() const {
    return status == BookingStatus::PENDING || status == BookingStatus::CONFIRMED;
}

void Booking::confirmBooking(const std::string& driverId, const std::string& vehicleId) {
    this->driverId = driverId;
    this->vehicleId = vehicleId;
    status = BookingStatus::CONFIRMED;
}

void Booking::cancelBooking() {
    status = BookingStatus::CANCELLED;
}

void Booking::assignDriver(const std::string& driverId, const std::string& vehicleId) {
    this->driverId = driverId;
    this->vehicleId = vehicleId;
    status = BookingStatus::ASSIGNED;
}

int Booking::getTimeUntilExpiry() const {
    auto now = std::chrono::system_clock::now();
    if (now >= expiryTime) {
        return 0;
    }
    
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(expiryTime - now);
    return duration.count();
} 