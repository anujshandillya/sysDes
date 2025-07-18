#include "../include/Theater.hpp"
#include <algorithm>
#include <sstream>

Theater::Theater(const std::string& id, const std::string& name, const std::string& location,
                 const std::string& address, const std::string& phone)
    : id(id), name(name), location(location), address(address), phone(phone), isActive(true) {}

std::string Theater::getId() const { return id; }
std::string Theater::getName() const { return name; }
std::string Theater::getLocation() const { return location; }
std::string Theater::getAddress() const { return address; }
std::string Theater::getPhone() const { return phone; }
bool Theater::getIsActive() const { return isActive; }
std::vector<Screen> Theater::getScreens() const { return screens; }
int Theater::getTotalScreens() const { return screens.size(); }

void Theater::setName(const std::string& name) { this->name = name; }
void Theater::setLocation(const std::string& location) { this->location = location; }
void Theater::setAddress(const std::string& address) { this->address = address; }
void Theater::setPhone(const std::string& phone) { this->phone = phone; }
void Theater::setIsActive(bool isActive) { this->isActive = isActive; }

Screen* Theater::getScreen(const std::string& screenId) {
    for (auto& screen : screens) {
        if (screen.getId() == screenId) {
            return &screen;
        }
    }
    return nullptr;
}

void Theater::addScreen(const Screen& screen) {
    screens.push_back(screen);
}

bool Theater::removeScreen(const std::string& screenId) {
    auto it = std::find_if(screens.begin(), screens.end(),
                          [&screenId](const Screen& screen) {
                              return screen.getId() == screenId;
                          });
    if (it != screens.end()) {
        screens.erase(it);
        return true;
    }
    return false;
}

bool Theater::hasScreen(const std::string& screenId) const {
    return std::any_of(screens.begin(), screens.end(),
                      [&screenId](const Screen& screen) {
                          return screen.getId() == screenId;
                      });
}

int Theater::getTotalSeats() const {
    int total = 0;
    for (const auto& screen : screens) {
        total += screen.getTotalSeats();
    }
    return total;
}

int Theater::getAvailableSeats() const {
    int total = 0;
    for (const auto& screen : screens) {
        total += screen.getAvailableSeatCount();
    }
    return total;
}

std::vector<Screen*> Theater::getActiveScreens() const {
    std::vector<Screen*> activeScreens;
    for (const auto& screen : screens) {
        if (screen.getIsActive()) {
            activeScreens.push_back(const_cast<Screen*>(&screen));
        }
    }
    return activeScreens;
}

std::string Theater::getTheaterInfo() const {
    std::ostringstream oss;
    oss << "Theater: " << name << "\n";
    oss << "Location: " << location << "\n";
    oss << "Address: " << address << "\n";
    oss << "Phone: " << phone << "\n";
    oss << "Screens: " << getTotalScreens() << "\n";
    oss << "Total Seats: " << getTotalSeats() << "\n";
    oss << "Available Seats: " << getAvailableSeats() << "\n";
    oss << "Status: " << (isActive ? "Active" : "Inactive");
    return oss.str();
} 