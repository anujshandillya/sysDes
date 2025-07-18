#pragma once

#include "Customer.hpp"
#include "Driver.hpp"
#include "Vehicle.hpp"
#include "Location.hpp"
#include "Trip.hpp"
#include "Booking.hpp"
#include "Payment.hpp"
#include "FareCalculator.hpp"
#include "NotificationService.hpp"
#include <vector>
#include <memory>
#include <unordered_map>

class CabBookingSystem {
private:
    // Data storage
    std::unordered_map<std::string, std::shared_ptr<Customer>> customers;
    std::unordered_map<std::string, std::shared_ptr<Driver>> drivers;
    std::unordered_map<std::string, std::shared_ptr<Vehicle>> vehicles;
    std::unordered_map<std::string, std::shared_ptr<Trip>> trips;
    std::unordered_map<std::string, std::shared_ptr<Booking>> bookings;
    std::unordered_map<std::string, std::shared_ptr<Payment>> payments;
    std::unordered_map<std::string, std::shared_ptr<Location>> locations;
    
    // Services
    std::shared_ptr<FareCalculator> fareCalculator;
    std::shared_ptr<NotificationService> notificationService;
    
    // System state
    int nextCustomerId;
    int nextDriverId;
    int nextVehicleId;
    int nextTripId;
    int nextBookingId;
    int nextPaymentId;
    int nextLocationId;

public:
    CabBookingSystem();
    
    // Customer management
    std::shared_ptr<Customer> registerCustomer(const std::string& name, const std::string& email,
                                              const std::string& phone, const std::string& password);
    std::shared_ptr<Customer> authenticateCustomer(const std::string& email, const std::string& password);
    bool updateCustomerProfile(const std::string& customerId, const std::string& name,
                              const std::string& email, const std::string& phone);
    std::vector<std::shared_ptr<Customer>> getAllCustomers() const;
    
    // Driver management
    std::shared_ptr<Driver> registerDriver(const std::string& name, const std::string& email,
                                          const std::string& phone, const std::string& password,
                                          const std::string& license);
    std::shared_ptr<Driver> authenticateDriver(const std::string& email, const std::string& password);
    bool updateDriverProfile(const std::string& driverId, const std::string& name,
                            const std::string& email, const std::string& phone);
    std::vector<std::shared_ptr<Driver>> getAvailableDrivers() const;
    std::vector<std::shared_ptr<Driver>> getAllDrivers() const;
    
    // Vehicle management
    std::shared_ptr<Vehicle> addVehicle(const std::string& licensePlate, const std::string& model,
                                       const std::string& brand, int year, VehicleType type,
                                       int capacity, double baseFare, double perKmRate, bool isAC = true);
    std::shared_ptr<Vehicle> assignVehicleToDriver(const std::string& vehicleId, const std::string& driverId);
    std::vector<std::shared_ptr<Vehicle>> getAvailableVehicles() const;
    std::vector<std::shared_ptr<Vehicle>> getAllVehicles() const;
    
    // Location management
    std::shared_ptr<Location> addLocation(const std::string& address, double latitude, double longitude,
                                         const std::string& city, const std::string& state,
                                         const std::string& country, const std::string& postalCode);
    std::vector<std::shared_ptr<Location>> searchLocations(const std::string& query) const;
    
    // Booking management
    std::shared_ptr<Booking> createBooking(const std::string& customerId, const std::string& pickupLocationId,
                                          const std::string& dropLocationId, const std::string& vehicleType);
    std::shared_ptr<Driver> findNearestDriver(const std::string& pickupLocationId, const std::string& vehicleType);
    bool assignDriverToBooking(const std::string& bookingId, const std::string& driverId);
    bool cancelBooking(const std::string& bookingId, const std::string& reason);
    std::vector<std::shared_ptr<Booking>> getCustomerBookings(const std::string& customerId) const;
    std::vector<std::shared_ptr<Booking>> getDriverBookings(const std::string& driverId) const;
    
    // Trip management
    std::shared_ptr<Trip> startTrip(const std::string& bookingId);
    bool updateTripLocation(const std::string& tripId, double latitude, double longitude);
    std::shared_ptr<Trip> completeTrip(const std::string& tripId, double actualFare);
    bool cancelTrip(const std::string& tripId);
    std::vector<std::shared_ptr<Trip>> getCustomerTrips(const std::string& customerId) const;
    std::vector<std::shared_ptr<Trip>> getDriverTrips(const std::string& driverId) const;
    
    // Payment management
    std::shared_ptr<Payment> processPayment(const std::string& tripId, PaymentMethod method);
    bool refundPayment(const std::string& paymentId);
    std::vector<std::shared_ptr<Payment>> getCustomerPayments(const std::string& customerId) const;
    std::vector<std::shared_ptr<Payment>> getDriverPayments(const std::string& driverId) const;
    
    // Rating and feedback
    bool rateTrip(const std::string& tripId, double customerRating, const std::string& customerFeedback,
                  double driverRating, const std::string& driverFeedback);
    
    // System utilities
    void updateDriverLocation(const std::string& driverId, double latitude, double longitude);
    void updateVehicleLocation(const std::string& vehicleId, double latitude, double longitude);
    double calculateEstimatedFare(const std::string& pickupLocationId, const std::string& dropLocationId,
                                 const std::string& vehicleType);
    std::vector<std::shared_ptr<Vehicle>> getNearbyVehicles(const std::string& locationId, double radiusKm) const;
    
    // Getters for services
    std::shared_ptr<FareCalculator> getFareCalculator() const;
    std::shared_ptr<NotificationService> getNotificationService() const;
    
    // System statistics
    int getTotalCustomers() const;
    int getTotalDrivers() const;
    int getTotalVehicles() const;
    int getTotalTrips() const;
    int getTotalBookings() const;
    int getTotalPayments() const;
    double getTotalRevenue() const;
}; 