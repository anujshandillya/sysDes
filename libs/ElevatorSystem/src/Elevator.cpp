#include "../include/Elevator.hpp"
#include <chrono>

Elevator::Elevator(const std::string& elevatorId, int maxFloor, int minFloor, 
                   double speed, double doorOpenTime, double doorCloseTime, int capacity)
    : elevatorId(elevatorId), currentFloor(minFloor), direction(Direction::NONE),
      state(ElevatorState::IDLE), doorState(DoorState::CLOSED), maxFloor(maxFloor),
      minFloor(minFloor), speed(speed), doorOpenTime(doorOpenTime), 
      doorCloseTime(doorCloseTime), capacity(capacity), currentLoad(0),
      lastStateChange(std::chrono::system_clock::now()), isOperational(true) {
    
    // Initialize floor buttons
    floorButtons.resize(maxFloor + 1, false);
}

void Elevator::setState(ElevatorState newState) {
    state = newState;
    lastStateChange = std::chrono::system_clock::now();
}

bool Elevator::addRequest(const std::shared_ptr<Request>& request) {
    if (!isOperational || currentLoad >= capacity) {
        return false;
    }
    
    requestQueue.push(request);
    return true;
}

std::shared_ptr<Request> Elevator::getNextRequest() {
    if (requestQueue.empty()) {
        return nullptr;
    }
    
    auto request = requestQueue.front();
    requestQueue.pop();
    return request;
}

void Elevator::completeRequest(const std::shared_ptr<Request>& request) {
    request->setStatus(RequestStatus::COMPLETED);
    request->setCompletionTime(std::chrono::system_clock::now());
    completedRequests.push_back(request);
}

void Elevator::removeRequest(const std::shared_ptr<Request>& request) {
    // This is a simplified implementation
    // In a real system, you'd need to search and remove from the queue
    std::queue<std::shared_ptr<Request>> tempQueue;
    
    while (!requestQueue.empty()) {
        auto req = requestQueue.front();
        requestQueue.pop();
        if (req->getRequestId() != request->getRequestId()) {
            tempQueue.push(req);
        }
    }
    
    requestQueue = tempQueue;
}

void Elevator::pressFloorButton(int floor) {
    if (floor >= minFloor && floor <= maxFloor) {
        floorButtons[floor] = true;
    }
}

void Elevator::clearFloorButton(int floor) {
    if (floor >= minFloor && floor <= maxFloor) {
        floorButtons[floor] = false;
    }
}

bool Elevator::isFloorButtonPressed(int floor) const {
    if (floor >= minFloor && floor <= maxFloor) {
        return floorButtons[floor];
    }
    return false;
}

std::vector<int> Elevator::getPressedFloorButtons() const {
    std::vector<int> pressedFloors;
    for (int i = minFloor; i <= maxFloor; ++i) {
        if (floorButtons[i]) {
            pressedFloors.push_back(i);
        }
    }
    return pressedFloors;
}

void Elevator::move() {
    if (!isOperational || state != ElevatorState::MOVING_UP && state != ElevatorState::MOVING_DOWN) {
        return;
    }
    
    if (state == ElevatorState::MOVING_UP && currentFloor < maxFloor) {
        currentFloor++;
    } else if (state == ElevatorState::MOVING_DOWN && currentFloor > minFloor) {
        currentFloor--;
    }
}

void Elevator::openDoor() {
    if (state == ElevatorState::IDLE || state == ElevatorState::DOOR_CLOSING) {
        setState(ElevatorState::DOOR_OPENING);
        doorState = DoorState::OPENING;
        
        // Simulate door opening time
        auto now = std::chrono::system_clock::now();
        auto duration = now - lastStateChange;
        if (std::chrono::duration_cast<std::chrono::seconds>(duration).count() >= doorOpenTime) {
            doorState = DoorState::OPEN;
            setState(ElevatorState::DOOR_OPEN);
        }
    }
}

void Elevator::closeDoor() {
    if (state == ElevatorState::DOOR_OPEN) {
        setState(ElevatorState::DOOR_CLOSING);
        doorState = DoorState::CLOSING;
        
        // Simulate door closing time
        auto now = std::chrono::system_clock::now();
        auto duration = now - lastStateChange;
        if (std::chrono::duration_cast<std::chrono::seconds>(duration).count() >= doorCloseTime) {
            doorState = DoorState::CLOSED;
            setState(ElevatorState::IDLE);
        }
    }
}

void Elevator::stop() {
    if (isMoving()) {
        setState(ElevatorState::IDLE);
        direction = Direction::NONE;
    }
}

void Elevator::emergencyStop() {
    setState(ElevatorState::EMERGENCY);
    isOperational = false;
}

bool Elevator::canServeFloor(int floor) const {
    return floor >= minFloor && floor <= maxFloor && isOperational;
}

bool Elevator::canServeDirection(Direction dir) const {
    if (!isOperational) return false;
    
    if (state == ElevatorState::IDLE) return true;
    
    if (state == ElevatorState::MOVING_UP && dir == Direction::UP) return true;
    if (state == ElevatorState::MOVING_DOWN && dir == Direction::DOWN) return true;
    
    return false;
}

int Elevator::getDistanceToFloor(int floor) const {
    return std::abs(currentFloor - floor);
}

std::string Elevator::getStateString() const {
    switch (state) {
        case ElevatorState::IDLE: return "IDLE";
        case ElevatorState::MOVING_UP: return "MOVING_UP";
        case ElevatorState::MOVING_DOWN: return "MOVING_DOWN";
        case ElevatorState::DOOR_OPENING: return "DOOR_OPENING";
        case ElevatorState::DOOR_OPEN: return "DOOR_OPEN";
        case ElevatorState::DOOR_CLOSING: return "DOOR_CLOSING";
        case ElevatorState::MAINTENANCE: return "MAINTENANCE";
        case ElevatorState::EMERGENCY: return "EMERGENCY";
        default: return "UNKNOWN";
    }
}

std::string Elevator::getDirectionString() const {
    switch (direction) {
        case Direction::UP: return "UP";
        case Direction::DOWN: return "DOWN";
        case Direction::NONE: return "NONE";
        default: return "UNKNOWN";
    }
}

std::string Elevator::getDoorStateString() const {
    switch (doorState) {
        case DoorState::OPEN: return "OPEN";
        case DoorState::CLOSED: return "CLOSED";
        case DoorState::OPENING: return "OPENING";
        case DoorState::CLOSING: return "CLOSING";
        default: return "UNKNOWN";
    }
}

double Elevator::getUtilizationRate() const {
    if (completedRequests.empty()) return 0.0;
    
    auto now = std::chrono::system_clock::now();
    auto totalTime = std::chrono::duration_cast<std::chrono::seconds>(now - lastStateChange).count();
    
    if (totalTime == 0) return 0.0;
    
    // Calculate utilization based on completed requests and time
    return (completedRequests.size() * 100.0) / totalTime;
} 