#include "../include/Request.hpp"
#include <sstream>
#include <iomanip>
#include <chrono>

Request::Request(int sourceFloor, int destinationFloor, Direction direction, RequestType type)
    : sourceFloor(sourceFloor), destinationFloor(destinationFloor), direction(direction),
      type(type), status(RequestStatus::PENDING), requestTime(std::chrono::system_clock::now()),
      priority(1) {
    
    // Generate unique request ID
    static int counter = 0;
    std::ostringstream oss;
    oss << "REQ" << std::setfill('0') << std::setw(6) << ++counter;
    requestId = oss.str();
    
    // Set priority based on type
    if (type == RequestType::EXTERNAL) {
        priority = 2; // External requests have higher priority
    }
}

int Request::getWaitTime() const {
    if (status == RequestStatus::PENDING) {
        auto now = std::chrono::system_clock::now();
        auto duration = now - requestTime;
        return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    }
    return 0;
}

int Request::getTotalTime() const {
    if (status == RequestStatus::COMPLETED) {
        auto duration = completionTime - requestTime;
        return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    }
    return getWaitTime();
}

std::string Request::getTypeString() const {
    switch (type) {
        case RequestType::EXTERNAL: return "External";
        case RequestType::INTERNAL: return "Internal";
        default: return "Unknown";
    }
}

std::string Request::getStatusString() const {
    switch (status) {
        case RequestStatus::PENDING: return "Pending";
        case RequestStatus::ASSIGNED: return "Assigned";
        case RequestStatus::COMPLETED: return "Completed";
        case RequestStatus::CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

std::string Request::getDirectionString() const {
    switch (direction) {
        case Direction::UP: return "UP";
        case Direction::DOWN: return "DOWN";
        case Direction::NONE: return "NONE";
        default: return "UNKNOWN";
    }
} 