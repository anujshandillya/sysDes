#include "../include/Scheduler.hpp"
#include <algorithm>
#include <limits>

Scheduler::Scheduler(SchedulingAlgorithm algo) : algorithm(algo) {}

void Scheduler::addElevator(const std::shared_ptr<Elevator>& elevator) {
    elevators.push_back(elevator);
}

void Scheduler::removeElevator(const std::string& elevatorId) {
    elevators.erase(
        std::remove_if(elevators.begin(), elevators.end(),
            [&elevatorId](const std::shared_ptr<Elevator>& elevator) {
                return elevator->getElevatorId() == elevatorId;
            }),
        elevators.end()
    );
}

std::shared_ptr<Elevator> Scheduler::getElevator(const std::string& elevatorId) const {
    for (const auto& elevator : elevators) {
        if (elevator->getElevatorId() == elevatorId) {
            return elevator;
        }
    }
    return nullptr;
}

void Scheduler::addRequest(const std::shared_ptr<Request>& request) {
    pendingRequests.push_back(request);
}

void Scheduler::removeRequest(const std::shared_ptr<Request>& request) {
    pendingRequests.erase(
        std::remove_if(pendingRequests.begin(), pendingRequests.end(),
            [&request](const std::shared_ptr<Request>& req) {
                return req->getRequestId() == request->getRequestId();
            }),
        pendingRequests.end()
    );
}

std::shared_ptr<Elevator> Scheduler::assignRequest(const std::shared_ptr<Request>& request) {
    switch (algorithm) {
        case SchedulingAlgorithm::FCFS:
            return fcfsAssign(request);
        case SchedulingAlgorithm::SCAN:
            return scanAssign(request);
        case SchedulingAlgorithm::LOOK:
            return lookAssign(request);
        case SchedulingAlgorithm::SSTF:
            return sstfAssign(request);
        case SchedulingAlgorithm::PRIORITY:
            return priorityAssign(request);
        default:
            return lookAssign(request);
    }
}

void Scheduler::scheduleRequests() {
    for (auto& request : pendingRequests) {
        if (request->isPending()) {
            auto elevator = assignRequest(request);
            if (elevator) {
                request->setStatus(RequestStatus::ASSIGNED);
                request->setAssignedElevatorId(elevator->getElevatorId());
                elevator->addRequest(request);
            }
        }
    }
}

void Scheduler::optimizeSchedules() {
    // This is a simplified optimization
    // In a real system, you'd implement more sophisticated optimization algorithms
    for (auto& elevator : elevators) {
        // Sort requests in the elevator's queue based on the current algorithm
        // This is a placeholder for actual optimization logic
    }
}

std::shared_ptr<Elevator> Scheduler::fcfsAssign(const std::shared_ptr<Request>& request) {
    // First Come First Serve - assign to the first available elevator
    for (const auto& elevator : getAvailableElevators()) {
        if (canServeRequest(elevator, request)) {
            return elevator;
        }
    }
    return nullptr;
}

std::shared_ptr<Elevator> Scheduler::scanAssign(const std::shared_ptr<Request>& request) {
    // SCAN algorithm - elevator moves in one direction until the end, then reverses
    std::shared_ptr<Elevator> bestElevator = nullptr;
    int minCost = std::numeric_limits<int>::max();
    
    for (const auto& elevator : getOperationalElevators()) {
        if (!canServeRequest(elevator, request)) continue;
        
        int cost = calculateCost(elevator, request);
        if (cost < minCost) {
            minCost = cost;
            bestElevator = elevator;
        }
    }
    
    return bestElevator;
}

std::shared_ptr<Elevator> Scheduler::lookAssign(const std::shared_ptr<Request>& request) {
    // LOOK algorithm - improved SCAN that doesn't go to the end
    std::shared_ptr<Elevator> bestElevator = nullptr;
    int minCost = std::numeric_limits<int>::max();
    
    for (const auto& elevator : getOperationalElevators()) {
        if (!canServeRequest(elevator, request)) continue;
        
        int cost = calculateCost(elevator, request);
        if (cost < minCost) {
            minCost = cost;
            bestElevator = elevator;
        }
    }
    
    return bestElevator;
}

std::shared_ptr<Elevator> Scheduler::sstfAssign(const std::shared_ptr<Request>& request) {
    // Shortest Seek Time First - assign to nearest elevator
    std::shared_ptr<Elevator> nearestElevator = nullptr;
    int minDistance = std::numeric_limits<int>::max();
    
    for (const auto& elevator : getOperationalElevators()) {
        if (!canServeRequest(elevator, request)) continue;
        
        int distance = elevator->getDistanceToFloor(request->getSourceFloor());
        if (distance < minDistance) {
            minDistance = distance;
            nearestElevator = elevator;
        }
    }
    
    return nearestElevator;
}

std::shared_ptr<Elevator> Scheduler::priorityAssign(const std::shared_ptr<Request>& request) {
    // Priority-based scheduling - consider request priority and elevator load
    std::shared_ptr<Elevator> bestElevator = nullptr;
    double bestScore = std::numeric_limits<double>::lowest();
    
    for (const auto& elevator : getOperationalElevators()) {
        if (!canServeRequest(elevator, request)) continue;
        
        // Calculate priority score based on multiple factors
        double distanceScore = 1.0 / (1.0 + elevator->getDistanceToFloor(request->getSourceFloor()));
        double loadScore = 1.0 - (elevator->getCurrentLoad() / (double)elevator->getCapacity());
        double priorityScore = request->getPriority() / 10.0;
        
        double totalScore = distanceScore * 0.4 + loadScore * 0.3 + priorityScore * 0.3;
        
        if (totalScore > bestScore) {
            bestScore = totalScore;
            bestElevator = elevator;
        }
    }
    
    return bestElevator;
}

std::shared_ptr<Elevator> Scheduler::findNearestIdleElevator(int floor) const {
    std::shared_ptr<Elevator> nearest = nullptr;
    int minDistance = std::numeric_limits<int>::max();
    
    for (const auto& elevator : elevators) {
        if (elevator->isIdle() && elevator->canServeFloor(floor)) {
            int distance = elevator->getDistanceToFloor(floor);
            if (distance < minDistance) {
                minDistance = distance;
                nearest = elevator;
            }
        }
    }
    
    return nearest;
}

std::shared_ptr<Elevator> Scheduler::findBestElevator(const std::shared_ptr<Request>& request) const {
    // Since assignRequest is not const, we need to implement the logic here
    switch (algorithm) {
        case SchedulingAlgorithm::FCFS:
            for (const auto& elevator : getAvailableElevators()) {
                if (canServeRequest(elevator, request)) {
                    return elevator;
                }
            }
            break;
        case SchedulingAlgorithm::SSTF: {
            std::shared_ptr<Elevator> nearestElevator = nullptr;
            int minDistance = std::numeric_limits<int>::max();
            
            for (const auto& elevator : getOperationalElevators()) {
                if (!canServeRequest(elevator, request)) continue;
                
                int distance = elevator->getDistanceToFloor(request->getSourceFloor());
                if (distance < minDistance) {
                    minDistance = distance;
                    nearestElevator = elevator;
                }
            }
            return nearestElevator;
        }
        default:
            // For other algorithms, find the best based on cost
            std::shared_ptr<Elevator> bestElevator = nullptr;
            int minCost = std::numeric_limits<int>::max();
            
            for (const auto& elevator : getOperationalElevators()) {
                if (!canServeRequest(elevator, request)) continue;
                
                int cost = calculateCost(elevator, request);
                if (cost < minCost) {
                    minCost = cost;
                    bestElevator = elevator;
                }
            }
            return bestElevator;
    }
    return nullptr;
}

int Scheduler::calculateCost(const std::shared_ptr<Elevator>& elevator, const std::shared_ptr<Request>& request) const {
    int distance = elevator->getDistanceToFloor(request->getSourceFloor());
    int queueSize = elevator->getRequestCount();
    int load = elevator->getCurrentLoad();
    
    // Weighted cost calculation
    return distance * 2 + queueSize * 5 + load * 3;
}

bool Scheduler::canServeRequest(const std::shared_ptr<Elevator>& elevator, const std::shared_ptr<Request>& request) const {
    if (!elevator->getIsOperational()) return false;
    if (!elevator->canServeFloor(request->getSourceFloor())) return false;
    if (!elevator->canServeFloor(request->getDestinationFloor())) return false;
    if (elevator->getCurrentLoad() >= elevator->getCapacity()) return false;
    
    return true;
}

std::string Scheduler::getAlgorithmString() const {
    switch (algorithm) {
        case SchedulingAlgorithm::FCFS: return "FCFS";
        case SchedulingAlgorithm::SCAN: return "SCAN";
        case SchedulingAlgorithm::LOOK: return "LOOK";
        case SchedulingAlgorithm::SSTF: return "SSTF";
        case SchedulingAlgorithm::PRIORITY: return "PRIORITY";
        default: return "UNKNOWN";
    }
}

std::vector<std::shared_ptr<Elevator>> Scheduler::getAvailableElevators() const {
    std::vector<std::shared_ptr<Elevator>> available;
    for (const auto& elevator : elevators) {
        if (elevator->getIsOperational() && elevator->getCurrentLoad() < elevator->getCapacity()) {
            available.push_back(elevator);
        }
    }
    return available;
}

std::vector<std::shared_ptr<Elevator>> Scheduler::getOperationalElevators() const {
    std::vector<std::shared_ptr<Elevator>> operational;
    for (const auto& elevator : elevators) {
        if (elevator->getIsOperational()) {
            operational.push_back(elevator);
        }
    }
    return operational;
}

int Scheduler::getTotalDistance(const std::shared_ptr<Elevator>& elevator, const std::shared_ptr<Request>& request) const {
    int currentToSource = elevator->getDistanceToFloor(request->getSourceFloor());
    int sourceToDest = std::abs(request->getDestinationFloor() - request->getSourceFloor());
    return currentToSource + sourceToDest;
}

bool Scheduler::isDirectionCompatible(const std::shared_ptr<Elevator>& elevator, const std::shared_ptr<Request>& request) const {
    if (elevator->isIdle()) return true;
    
    Direction elevatorDir = elevator->getDirection();
    Direction requestDir = request->getDirection();
    
    return elevatorDir == requestDir || elevatorDir == Direction::NONE;
} 