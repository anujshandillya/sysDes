#pragma once

#include "ParkingSlot.hpp"
#include "Vehicle.hpp"
class TruckSlot: public ParkingSlot {
public:
    explicit TruckSlot(int id);
    bool canFitVehicle(const Vehicle& v) const override;
};