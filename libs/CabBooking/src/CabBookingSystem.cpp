#include "../include/CabBookingSystem.hpp"

CabBookingSystem::CabBookingSystem() 
    : nextCustomerId(1), nextDriverId(1), nextVehicleId(1), nextTripId(1), 
      nextBookingId(1), nextPaymentId(1), nextLocationId(1) {
    
    fareCalculator = std::make_shared<FareCalculator>();
    notificationService = std::make_shared<NotificationService>();
}

// Customer management
std::shared_ptr<Customer> CabBookingSystem::registerCustomer(const std::string& name, const std::string& email,
                                                            const std::string& phone, const std::string& password) {
    std::string customerId = "CUST_" + std::to_string(nextCustomerId++);
    auto customer = std::make_shared<Customer>(customerId, name, email, phone, password);
    customers[customerId] = customer;
    return customer;
}

std::shared_ptr<Customer> CabBookingSystem::authenticateCustomer(const std::string& email, const std::string& password) {
    for (const auto& pair : customers) {
        if (pair.second->getEmail() == email && pair.second->authenticate(password)) {
            return pair.second;
        }
    }
    return nullptr;
}

bool CabBookingSystem::updateCustomerProfile(const std::string& customerId, const std::string& name,
                                            const std::string& email, const std::string& phone) {
    auto it = customers.find(customerId);
    if (it != customers.end()) {
        it->second->updateProfile(name, email, phone);
        return true;
    }
    return false;
}

std::vector<std::shared_ptr<Customer>> CabBookingSystem::getAllCustomers() const {
    std::vector<std::shared_ptr<Customer>> result;
    for (const auto& pair : customers) {
        result.push_back(pair.second);
    }
    return result;
}

// Driver management
std::shared_ptr<Driver> CabBookingSystem::registerDriver(const std::string& name, const std::string& email,
                                                        const std::string& phone, const std::string& password,
                                                        const std::string& license) {
    std::string driverId = "DRIVER_" + std::to_string(nextDriverId++);
    auto driver = std::make_shared<Driver>(driverId, name, email, phone, password, license);
    drivers[driverId] = driver;
    return driver;
}

std::shared_ptr<Driver> CabBookingSystem::authenticateDriver(const std::string& email, const std::string& password) {
    for (const auto& pair : drivers) {
        if (pair.second->getEmail() == email && pair.second->authenticate(password)) {
            return pair.second;
        }
    }
    return nullptr;
}

bool CabBookingSystem::updateDriverProfile(const std::string& driverId, const std::string& name,
                                          const std::string& email, const std::string& phone) {
    auto it = drivers.find(driverId);
    if (it != drivers.end()) {
        it->second->updateProfile(name, email, phone);
        return true;
    }
    return false;
}

std::vector<std::shared_ptr<Driver>> CabBookingSystem::getAvailableDrivers() const {
    std::vector<std::shared_ptr<Driver>> result;
    for (const auto& pair : drivers) {
        if (pair.second->canAcceptRide()) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Driver>> CabBookingSystem::getAllDrivers() const {
    std::vector<std::shared_ptr<Driver>> result;
    for (const auto& pair : drivers) {
        result.push_back(pair.second);
    }
    return result;
}

// Vehicle management
std::shared_ptr<Vehicle> CabBookingSystem::addVehicle(const std::string& licensePlate, const std::string& model,
                                                     const std::string& brand, int year, VehicleType type,
                                                     int capacity, double baseFare, double perKmRate, bool isAC) {
    std::string vehicleId = "VEH_" + std::to_string(nextVehicleId++);
    auto vehicle = std::make_shared<Vehicle>(vehicleId, licensePlate, model, brand, year, type, capacity, baseFare, perKmRate, isAC);
    vehicles[vehicleId] = vehicle;
    return vehicle;
}

std::shared_ptr<Vehicle> CabBookingSystem::assignVehicleToDriver(const std::string& vehicleId, const std::string& driverId) {
    auto vehicleIt = vehicles.find(vehicleId);
    auto driverIt = drivers.find(driverId);
    
    if (vehicleIt != vehicles.end() && driverIt != drivers.end()) {
        vehicleIt->second->setDriverId(driverId);
        driverIt->second->assignVehicle(vehicleId);
        return vehicleIt->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<Vehicle>> CabBookingSystem::getAvailableVehicles() const {
    std::vector<std::shared_ptr<Vehicle>> result;
    for (const auto& pair : vehicles) {
        if (pair.second->isAvailable()) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Vehicle>> CabBookingSystem::getAllVehicles() const {
    std::vector<std::shared_ptr<Vehicle>> result;
    for (const auto& pair : vehicles) {
        result.push_back(pair.second);
    }
    return result;
}

// Location management
std::shared_ptr<Location> CabBookingSystem::addLocation(const std::string& address, double latitude, double longitude,
                                                       const std::string& city, const std::string& state,
                                                       const std::string& country, const std::string& postalCode) {
    std::string locationId = "LOC_" + std::to_string(nextLocationId++);
    auto location = std::make_shared<Location>(locationId, address, latitude, longitude, city, state, country, postalCode);
    locations[locationId] = location;
    return location;
}

std::vector<std::shared_ptr<Location>> CabBookingSystem::searchLocations(const std::string& query) const {
    std::vector<std::shared_ptr<Location>> result;
    for (const auto& pair : locations) {
        if (pair.second->getAddress().find(query) != std::string::npos ||
            pair.second->getCity().find(query) != std::string::npos) {
            result.push_back(pair.second);
        }
    }
    return result;
}

// Booking management
std::shared_ptr<Booking> CabBookingSystem::createBooking(const std::string& customerId, const std::string& pickupLocationId,
                                                        const std::string& dropLocationId, const std::string& vehicleType) {
    auto customerIt = customers.find(customerId);
    auto pickupIt = locations.find(pickupLocationId);
    auto dropIt = locations.find(dropLocationId);
    
    if (customerIt == customers.end() || pickupIt == locations.end() || dropIt == locations.end()) {
        return nullptr;
    }
    
    // Calculate estimated fare
    double estimatedFare = calculateEstimatedFare(pickupLocationId, dropLocationId, vehicleType);
    
    std::string bookingId = "BOOK_" + std::to_string(nextBookingId++);
    auto booking = std::make_shared<Booking>(bookingId, customerId, pickupIt->second, dropIt->second, vehicleType, estimatedFare);
    bookings[bookingId] = booking;
    
    // Update customer status
    customerIt->second->setHasActiveBooking(true);
    
    return booking;
}

std::shared_ptr<Driver> CabBookingSystem::findNearestDriver(const std::string& pickupLocationId, const std::string& vehicleType) {
    auto pickupIt = locations.find(pickupLocationId);
    if (pickupIt == locations.end()) {
        return nullptr;
    }
    
    std::shared_ptr<Driver> nearestDriver = nullptr;
    double minDistance = std::numeric_limits<double>::max();
    
    for (const auto& pair : drivers) {
        if (!pair.second->canAcceptRide()) {
            continue;
        }
        
        // Check if driver has the required vehicle type
        auto vehicleIt = vehicles.find(pair.second->getVehicleId());
        if (vehicleIt == vehicles.end() || vehicleIt->second->getVehicleTypeString() != vehicleType) {
            continue;
        }
        
        double distance = pickupIt->second->calculateDistance(Location("temp", "", 
                                                                      pair.second->getCurrentLatitude(), 
                                                                      pair.second->getCurrentLongitude(), "", "", "", ""));
        
        if (distance < minDistance) {
            minDistance = distance;
            nearestDriver = pair.second;
        }
    }
    
    return nearestDriver;
}

bool CabBookingSystem::assignDriverToBooking(const std::string& bookingId, const std::string& driverId) {
    auto bookingIt = bookings.find(bookingId);
    auto driverIt = drivers.find(driverId);
    
    if (bookingIt == bookings.end() || driverIt == drivers.end()) {
        return false;
    }
    
    auto vehicleIt = vehicles.find(driverIt->second->getVehicleId());
    if (vehicleIt == vehicles.end()) {
        return false;
    }
    
    bookingIt->second->assignDriver(driverId, vehicleIt->second->getVehicleId());
    return true;
}

bool CabBookingSystem::cancelBooking(const std::string& bookingId, const std::string& reason) {
    auto bookingIt = bookings.find(bookingId);
    if (bookingIt == bookings.end()) {
        return false;
    }
    
    bookingIt->second->cancelBooking();
    
    // Update customer status
    auto customerIt = customers.find(bookingIt->second->getCustomerId());
    if (customerIt != customers.end()) {
        customerIt->second->setHasActiveBooking(false);
    }
    
    // Send notification
    notificationService->notifyBookingCancelled(bookingIt->second->getCustomerId(), reason);
    
    return true;
}

std::vector<std::shared_ptr<Booking>> CabBookingSystem::getCustomerBookings(const std::string& customerId) const {
    std::vector<std::shared_ptr<Booking>> result;
    for (const auto& pair : bookings) {
        if (pair.second->getCustomerId() == customerId) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Booking>> CabBookingSystem::getDriverBookings(const std::string& driverId) const {
    std::vector<std::shared_ptr<Booking>> result;
    for (const auto& pair : bookings) {
        if (pair.second->getDriverId() == driverId) {
            result.push_back(pair.second);
        }
    }
    return result;
}

// Trip management
std::shared_ptr<Trip> CabBookingSystem::startTrip(const std::string& bookingId) {
    auto bookingIt = bookings.find(bookingId);
    if (bookingIt == bookings.end()) {
        return nullptr;
    }
    
    std::string tripId = "TRIP_" + std::to_string(nextTripId++);
    auto trip = std::make_shared<Trip>(tripId, bookingIt->second->getCustomerId(),
                                      bookingIt->second->getPickupLocation(),
                                      bookingIt->second->getDropLocation());
    
    trip->setDriverId(bookingIt->second->getDriverId());
    trip->setVehicleId(bookingIt->second->getVehicleId());
    trip->setStatus(TripStatus::ACCEPTED);
    
    trips[tripId] = trip;
    bookingIt->second->setTrip(trip);
    
    // Send notifications
    auto driverIt = drivers.find(bookingIt->second->getDriverId());
    if (driverIt != drivers.end()) {
        notificationService->notifyTripStarted(bookingIt->second->getCustomerId(), driverIt->second->getName());
    }
    
    return trip;
}

bool CabBookingSystem::updateTripLocation(const std::string& tripId, double latitude, double longitude) {
    auto tripIt = trips.find(tripId);
    if (tripIt == trips.end()) {
        return false;
    }
    
    // Update driver location
    auto driverIt = drivers.find(tripIt->second->getDriverId());
    if (driverIt != drivers.end()) {
        driverIt->second->updateLocation(latitude, longitude);
    }
    
    // Update vehicle location
    auto vehicleIt = vehicles.find(tripIt->second->getVehicleId());
    if (vehicleIt != vehicles.end()) {
        vehicleIt->second->updateLocation(latitude, longitude);
    }
    
    return true;
}

std::shared_ptr<Trip> CabBookingSystem::completeTrip(const std::string& tripId, double actualFare) {
    auto tripIt = trips.find(tripId);
    if (tripIt == trips.end()) {
        return nullptr;
    }
    
    tripIt->second->completeTrip();
    tripIt->second->setActualFare(actualFare);
    
    // Update customer status
    auto customerIt = customers.find(tripIt->second->getCustomerId());
    if (customerIt != customers.end()) {
        customerIt->second->setHasActiveBooking(false);
        customerIt->second->addTripToHistory(tripIt->second);
        customerIt->second->incrementTotalTrips();
    }
    
    // Update driver
    auto driverIt = drivers.find(tripIt->second->getDriverId());
    if (driverIt != drivers.end()) {
        driverIt->second->addCompletedTrip(tripIt->second);
        driverIt->second->addEarnings(actualFare * 0.85); // Driver gets 85% of fare
        driverIt->second->incrementTotalTrips();
    }
    
    // Send notification
    notificationService->notifyTripCompleted(tripIt->second->getCustomerId(), 
                                            tripIt->second->getDriverId(), 
                                            actualFare, "CASH");
    
    return tripIt->second;
}

bool CabBookingSystem::cancelTrip(const std::string& tripId) {
    auto tripIt = trips.find(tripId);
    if (tripIt == trips.end()) {
        return false;
    }
    
    tripIt->second->cancelTrip();
    
    // Update customer status
    auto customerIt = customers.find(tripIt->second->getCustomerId());
    if (customerIt != customers.end()) {
        customerIt->second->setHasActiveBooking(false);
    }
    
    return true;
}

std::vector<std::shared_ptr<Trip>> CabBookingSystem::getCustomerTrips(const std::string& customerId) const {
    std::vector<std::shared_ptr<Trip>> result;
    for (const auto& pair : trips) {
        if (pair.second->getCustomerId() == customerId) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Trip>> CabBookingSystem::getDriverTrips(const std::string& driverId) const {
    std::vector<std::shared_ptr<Trip>> result;
    for (const auto& pair : trips) {
        if (pair.second->getDriverId() == driverId) {
            result.push_back(pair.second);
        }
    }
    return result;
}

// Payment management
std::shared_ptr<Payment> CabBookingSystem::processPayment(const std::string& tripId, PaymentMethod method) {
    auto tripIt = trips.find(tripId);
    if (tripIt == trips.end()) {
        return nullptr;
    }
    
    std::string paymentId = "PAY_" + std::to_string(nextPaymentId++);
    auto payment = std::make_shared<Payment>(paymentId, tripId, tripIt->second->getCustomerId(),
                                            tripIt->second->getDriverId(), tripIt->second->getActualFare(), method);
    
    payment->processPayment();
    payments[paymentId] = payment;
    
    tripIt->second->setIsPaid(true);
    tripIt->second->setPaymentMethod(payment->getMethodString());
    
    // Send notification
    notificationService->notifyPaymentSuccess(tripIt->second->getCustomerId(),
                                             tripIt->second->getDriverId(),
                                             payment->getAmount(),
                                             payment->getTransactionId());
    
    return payment;
}

bool CabBookingSystem::refundPayment(const std::string& paymentId) {
    auto paymentIt = payments.find(paymentId);
    if (paymentIt == payments.end()) {
        return false;
    }
    
    paymentIt->second->refund();
    return true;
}

std::vector<std::shared_ptr<Payment>> CabBookingSystem::getCustomerPayments(const std::string& customerId) const {
    std::vector<std::shared_ptr<Payment>> result;
    for (const auto& pair : payments) {
        if (pair.second->getCustomerId() == customerId) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Payment>> CabBookingSystem::getDriverPayments(const std::string& driverId) const {
    std::vector<std::shared_ptr<Payment>> result;
    for (const auto& pair : payments) {
        if (pair.second->getDriverId() == driverId) {
            result.push_back(pair.second);
        }
    }
    return result;
}

// Rating and feedback
bool CabBookingSystem::rateTrip(const std::string& tripId, double customerRating, const std::string& customerFeedback,
                                double driverRating, const std::string& driverFeedback) {
    auto tripIt = trips.find(tripId);
    if (tripIt == trips.end()) {
        return false;
    }
    
    tripIt->second->setCustomerRating(customerRating);
    tripIt->second->setCustomerFeedback(customerFeedback);
    tripIt->second->setDriverRating(driverRating);
    tripIt->second->setDriverFeedback(driverFeedback);
    
    // Update user ratings
    auto customerIt = customers.find(tripIt->second->getCustomerId());
    if (customerIt != customers.end()) {
        // Calculate new average rating
        auto customerTrips = getCustomerTrips(tripIt->second->getCustomerId());
        double totalRating = 0.0;
        int ratedTrips = 0;
        
        for (const auto& trip : customerTrips) {
            if (trip->getCustomerRating() > 0) {
                totalRating += trip->getCustomerRating();
                ratedTrips++;
            }
        }
        
        if (ratedTrips > 0) {
            customerIt->second->setRating(totalRating / ratedTrips);
        }
    }
    
    auto driverIt = drivers.find(tripIt->second->getDriverId());
    if (driverIt != drivers.end()) {
        driverIt->second->setRating(driverIt->second->getAverageRating());
    }
    
    return true;
}

// System utilities
void CabBookingSystem::updateDriverLocation(const std::string& driverId, double latitude, double longitude) {
    auto driverIt = drivers.find(driverId);
    if (driverIt != drivers.end()) {
        driverIt->second->updateLocation(latitude, longitude);
        
        // Update vehicle location if assigned
        auto vehicleIt = vehicles.find(driverIt->second->getVehicleId());
        if (vehicleIt != vehicles.end()) {
            vehicleIt->second->updateLocation(latitude, longitude);
        }
    }
}

void CabBookingSystem::updateVehicleLocation(const std::string& vehicleId, double latitude, double longitude) {
    auto vehicleIt = vehicles.find(vehicleId);
    if (vehicleIt != vehicles.end()) {
        vehicleIt->second->updateLocation(latitude, longitude);
    }
}

double CabBookingSystem::calculateEstimatedFare(const std::string& pickupLocationId, const std::string& dropLocationId,
                                               const std::string& vehicleType) {
    auto pickupIt = locations.find(pickupLocationId);
    auto dropIt = locations.find(dropLocationId);
    
    if (pickupIt == locations.end() || dropIt == locations.end()) {
        return 0.0;
    }
    
    // Create a temporary vehicle for fare calculation
    auto tempVehicle = std::make_shared<Vehicle>("temp", "", "", "", 2020, VehicleType::SEDAN, 4, 50.0, 12.0);
    
    return fareCalculator->calculateFare(pickupIt->second, dropIt->second, tempVehicle);
}

std::vector<std::shared_ptr<Vehicle>> CabBookingSystem::getNearbyVehicles(const std::string& locationId, double radiusKm) const {
    auto locationIt = locations.find(locationId);
    if (locationIt == locations.end()) {
        return {};
    }
    
    std::vector<std::shared_ptr<Vehicle>> result;
    for (const auto& pair : vehicles) {
        if (!pair.second->isAvailable()) {
            continue;
        }
        
        double distance = locationIt->second->calculateDistance(Location("temp", "", 
                                                                        pair.second->getCurrentLatitude(), 
                                                                        pair.second->getCurrentLongitude(), "", "", "", ""));
        
        if (distance <= radiusKm) {
            result.push_back(pair.second);
        }
    }
    
    return result;
}

// Getters for services
std::shared_ptr<FareCalculator> CabBookingSystem::getFareCalculator() const {
    return fareCalculator;
}

std::shared_ptr<NotificationService> CabBookingSystem::getNotificationService() const {
    return notificationService;
}

// System statistics
int CabBookingSystem::getTotalCustomers() const {
    return customers.size();
}

int CabBookingSystem::getTotalDrivers() const {
    return drivers.size();
}

int CabBookingSystem::getTotalVehicles() const {
    return vehicles.size();
}

int CabBookingSystem::getTotalTrips() const {
    return trips.size();
}

int CabBookingSystem::getTotalBookings() const {
    return bookings.size();
}

int CabBookingSystem::getTotalPayments() const {
    return payments.size();
}

double CabBookingSystem::getTotalRevenue() const {
    double totalRevenue = 0.0;
    for (const auto& pair : payments) {
        if (pair.second->isSuccessful()) {
            totalRevenue += pair.second->getPlatformFee();
        }
    }
    return totalRevenue;
} 