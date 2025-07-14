#pragma once

#include "Vehicle.hpp"

class Car: public Vehicle {
public:
    explicit Car(std::string license);
    int getRequiredSlots() const override;
};