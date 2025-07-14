#pragma once

#include "ParkingLevel.hpp"
#include "Payment.hpp"
#include "Strategy.hpp"
#include "Ticket.hpp"
#include "Vehicle.hpp"
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
class ParkingLot {
    std::string name;
    std::vector<std::shared_ptr<ParkingLevel>> levels;
    std::unordered_map<std::string, std::shared_ptr<Ticket>> activeTickets;
    std::unique_ptr<PricingStrategy> pricingStrategy;
    double hourlyRate;
    mutable std::mutex parkingMutex;

public:
    ParkingLot(std::string lotName, int levelNums, int slotsPerLevel, double rate);

    std::string getName() const { return name; }

    // parking operations
    std::shared_ptr<Ticket> parkVehicle(std::shared_ptr<Vehicle> v);
    bool unParkVehicle(const std::string& license);

    // payment operations
    double calculateFee(const std::string& ticketId);
    std::shared_ptr<Payment> processPayment(const std::string& ticketId, PaymentMethod method);

    // status queries
    int getAvailableSlots() const;
    int getTotalSlots() const;
    bool isVehicleParked(const std::string& license) const;

    // strategy pattern for pricing
    void setPricingStrategy(std::unique_ptr<PricingStrategy> strategy);
};