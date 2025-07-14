#include "../include/BikeSlot.hpp"

BikeSlot::BikeSlot(int id): ParkingSlot(id, SlotType::BIKE) {}

bool BikeSlot::canFitVehicle(const Vehicle& v) const {
    return v.getType() == VehicleType::BIKE;
}