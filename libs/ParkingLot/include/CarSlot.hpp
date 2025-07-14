#pragma once

#include "ParkingSlot.hpp"
#include "Vehicle.hpp"
class CarSlot: public ParkingSlot {
public:
    explicit CarSlot(int id);
    bool canFitVehicle(const Vehicle& v) const override;
};