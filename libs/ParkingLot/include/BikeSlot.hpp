#pragma once

#include "ParkingSlot.hpp"
#include "Vehicle.hpp"
class BikeSlot: public ParkingSlot {
public:
    explicit BikeSlot(int id);
    bool canFitVehicle(const Vehicle& v) const override;
};