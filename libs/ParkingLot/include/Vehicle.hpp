#pragma once

#include <string>
#include "VehicleType.hpp"

class Vehicle {
    std::string licenseNumber;
    VehicleType type;

public:
    Vehicle(std::string l, VehicleType v);
    virtual ~Vehicle() = default;
    std::string getLicenseNumber() { return licenseNumber; }
    VehicleType getType() const { return type; }

    virtual int getRequiredSlots() const = 0;
};
