#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "Seat.hpp"
#include <vector>
#include <map>
#include <string>

class Screen {
private:
    std::string id;
    std::string name;
    int totalRows;
    int totalColumns;
    std::vector<std::vector<Seat>> seats;
    std::map<std::string, Seat*> seatMap; // id -> seat pointer for quick lookup
    bool isActive;

public:
    Screen(const std::string& id, const std::string& name, int totalRows, int totalColumns);
    
    // Getters
    std::string getId() const;
    std::string getName() const;
    int getTotalRows() const;
    int getTotalColumns() const;
    int getTotalSeats() const;
    bool getIsActive() const;
    Seat* getSeat(const std::string& seatId);
    Seat* getSeat(int row, int column);
    std::vector<std::vector<Seat>> getAllSeats() const;
    
    // Setters
    void setName(const std::string& name);
    void setIsActive(bool isActive);
    
    // Seat management
    bool reserveSeat(const std::string& seatId, const std::string& bookingId);
    bool reserveSeat(int row, int column, const std::string& bookingId);
    bool occupySeat(const std::string& seatId);
    bool occupySeat(int row, int column);
    bool releaseSeat(const std::string& seatId);
    bool releaseSeat(int row, int column);
    void setSeatMaintenance(const std::string& seatId);
    void setSeatMaintenance(int row, int column);
    
    // Utility methods
    std::vector<Seat*> getAvailableSeats() const;
    std::vector<Seat*> getReservedSeats() const;
    std::vector<Seat*> getOccupiedSeats() const;
    int getAvailableSeatCount() const;
    int getReservedSeatCount() const;
    int getOccupiedSeatCount() const;
    bool isSeatAvailable(const std::string& seatId) const;
    bool isSeatAvailable(int row, int column) const;
    std::string getSeatLayoutString() const;
    void initializeSeats();
    void setSeatType(int row, int column, SeatType type);
    void setSeatPrice(int row, int column, double price);
};

#endif // SCREEN_HPP 