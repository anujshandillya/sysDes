#include "../include/Floor.hpp"
#include <sstream>

Floor::Floor(int floorNumber) 
    : floorNumber(floorNumber), upButton(ButtonState::NOT_PRESSED), 
      downButton(ButtonState::NOT_PRESSED), isDestination(false), 
      requestedDirection(Direction::NONE) {}

void Floor::clearAllRequests() {
    upButton = ButtonState::NOT_PRESSED;
    downButton = ButtonState::NOT_PRESSED;
    requestedDirection = Direction::NONE;
}

std::string Floor::getDirectionString() const {
    switch (requestedDirection) {
        case Direction::UP: return "UP";
        case Direction::DOWN: return "DOWN";
        case Direction::NONE: return "NONE";
        default: return "UNKNOWN";
    }
}

std::string Floor::getButtonStateString() const {
    std::ostringstream oss;
    oss << "Floor " << floorNumber << ": ";
    oss << "UP=" << (upButton == ButtonState::PRESSED ? "PRESSED" : "NOT_PRESSED") << ", ";
    oss << "DOWN=" << (downButton == ButtonState::PRESSED ? "PRESSED" : "NOT_PRESSED");
    return oss.str();
} 