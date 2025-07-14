#include "../include/Car.hpp"
#include <string>

Car::Car(std::string license): Vehicle(std::move(license), VehicleType::CAR) {}

int Car::getRequiredSlots() const {
    return 1;
}