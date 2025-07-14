#pragma once

#include "Vehicle.hpp"
#include <memory>
enum class SlotType { CAR, BIKE, TRUCK };

class ParkingSlot {
protected:
    int slotId;
    SlotType type;
    bool occupied;
    std::shared_ptr<Vehicle> parkedVehicle;

public:
    ParkingSlot(int id, SlotType slotType);
    virtual ~ParkingSlot() = default;

    int getSlotId() const { return slotId; }
    SlotType getType() const { return type; }
    bool isAvailable() const  { return !occupied; }

    virtual bool canFitVehicle(const Vehicle& v) const = 0;
    bool parkVehicle(std::shared_ptr<Vehicle> v);
    void unParkVehicle();

    std::shared_ptr<Vehicle> getParkedVehicle() const { return parkedVehicle; }
};