#include "../include/Seat.hpp"
#include <sstream>

Seat::Seat(const std::string& id, int row, int column, SeatType type)
    : id(id), row(row), column(column), status(SeatStatus::AVAILABLE), 
      type(type), price(0.0), bookingId("") {}

std::string Seat::getId() const { return id; }
int Seat::getRow() const { return row; }
int Seat::getColumn() const { return column; }
SeatStatus Seat::getStatus() const { return status; }
SeatType Seat::getType() const { return type; }
double Seat::getPrice() const { return price; }
std::string Seat::getBookingId() const { return bookingId; }

void Seat::setStatus(SeatStatus status) { this->status = status; }
void Seat::setType(SeatType type) { this->type = type; }
void Seat::setPrice(double price) { this->price = price; }
void Seat::setBookingId(const std::string& bookingId) { this->bookingId = bookingId; }

std::string Seat::getSeatNumber() const {
    std::ostringstream oss;
    oss << static_cast<char>('A' + row) << (column + 1);
    return oss.str();
}

std::string Seat::getStatusString() const {
    switch (status) {
        case SeatStatus::AVAILABLE: return "Available";
        case SeatStatus::OCCUPIED: return "Occupied";
        case SeatStatus::RESERVED: return "Reserved";
        case SeatStatus::MAINTENANCE: return "Maintenance";
        default: return "Unknown";
    }
}

std::string Seat::getTypeString() const {
    switch (type) {
        case SeatType::REGULAR: return "Regular";
        case SeatType::PREMIUM: return "Premium";
        case SeatType::VIP: return "VIP";
        case SeatType::WHEELCHAIR_ACCESSIBLE: return "Wheelchair Accessible";
        default: return "Unknown";
    }
}

bool Seat::isAvailable() const { return status == SeatStatus::AVAILABLE; }
bool Seat::isOccupied() const { return status == SeatStatus::OCCUPIED; }
bool Seat::isReserved() const { return status == SeatStatus::RESERVED; }

void Seat::reserve(const std::string& bookingId) {
    if (status == SeatStatus::AVAILABLE) {
        status = SeatStatus::RESERVED;
        this->bookingId = bookingId;
    }
}

void Seat::occupy() {
    if (status == SeatStatus::RESERVED) {
        status = SeatStatus::OCCUPIED;
    }
}

void Seat::release() {
    status = SeatStatus::AVAILABLE;
    bookingId = "";
}

void Seat::setMaintenance() {
    status = SeatStatus::MAINTENANCE;
    bookingId = "";
} 