#ifndef THEATER_HPP
#define THEATER_HPP

#include "Screen.hpp"
#include <vector>
#include <string>

class Theater {
private:
    std::string id;
    std::string name;
    std::string location;
    std::string address;
    std::string phone;
    std::vector<Screen> screens;
    bool isActive;

public:
    Theater(const std::string& id, const std::string& name, const std::string& location,
            const std::string& address, const std::string& phone);
    
    // Getters
    std::string getId() const;
    std::string getName() const;
    std::string getLocation() const;
    std::string getAddress() const;
    std::string getPhone() const;
    bool getIsActive() const;
    std::vector<Screen> getScreens() const;
    Screen* getScreen(const std::string& screenId);
    int getTotalScreens() const;
    
    // Setters
    void setName(const std::string& name);
    void setLocation(const std::string& location);
    void setAddress(const std::string& address);
    void setPhone(const std::string& phone);
    void setIsActive(bool isActive);
    
    // Screen management
    void addScreen(const Screen& screen);
    bool removeScreen(const std::string& screenId);
    bool hasScreen(const std::string& screenId) const;
    
    // Utility methods
    int getTotalSeats() const;
    int getAvailableSeats() const;
    std::vector<Screen*> getActiveScreens() const;
    std::string getTheaterInfo() const;
};

#endif // THEATER_HPP 