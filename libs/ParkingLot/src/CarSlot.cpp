#include "../include/CarSlot.hpp"

CarSlot::CarSlot(int id): ParkingSlot(id, SlotType::CAR) {}

bool CarSlot::canFitVehicle(const Vehicle& v) const {
    return v.getType() == VehicleType::CAR;
}