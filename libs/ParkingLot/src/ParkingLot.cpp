#include "../include/ParkingLot.hpp"
#include "../include/Strategy.hpp"
#include "../include/Payment.hpp"
#include <chrono>
#include <memory>
#include <mutex>
#include <random>
#include <sstream>

ParkingLot::ParkingLot(std::string lotName, int levelNums, int slotsPerLevel, double rate): name(std::move(lotName)), hourlyRate(rate) {
    int carSlots = slotsPerLevel / 2;
    int bikeSlots = slotsPerLevel / 3;
    int truckSlots = slotsPerLevel - carSlots - bikeSlots;

    for(int i = 1; i <= levelNums; i++) {
        levels.push_back(std::make_shared<ParkingLevel>(i, carSlots, bikeSlots, truckSlots));
    }

    pricingStrategy = std::make_unique<StandardPricing>();
}

std::shared_ptr<Ticket> ParkingLot::parkVehicle(std::shared_ptr<Vehicle> v) {
    std::lock_guard<std::mutex> lock(parkingMutex);

    if(isVehicleParked(v->getLicenseNumber())) {
        return nullptr;
    }

    for(auto &level: levels) {
        if(level->parkVehicle(v)) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1000, 9999);

            std::stringstream ss;
            ss << "TKT" << dis(gen);
            std::string ticketId = ss.str();

            auto ticket = std::make_shared<Ticket>(ticketId, v->getLicenseNumber());
            activeTickets[v->getLicenseNumber()] = ticket;

            return ticket;
        }
    }

    return nullptr;
}

bool ParkingLot::unParkVehicle(const std::string& licenseNumber) {
    std::lock_guard<std::mutex> lock(parkingMutex);

    auto ticketIt = activeTickets.find(licenseNumber);
    if(ticketIt == activeTickets.end()) {
        return false;
    }

    for(auto &level: levels) {
        if(level->unParkVehicle(licenseNumber)) {
            ticketIt->second->setExitTime();
            activeTickets.erase(ticketIt);
            return true;
        }
    }

    return false;
}

double ParkingLot::calculateFee(const std::string& ticketId) {
    std::lock_guard<std::mutex> lock(parkingMutex);

    for(auto &[license, ticket]: activeTickets) {
        if(ticket->getTicketId() == ticketId) {
            return ticket->calculateAmount(hourlyRate);
        }
    }

    return 0.0;
}

std::shared_ptr<Payment> ParkingLot::processPayment(const std::string& ticketId, PaymentMethod method) {
    double amount = calculateFee(ticketId);
    if(amount <= 0) {
        return nullptr;
    }

    std::stringstream ss;
    ss << "PAY" << std::chrono::system_clock::now().time_since_epoch().count();
    std::string paymentId = ss.str();

    auto payment = std::make_shared<Payment>(paymentId, amount, method, ticketId);

    if(payment->processPayment()) {
        for(auto &[license, ticket]: activeTickets) {
            if(ticket->getTicketId() == ticketId) {
                ticket->markAsPaid();
                break;
            }
        }
        return payment;
    }

    return nullptr;
}

int ParkingLot::getAvailableSlots() const {
    std::lock_guard<std::mutex> lock(parkingMutex);

    int total = 0;
    for(const auto &level: levels) {
        total += level->getAvailableSlots(SlotType::CAR) + level->getAvailableSlots(SlotType::BIKE) + level->getAvailableSlots(SlotType::TRUCK);
    }

    return total;
}

bool ParkingLot::isVehicleParked(const std::string& licenseNumber) const {
    std::lock_guard<std::mutex> lock(parkingMutex);
    return activeTickets.find(licenseNumber) != activeTickets.end();
}

void ParkingLot::setPricingStrategy(std::unique_ptr<PricingStrategy> strategy) {
    std::lock_guard<std::mutex> lock(parkingMutex);
    pricingStrategy = std::move(strategy);
}