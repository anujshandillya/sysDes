#pragma once

#include <vector>
#include <memory>
#include "Elevator.hpp"
#include "Request.hpp"

enum class SchedulingAlgorithm {
    FCFS,      // First Come First Serve
    SCAN,      // Elevator algorithm
    LOOK,      // Improved SCAN
    SSTF,      // Shortest Seek Time First
    PRIORITY   // Priority-based scheduling
};

class Scheduler {
private:
    SchedulingAlgorithm algorithm;
    std::vector<std::shared_ptr<Elevator>> elevators;
    std::vector<std::shared_ptr<Request>> pendingRequests;

public:
    Scheduler(SchedulingAlgorithm algo = SchedulingAlgorithm::LOOK);

    // Algorithm selection
    void setAlgorithm(SchedulingAlgorithm algo) { algorithm = algo; }
    SchedulingAlgorithm getAlgorithm() const { return algorithm; }

    // Elevator management
    void addElevator(const std::shared_ptr<Elevator>& elevator);
    void removeElevator(const std::string& elevatorId);
    std::vector<std::shared_ptr<Elevator>> getElevators() const { return elevators; }
    std::shared_ptr<Elevator> getElevator(const std::string& elevatorId) const;

    // Request management
    void addRequest(const std::shared_ptr<Request>& request);
    void removeRequest(const std::shared_ptr<Request>& request);
    std::vector<std::shared_ptr<Request>> getPendingRequests() const { return pendingRequests; }
    void clearPendingRequests() { pendingRequests.clear(); }

    // Scheduling algorithms
    std::shared_ptr<Elevator> assignRequest(const std::shared_ptr<Request>& request);
    void scheduleRequests();
    void optimizeSchedules();

    // Algorithm implementations
    std::shared_ptr<Elevator> fcfsAssign(const std::shared_ptr<Request>& request);
    std::shared_ptr<Elevator> scanAssign(const std::shared_ptr<Request>& request);
    std::shared_ptr<Elevator> lookAssign(const std::shared_ptr<Request>& request);
    std::shared_ptr<Elevator> sstfAssign(const std::shared_ptr<Request>& request);
    std::shared_ptr<Elevator> priorityAssign(const std::shared_ptr<Request>& request);

    // Utility methods
    std::shared_ptr<Elevator> findNearestIdleElevator(int floor) const;
    std::shared_ptr<Elevator> findBestElevator(const std::shared_ptr<Request>& request) const;
    int calculateCost(const std::shared_ptr<Elevator>& elevator, const std::shared_ptr<Request>& request) const;
    bool canServeRequest(const std::shared_ptr<Elevator>& elevator, const std::shared_ptr<Request>& request) const;
    std::string getAlgorithmString() const;

private:
    // Helper methods for algorithms
    std::vector<std::shared_ptr<Elevator>> getAvailableElevators() const;
    std::vector<std::shared_ptr<Elevator>> getOperationalElevators() const;
    int getTotalDistance(const std::shared_ptr<Elevator>& elevator, const std::shared_ptr<Request>& request) const;
    bool isDirectionCompatible(const std::shared_ptr<Elevator>& elevator, const std::shared_ptr<Request>& request) const;
}; 