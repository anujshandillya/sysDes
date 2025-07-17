#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <string>
#include "Floor.hpp"
#include "Request.hpp"

enum class ElevatorState {
    IDLE,
    MOVING_UP,
    MOVING_DOWN,
    DOOR_OPENING,
    DOOR_OPEN,
    DOOR_CLOSING,
    MAINTENANCE,
    EMERGENCY
};

enum class DoorState {
    OPEN,
    CLOSED,
    OPENING,
    CLOSING
};

class Elevator {
private:
    std::string elevatorId;
    int currentFloor;
    Direction direction;
    ElevatorState state;
    DoorState doorState;
    int maxFloor;
    int minFloor;
    double speed; // floors per second
    double doorOpenTime; // seconds
    double doorCloseTime; // seconds
    int capacity; // maximum number of passengers
    int currentLoad; // current number of passengers
    std::vector<bool> floorButtons; // Internal floor buttons
    std::queue<std::shared_ptr<Request>> requestQueue;
    std::vector<std::shared_ptr<Request>> completedRequests;
    std::chrono::system_clock::time_point lastStateChange;
    bool isOperational;

public:
    Elevator(const std::string& elevatorId, int maxFloor, int minFloor = 1, 
             double speed = 1.0, double doorOpenTime = 3.0, double doorCloseTime = 3.0,
             int capacity = 8);

    // Getters
    std::string getElevatorId() const { return elevatorId; }
    int getCurrentFloor() const { return currentFloor; }
    Direction getDirection() const { return direction; }
    ElevatorState getState() const { return state; }
    DoorState getDoorState() const { return doorState; }
    int getMaxFloor() const { return maxFloor; }
    int getMinFloor() const { return minFloor; }
    double getSpeed() const { return speed; }
    double getDoorOpenTime() const { return doorOpenTime; }
    double getDoorCloseTime() const { return doorCloseTime; }
    int getCapacity() const { return capacity; }
    int getCurrentLoad() const { return currentLoad; }
    bool getIsOperational() const { return isOperational; }
    std::queue<std::shared_ptr<Request>> getRequestQueue() const { return requestQueue; }
    std::vector<std::shared_ptr<Request>> getCompletedRequests() const { return completedRequests; }

    // Setters
    void setCurrentFloor(int floor) { currentFloor = floor; }
    void setDirection(Direction dir) { direction = dir; }
    void setState(ElevatorState newState);
    void setDoorState(DoorState newState) { doorState = newState; }
    void setCurrentLoad(int load) { currentLoad = load; }
    void setIsOperational(bool operational) { isOperational = operational; }

    // Request management
    bool addRequest(const std::shared_ptr<Request>& request);
    std::shared_ptr<Request> getNextRequest();
    void completeRequest(const std::shared_ptr<Request>& request);
    void removeRequest(const std::shared_ptr<Request>& request);
    bool hasRequests() const { return !requestQueue.empty(); }
    int getRequestCount() const { return requestQueue.size(); }

    // Floor button management
    void pressFloorButton(int floor);
    void clearFloorButton(int floor);
    bool isFloorButtonPressed(int floor) const;
    std::vector<int> getPressedFloorButtons() const;

    // Movement and operation
    void move();
    void openDoor();
    void closeDoor();
    void stop();
    void emergencyStop();
    bool canServeFloor(int floor) const;
    bool canServeDirection(Direction dir) const;
    int getDistanceToFloor(int floor) const;
    bool isIdle() const { return state == ElevatorState::IDLE; }
    bool isMoving() const { return state == ElevatorState::MOVING_UP || state == ElevatorState::MOVING_DOWN; }
    bool isDoorOpen() const { return doorState == DoorState::OPEN; }

    // Utility methods
    std::string getStateString() const;
    std::string getDirectionString() const;
    std::string getDoorStateString() const;
    double getUtilizationRate() const; // Returns utilization percentage
    int getTotalTrips() const { return completedRequests.size(); }
}; 