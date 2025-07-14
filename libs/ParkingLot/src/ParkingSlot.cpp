#include "../include/ParkingSlot.hpp"
#include <memory>

ParkingSlot::ParkingSlot(int id, SlotType slotType): slotId(id), type(slotType), occupied(false), parkedVehicle(nullptr) {}

bool ParkingSlot::parkVehicle(std::shared_ptr<Vehicle> v) {
    if(!isAvailable() || !canFitVehicle(*v)) {
        return false;
    }

    parkedVehicle = std::move(v);
    occupied = true;
    return true;
}

void ParkingSlot::unParkVehicle() {
    parkedVehicle.reset();
    occupied = false;
}