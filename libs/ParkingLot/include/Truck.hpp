#pragma once

#include "Vehicle.hpp"
#include <string>

class Truck: public Vehicle {
public:
    explicit Truck(std::string license);
    int getRequiredSlots() const override;
};