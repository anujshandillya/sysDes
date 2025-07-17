#pragma once

#include <string>

enum class Direction {
    UP,
    DOWN,
    NONE
};

enum class ButtonState {
    PRESSED,
    NOT_PRESSED
};

class Floor {
private:
    int floorNumber;
    ButtonState upButton;
    ButtonState downButton;
    bool isDestination; // Whether this floor is a destination for any elevator
    Direction requestedDirection;

public:
    Floor(int floorNumber);

    // Getters
    int getFloorNumber() const { return floorNumber; }
    ButtonState getUpButton() const { return upButton; }
    ButtonState getDownButton() const { return downButton; }
    bool getIsDestination() const { return isDestination; }
    Direction getRequestedDirection() const { return requestedDirection; }

    // Setters
    void setUpButton(ButtonState state) { upButton = state; }
    void setDownButton(ButtonState state) { downButton = state; }
    void setIsDestination(bool destination) { isDestination = destination; }
    void setRequestedDirection(Direction direction) { requestedDirection = direction; }

    // Utility methods
    bool hasUpRequest() const { return upButton == ButtonState::PRESSED; }
    bool hasDownRequest() const { return downButton == ButtonState::PRESSED; }
    bool hasAnyRequest() const { return hasUpRequest() || hasDownRequest(); }
    void clearUpRequest() { upButton = ButtonState::NOT_PRESSED; }
    void clearDownRequest() { downButton = ButtonState::NOT_PRESSED; }
    void clearAllRequests();
    std::string getDirectionString() const;
    std::string getButtonStateString() const;
}; 