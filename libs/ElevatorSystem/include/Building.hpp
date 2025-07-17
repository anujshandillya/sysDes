#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include "Floor.hpp"
#include "Elevator.hpp"
#include "Request.hpp"
#include "Scheduler.hpp"

class Building {
private:
    std::string buildingId;
    std::string buildingName;
    int totalFloors;
    int basementFloors;
    std::vector<std::shared_ptr<Floor>> floors;
    std::vector<std::shared_ptr<Elevator>> elevators;
    std::shared_ptr<Scheduler> scheduler;
    std::vector<std::shared_ptr<Request>> allRequests;
    std::chrono::system_clock::time_point simulationStartTime;
    bool isSimulationRunning;

public:
    Building(const std::string& buildingId, const std::string& buildingName, 
             int totalFloors, int basementFloors = 0);

    // Building configuration
    void setTotalFloors(int floors);
    void setBasementFloors(int basements);
    int getTotalFloors() const { return totalFloors; }
    int getBasementFloors() const { return basementFloors; }
    std::string getBuildingId() const { return buildingId; }
    std::string getBuildingName() const { return buildingName; }

    // Floor management
    void initializeFloors();
    std::shared_ptr<Floor> getFloor(int floorNumber) const;
    std::vector<std::shared_ptr<Floor>> getAllFloors() const { return floors; }
    std::vector<std::shared_ptr<Floor>> getFloorsWithRequests() const;

    // Elevator management
    void addElevator(const std::shared_ptr<Elevator>& elevator);
    void removeElevator(const std::string& elevatorId);
    std::shared_ptr<Elevator> getElevator(const std::string& elevatorId) const;
    std::vector<std::shared_ptr<Elevator>> getAllElevators() const { return elevators; }
    std::vector<std::shared_ptr<Elevator>> getOperationalElevators() const;
    int getElevatorCount() const { return elevators.size(); }

    // Scheduler management
    void setScheduler(const std::shared_ptr<Scheduler>& newScheduler);
    std::shared_ptr<Scheduler> getScheduler() const { return scheduler; }
    void setSchedulingAlgorithm(SchedulingAlgorithm algorithm);

    // Request management
    std::shared_ptr<Request> createExternalRequest(int sourceFloor, Direction direction);
    std::shared_ptr<Request> createInternalRequest(int sourceFloor, int destinationFloor);
    void submitRequest(const std::shared_ptr<Request>& request);
    std::vector<std::shared_ptr<Request>> getAllRequests() const { return allRequests; }
    std::vector<std::shared_ptr<Request>> getPendingRequests() const;
    std::vector<std::shared_ptr<Request>> getCompletedRequests() const;
    void clearAllRequests() { allRequests.clear(); }

    // Simulation and operation
    void startSimulation();
    void stopSimulation();
    void stepSimulation();
    void runSimulation(int steps);
    bool isSimulationActive() const { return isSimulationRunning; }
    std::chrono::system_clock::time_point getSimulationStartTime() const { return simulationStartTime; }

    // Statistics and reporting
    double getAverageWaitTime() const;
    double getAverageTravelTime() const;
    int getTotalRequests() const { return allRequests.size(); }
    int getCompletedRequestsCount() const;
    double getSystemUtilization() const;
    std::vector<std::shared_ptr<Elevator>> getMostUtilizedElevators(int count = 3) const;
    std::vector<int> getMostRequestedFloors(int count = 5) const;
    std::unordered_map<std::string, int> getElevatorTripCounts() const;

    // Emergency and maintenance
    void emergencyStopAll();
    void setElevatorMaintenance(const std::string& elevatorId, bool maintenance);
    bool isElevatorInMaintenance(const std::string& elevatorId) const;
    void resetSystem();

    // Utility methods
    bool isValidFloor(int floorNumber) const;
    int getFloorIndex(int floorNumber) const;
    std::string getFloorDisplayName(int floorNumber) const;
    void printStatus() const;
    void printStatistics() const;
}; 