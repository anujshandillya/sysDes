#pragma once

#include <string>
#include <chrono>
#include "Floor.hpp"

enum class RequestType {
    EXTERNAL,  // Request from floor button
    INTERNAL   // Request from elevator button
};

enum class RequestStatus {
    PENDING,
    ASSIGNED,
    COMPLETED,
    CANCELLED
};

class Request {
private:
    std::string requestId;
    int sourceFloor;
    int destinationFloor;
    Direction direction;
    RequestType type;
    RequestStatus status;
    std::chrono::system_clock::time_point requestTime;
    std::chrono::system_clock::time_point completionTime;
    std::string assignedElevatorId;
    int priority; // Higher number = higher priority

public:
    Request(int sourceFloor, int destinationFloor, Direction direction, RequestType type);

    // Getters
    std::string getRequestId() const { return requestId; }
    int getSourceFloor() const { return sourceFloor; }
    int getDestinationFloor() const { return destinationFloor; }
    Direction getDirection() const { return direction; }
    RequestType getType() const { return type; }
    RequestStatus getStatus() const { return status; }
    std::chrono::system_clock::time_point getRequestTime() const { return requestTime; }
    std::chrono::system_clock::time_point getCompletionTime() const { return completionTime; }
    std::string getAssignedElevatorId() const { return assignedElevatorId; }
    int getPriority() const { return priority; }

    // Setters
    void setStatus(RequestStatus newStatus) { status = newStatus; }
    void setCompletionTime(const std::chrono::system_clock::time_point& time) { completionTime = time; }
    void setAssignedElevatorId(const std::string& elevatorId) { assignedElevatorId = elevatorId; }
    void setPriority(int newPriority) { priority = newPriority; }

    // Utility methods
    bool isExternal() const { return type == RequestType::EXTERNAL; }
    bool isInternal() const { return type == RequestType::INTERNAL; }
    bool isPending() const { return status == RequestStatus::PENDING; }
    bool isAssigned() const { return status == RequestStatus::ASSIGNED; }
    bool isCompleted() const { return status == RequestStatus::COMPLETED; }
    int getWaitTime() const; // Returns wait time in seconds
    int getTotalTime() const; // Returns total processing time in seconds
    std::string getTypeString() const;
    std::string getStatusString() const;
    std::string getDirectionString() const;
}; 