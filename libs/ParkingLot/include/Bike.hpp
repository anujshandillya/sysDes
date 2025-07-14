#pragma once

#include "Vehicle.hpp"
#include <string>

class Bike: public Vehicle {
public:
    explicit Bike(std::string license);
    int getRequiredSlots() const override;
};