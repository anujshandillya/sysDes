#pragma once

#include "ParkingSlot.hpp"
#include "Vehicle.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
class ParkingLevel {
    int floorNumber;
    std::vector<std::shared_ptr<ParkingSlot>> slots;
    std::unordered_map<std::string, std::shared_ptr<ParkingSlot>> vehicleToSlot;

public:
    ParkingLevel(int floorNumber, int carSlots, int bikeSlots, int truckSlots);

    int getFloorNumber() const { return floorNumber; }
    std::shared_ptr<ParkingSlot> findAvailableSlot(const Vehicle& v);
    bool parkVehicle(std::shared_ptr<Vehicle> v);
    bool unParkVehicle(const std::string& licenseNumber);

    int getAvailableSlots(SlotType type) const;
    int getTotalSlots(SlotType type) const;
};