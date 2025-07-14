#include "../include/ParkingLevel.hpp"
#include "../include/CarSlot.hpp"
#include "../include/BikeSlot.hpp"
#include "../include/TruckSlot.hpp"
#include <algorithm>
#include <memory>
#include <string>

ParkingLevel::ParkingLevel(int floorNum, int carSlots, int bikeSlots, int truckSlots): floorNumber(floorNum) {
    int slotId = 1;

    for(int i = 0; i < carSlots; i++) {
        slots.push_back(std::make_shared<CarSlot>(slotId++));
    }

    for(int i = 0; i < bikeSlots; i++) {
        slots.push_back(std::make_shared<BikeSlot>(slotId++));
    }

    for(int i = 0; i < truckSlots; i++) {
        slots.push_back(std::make_shared<TruckSlot>(slotId++));
    }
}

std::shared_ptr<ParkingSlot> ParkingLevel::findAvailableSlot(const Vehicle& v) {
    for(auto &slot: slots) {
        if(slot->isAvailable() && slot->canFitVehicle(v)) {
            return slot;
        }
    }

    return nullptr;
}

bool ParkingLevel::parkVehicle(std::shared_ptr<Vehicle> v) {
    auto slot = findAvailableSlot(*v);
    if(!slot) {
        return false;
    }

    if(slot->parkVehicle(v)) {
        vehicleToSlot[v->getLicenseNumber()] = slot;
        return true;
    }

    return false;
}

bool ParkingLevel::unParkVehicle(const std::string& licenseNumber) {
    auto it = vehicleToSlot.find(licenseNumber);
    if(it == vehicleToSlot.end()) {
        return false;
    }

    it->second->unParkVehicle();
    vehicleToSlot.erase(it);
    return true;
}

int ParkingLevel::getAvailableSlots(SlotType type) const {
    return std::count_if(slots.begin(), slots.end(), [type](const std::shared_ptr<ParkingSlot>& slot) {
        return slot->getType() == type && !slot->isAvailable();
    });
}

int ParkingLevel::getTotalSlots(SlotType type) const {
    return std::count_if(slots.begin(), slots.end(), [type](const std::shared_ptr<ParkingSlot>& slot)  {
        return slot->getType() == type;
    });
}