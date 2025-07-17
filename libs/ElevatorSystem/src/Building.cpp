#include "../include/Building.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>

Building::Building(const std::string& buildingId, const std::string& buildingName, 
                   int totalFloors, int basementFloors)
    : buildingId(buildingId), buildingName(buildingName), totalFloors(totalFloors),
      basementFloors(basementFloors), isSimulationRunning(false) {
    
    initializeFloors();
    scheduler = std::make_shared<Scheduler>();
}

void Building::setTotalFloors(int floors) {
    totalFloors = floors;
    initializeFloors();
}

void Building::setBasementFloors(int basements) {
    basementFloors = basements;
    initializeFloors();
}

void Building::initializeFloors() {
    floors.clear();
    for (int i = -basementFloors; i <= totalFloors; ++i) {
        if (i != 0) { // Skip floor 0
            floors.push_back(std::make_shared<Floor>(i));
        }
    }
}

std::shared_ptr<Floor> Building::getFloor(int floorNumber) const {
    for (const auto& floor : floors) {
        if (floor->getFloorNumber() == floorNumber) {
            return floor;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Floor>> Building::getFloorsWithRequests() const {
    std::vector<std::shared_ptr<Floor>> floorsWithRequests;
    for (const auto& floor : floors) {
        if (floor->hasAnyRequest()) {
            floorsWithRequests.push_back(floor);
        }
    }
    return floorsWithRequests;
}

void Building::addElevator(const std::shared_ptr<Elevator>& elevator) {
    elevators.push_back(elevator);
    if (scheduler) {
        scheduler->addElevator(elevator);
    }
}

void Building::removeElevator(const std::string& elevatorId) {
    elevators.erase(
        std::remove_if(elevators.begin(), elevators.end(),
            [&elevatorId](const std::shared_ptr<Elevator>& elevator) {
                return elevator->getElevatorId() == elevatorId;
            }),
        elevators.end()
    );
    
    if (scheduler) {
        scheduler->removeElevator(elevatorId);
    }
}

std::shared_ptr<Elevator> Building::getElevator(const std::string& elevatorId) const {
    for (const auto& elevator : elevators) {
        if (elevator->getElevatorId() == elevatorId) {
            return elevator;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Elevator>> Building::getOperationalElevators() const {
    std::vector<std::shared_ptr<Elevator>> operational;
    for (const auto& elevator : elevators) {
        if (elevator->getIsOperational()) {
            operational.push_back(elevator);
        }
    }
    return operational;
}

void Building::setScheduler(const std::shared_ptr<Scheduler>& newScheduler) {
    scheduler = newScheduler;
    
    // Add all existing elevators to the new scheduler
    for (const auto& elevator : elevators) {
        scheduler->addElevator(elevator);
    }
}

void Building::setSchedulingAlgorithm(SchedulingAlgorithm algorithm) {
    if (scheduler) {
        scheduler->setAlgorithm(algorithm);
    }
}

std::shared_ptr<Request> Building::createExternalRequest(int sourceFloor, Direction direction) {
    if (!isValidFloor(sourceFloor)) {
        return nullptr;
    }
    
    // For external requests, destination is determined by the scheduler
    auto request = std::make_shared<Request>(sourceFloor, sourceFloor, direction, RequestType::EXTERNAL);
    
    // Set the floor button
    auto floor = getFloor(sourceFloor);
    if (floor) {
        if (direction == Direction::UP) {
            floor->setUpButton(ButtonState::PRESSED);
        } else if (direction == Direction::DOWN) {
            floor->setDownButton(ButtonState::PRESSED);
        }
    }
    
    return request;
}

std::shared_ptr<Request> Building::createInternalRequest(int sourceFloor, int destinationFloor) {
    if (!isValidFloor(sourceFloor) || !isValidFloor(destinationFloor)) {
        return nullptr;
    }
    
    Direction direction = (destinationFloor > sourceFloor) ? Direction::UP : Direction::DOWN;
    auto request = std::make_shared<Request>(sourceFloor, destinationFloor, direction, RequestType::INTERNAL);
    
    return request;
}

void Building::submitRequest(const std::shared_ptr<Request>& request) {
    if (!request) return;
    
    allRequests.push_back(request);
    
    if (scheduler) {
        scheduler->addRequest(request);
    }
}

std::vector<std::shared_ptr<Request>> Building::getPendingRequests() const {
    std::vector<std::shared_ptr<Request>> pending;
    for (const auto& request : allRequests) {
        if (request->isPending()) {
            pending.push_back(request);
        }
    }
    return pending;
}

std::vector<std::shared_ptr<Request>> Building::getCompletedRequests() const {
    std::vector<std::shared_ptr<Request>> completed;
    for (const auto& request : allRequests) {
        if (request->isCompleted()) {
            completed.push_back(request);
        }
    }
    return completed;
}

void Building::startSimulation() {
    isSimulationRunning = true;
    simulationStartTime = std::chrono::system_clock::now();
}

void Building::stopSimulation() {
    isSimulationRunning = false;
}

void Building::stepSimulation() {
    if (!isSimulationRunning) return;
    
    // Process all elevators
    for (auto& elevator : elevators) {
        if (!elevator->getIsOperational()) continue;
        
        // Get next request for this elevator
        auto request = elevator->getNextRequest();
        if (request) {
            // Process the request
            if (elevator->getCurrentFloor() == request->getSourceFloor()) {
                // Elevator has reached the source floor
                elevator->openDoor();
                
                // Simulate passenger boarding
                if (elevator->getDoorState() == DoorState::OPEN) {
                    elevator->closeDoor();
                    request->setStatus(RequestStatus::ASSIGNED);
                }
            } else {
                // Move elevator towards the request
                if (elevator->getCurrentFloor() < request->getSourceFloor()) {
                    elevator->setDirection(Direction::UP);
                    elevator->setState(ElevatorState::MOVING_UP);
                } else {
                    elevator->setDirection(Direction::DOWN);
                    elevator->setState(ElevatorState::MOVING_DOWN);
                }
                elevator->move();
            }
        } else {
            // No requests, return to idle
            if (elevator->isMoving()) {
                elevator->stop();
            }
        }
    }
    
    // Schedule new requests
    if (scheduler) {
        scheduler->scheduleRequests();
    }
}

void Building::runSimulation(int steps) {
    startSimulation();
    for (int i = 0; i < steps; ++i) {
        stepSimulation();
    }
    stopSimulation();
}

double Building::getAverageWaitTime() const {
    auto completed = getCompletedRequests();
    if (completed.empty()) return 0.0;
    
    double totalWaitTime = 0.0;
    for (const auto& request : completed) {
        totalWaitTime += request->getWaitTime();
    }
    
    return totalWaitTime / completed.size();
}

double Building::getAverageTravelTime() const {
    auto completed = getCompletedRequests();
    if (completed.empty()) return 0.0;
    
    double totalTravelTime = 0.0;
    for (const auto& request : completed) {
        totalTravelTime += request->getTotalTime();
    }
    
    return totalTravelTime / completed.size();
}

int Building::getCompletedRequestsCount() const {
    return getCompletedRequests().size();
}

double Building::getSystemUtilization() const {
    if (elevators.empty()) return 0.0;
    
    double totalUtilization = 0.0;
    for (const auto& elevator : elevators) {
        totalUtilization += elevator->getUtilizationRate();
    }
    
    return totalUtilization / elevators.size();
}

std::vector<std::shared_ptr<Elevator>> Building::getMostUtilizedElevators(int count) const {
    std::vector<std::shared_ptr<Elevator>> sortedElevators = elevators;
    
    std::sort(sortedElevators.begin(), sortedElevators.end(),
        [](const std::shared_ptr<Elevator>& a, const std::shared_ptr<Elevator>& b) {
            return a->getUtilizationRate() > b->getUtilizationRate();
        });
    
    if (count > sortedElevators.size()) {
        count = sortedElevators.size();
    }
    
    return std::vector<std::shared_ptr<Elevator>>(sortedElevators.begin(), 
                                                  sortedElevators.begin() + count);
}

std::vector<int> Building::getMostRequestedFloors(int count) const {
    std::unordered_map<int, int> floorRequestCounts;
    
    for (const auto& request : allRequests) {
        floorRequestCounts[request->getSourceFloor()]++;
        if (request->getType() == RequestType::INTERNAL) {
            floorRequestCounts[request->getDestinationFloor()]++;
        }
    }
    
    std::vector<std::pair<int, int>> sortedFloors;
    for (const auto& pair : floorRequestCounts) {
        sortedFloors.push_back(pair);
    }
    
    std::sort(sortedFloors.begin(), sortedFloors.end(),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return a.second > b.second;
        });
    
    std::vector<int> result;
    for (int i = 0; i < count && i < sortedFloors.size(); ++i) {
        result.push_back(sortedFloors[i].first);
    }
    
    return result;
}

std::unordered_map<std::string, int> Building::getElevatorTripCounts() const {
    std::unordered_map<std::string, int> tripCounts;
    
    for (const auto& elevator : elevators) {
        tripCounts[elevator->getElevatorId()] = elevator->getTotalTrips();
    }
    
    return tripCounts;
}

void Building::emergencyStopAll() {
    for (auto& elevator : elevators) {
        elevator->emergencyStop();
    }
    stopSimulation();
}

void Building::setElevatorMaintenance(const std::string& elevatorId, bool maintenance) {
    auto elevator = getElevator(elevatorId);
    if (elevator) {
        elevator->setIsOperational(!maintenance);
        if (maintenance) {
            elevator->setState(ElevatorState::MAINTENANCE);
        } else {
            elevator->setState(ElevatorState::IDLE);
        }
    }
}

bool Building::isElevatorInMaintenance(const std::string& elevatorId) const {
    auto elevator = getElevator(elevatorId);
    if (elevator) {
        return !elevator->getIsOperational() && elevator->getState() == ElevatorState::MAINTENANCE;
    }
    return false;
}

void Building::resetSystem() {
    // Reset all elevators
    for (auto& elevator : elevators) {
        elevator->setIsOperational(true);
        elevator->setState(ElevatorState::IDLE);
        elevator->setCurrentLoad(0);
    }
    
    // Clear all floor buttons
    for (auto& floor : floors) {
        floor->clearAllRequests();
    }
    
    // Clear all requests
    allRequests.clear();
    if (scheduler) {
        scheduler->clearPendingRequests();
    }
    
    stopSimulation();
}

bool Building::isValidFloor(int floorNumber) const {
    return floorNumber >= -basementFloors && floorNumber <= totalFloors && floorNumber != 0;
}

int Building::getFloorIndex(int floorNumber) const {
    if (!isValidFloor(floorNumber)) return -1;
    
    int index = 0;
    for (int i = -basementFloors; i <= totalFloors; ++i) {
        if (i == 0) continue; // Skip floor 0
        if (i == floorNumber) return index;
        index++;
    }
    return -1;
}

std::string Building::getFloorDisplayName(int floorNumber) const {
    if (floorNumber == 0) return "G";
    if (floorNumber < 0) return "B" + std::to_string(-floorNumber);
    return std::to_string(floorNumber);
}

void Building::printStatus() const {
    std::cout << "\n=== Building Status: " << buildingName << " ===" << std::endl;
    std::cout << "Building ID: " << buildingId << std::endl;
    std::cout << "Total Floors: " << totalFloors << " (Basements: " << basementFloors << ")" << std::endl;
    std::cout << "Elevators: " << elevators.size() << std::endl;
    std::cout << "Simulation Running: " << (isSimulationRunning ? "Yes" : "No") << std::endl;
    
    std::cout << "\n--- Elevator Status ---" << std::endl;
    for (const auto& elevator : elevators) {
        std::cout << "Elevator " << elevator->getElevatorId() 
                  << ": Floor " << elevator->getCurrentFloor()
                  << " | State: " << elevator->getStateString()
                  << " | Direction: " << elevator->getDirectionString()
                  << " | Load: " << elevator->getCurrentLoad() << "/" << elevator->getCapacity()
                  << " | Operational: " << (elevator->getIsOperational() ? "Yes" : "No") << std::endl;
    }
    
    std::cout << "\n--- Floor Requests ---" << std::endl;
    for (const auto& floor : floors) {
        if (floor->hasAnyRequest()) {
            std::cout << "Floor " << getFloorDisplayName(floor->getFloorNumber())
                      << ": " << floor->getButtonStateString() << std::endl;
        }
    }
}

void Building::printStatistics() const {
    std::cout << "\n=== Building Statistics: " << buildingName << " ===" << std::endl;
    std::cout << "Total Requests: " << getTotalRequests() << std::endl;
    std::cout << "Completed Requests: " << getCompletedRequestsCount() << std::endl;
    std::cout << "Average Wait Time: " << std::fixed << std::setprecision(2) 
              << getAverageWaitTime() << " seconds" << std::endl;
    std::cout << "Average Travel Time: " << std::fixed << std::setprecision(2) 
              << getAverageTravelTime() << " seconds" << std::endl;
    std::cout << "System Utilization: " << std::fixed << std::setprecision(2) 
              << getSystemUtilization() << "%" << std::endl;
    
    std::cout << "\n--- Elevator Utilization ---" << std::endl;
    for (const auto& elevator : elevators) {
        std::cout << "Elevator " << elevator->getElevatorId() 
                  << ": " << std::fixed << std::setprecision(2) 
                  << elevator->getUtilizationRate() << "%"
                  << " | Trips: " << elevator->getTotalTrips() << std::endl;
    }
    
    auto mostUtilized = getMostUtilizedElevators(3);
    if (!mostUtilized.empty()) {
        std::cout << "\n--- Most Utilized Elevators ---" << std::endl;
        for (const auto& elevator : mostUtilized) {
            std::cout << "Elevator " << elevator->getElevatorId() 
                      << ": " << std::fixed << std::setprecision(2) 
                      << elevator->getUtilizationRate() << "%" << std::endl;
        }
    }
    
    auto mostRequested = getMostRequestedFloors(5);
    if (!mostRequested.empty()) {
        std::cout << "\n--- Most Requested Floors ---" << std::endl;
        for (int floor : mostRequested) {
            std::cout << "Floor " << getFloorDisplayName(floor) << std::endl;
        }
    }
} 