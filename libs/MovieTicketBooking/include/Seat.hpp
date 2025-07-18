#ifndef SEAT_HPP
#define SEAT_HPP

#include <string>

enum class SeatStatus {
    AVAILABLE,
    OCCUPIED,
    RESERVED,
    MAINTENANCE
};

enum class SeatType {
    REGULAR,
    PREMIUM,
    VIP,
    WHEELCHAIR_ACCESSIBLE
};

class Seat {
private:
    std::string id;
    int row;
    int column;
    SeatStatus status;
    SeatType type;
    double price;
    std::string bookingId; // ID of the booking that reserved this seat

public:
    Seat(const std::string& id, int row, int column, SeatType type = SeatType::REGULAR);
    
    // Getters
    std::string getId() const;
    int getRow() const;
    int getColumn() const;
    SeatStatus getStatus() const;
    SeatType getType() const;
    double getPrice() const;
    std::string getBookingId() const;
    
    // Setters
    void setStatus(SeatStatus status);
    void setType(SeatType type);
    void setPrice(double price);
    void setBookingId(const std::string& bookingId);
    
    // Utility methods
    std::string getSeatNumber() const; // Returns "A1", "B5", etc.
    std::string getStatusString() const;
    std::string getTypeString() const;
    bool isAvailable() const;
    bool isOccupied() const;
    bool isReserved() const;
    void reserve(const std::string& bookingId);
    void occupy();
    void release();
    void setMaintenance();
};

#endif // SEAT_HPP 