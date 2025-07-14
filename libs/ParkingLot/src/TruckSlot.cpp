#include "../include/TruckSlot.hpp"

TruckSlot::TruckSlot(int id): ParkingSlot(id, SlotType::TRUCK) {}

bool TruckSlot::canFitVehicle(const Vehicle& v) const {
    return v.getType() == VehicleType::TRUCK;
}