#include "../include/Bike.hpp"
#include <string>

Bike::Bike(std::string license): Vehicle(std::move(license), VehicleType::BIKE) {}

int Bike::getRequiredSlots() const {
    return 1;
}