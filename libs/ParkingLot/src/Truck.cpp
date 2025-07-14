#include "../include/Truck.hpp"
#include <string>

Truck::Truck(std::string license): Vehicle(std::move(license), VehicleType::TRUCK) {}

int Truck::getRequiredSlots() const {
    return 3;
}