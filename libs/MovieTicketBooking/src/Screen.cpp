#include "../include/Screen.hpp"
#include <sstream>

Screen::Screen(const std::string& id, const std::string& name, int totalRows, int totalColumns)
    : id(id), name(name), totalRows(totalRows), totalColumns(totalColumns), isActive(true) {
    initializeSeats();
}

std::string Screen::getId() const { return id; }
std::string Screen::getName() const { return name; }
int Screen::getTotalRows() const { return totalRows; }
int Screen::getTotalColumns() const { return totalColumns; }
int Screen::getTotalSeats() const { return totalRows * totalColumns; }
bool Screen::getIsActive() const { return isActive; }

void Screen::setName(const std::string& name) { this->name = name; }
void Screen::setIsActive(bool isActive) { this->isActive = isActive; }

Seat* Screen::getSeat(const std::string& seatId) {
    auto it = seatMap.find(seatId);
    return (it != seatMap.end()) ? it->second : nullptr;
}

Seat* Screen::getSeat(int row, int column) {
    if (row >= 0 && row < totalRows && column >= 0 && column < totalColumns) {
        return &seats[row][column];
    }
    return nullptr;
}

std::vector<std::vector<Seat>> Screen::getAllSeats() const { return seats; }

bool Screen::reserveSeat(const std::string& seatId, const std::string& bookingId) {
    Seat* seat = getSeat(seatId);
    if (seat && seat->isAvailable()) {
        seat->reserve(bookingId);
        return true;
    }
    return false;
}

bool Screen::reserveSeat(int row, int column, const std::string& bookingId) {
    Seat* seat = getSeat(row, column);
    if (seat && seat->isAvailable()) {
        seat->reserve(bookingId);
        return true;
    }
    return false;
}

bool Screen::occupySeat(const std::string& seatId) {
    Seat* seat = getSeat(seatId);
    if (seat && seat->isReserved()) {
        seat->occupy();
        return true;
    }
    return false;
}

bool Screen::occupySeat(int row, int column) {
    Seat* seat = getSeat(row, column);
    if (seat && seat->isReserved()) {
        seat->occupy();
        return true;
    }
    return false;
}

bool Screen::releaseSeat(const std::string& seatId) {
    Seat* seat = getSeat(seatId);
    if (seat) {
        seat->release();
        return true;
    }
    return false;
}

bool Screen::releaseSeat(int row, int column) {
    Seat* seat = getSeat(row, column);
    if (seat) {
        seat->release();
        return true;
    }
    return false;
}

void Screen::setSeatMaintenance(const std::string& seatId) {
    Seat* seat = getSeat(seatId);
    if (seat) {
        seat->setMaintenance();
    }
}

void Screen::setSeatMaintenance(int row, int column) {
    Seat* seat = getSeat(row, column);
    if (seat) {
        seat->setMaintenance();
    }
}

std::vector<Seat*> Screen::getAvailableSeats() const {
    std::vector<Seat*> availableSeats;
    for (const auto& row : seats) {
        for (const auto& seat : row) {
            if (seat.isAvailable()) {
                availableSeats.push_back(const_cast<Seat*>(&seat));
            }
        }
    }
    return availableSeats;
}

std::vector<Seat*> Screen::getReservedSeats() const {
    std::vector<Seat*> reservedSeats;
    for (const auto& row : seats) {
        for (const auto& seat : row) {
            if (seat.isReserved()) {
                reservedSeats.push_back(const_cast<Seat*>(&seat));
            }
        }
    }
    return reservedSeats;
}

std::vector<Seat*> Screen::getOccupiedSeats() const {
    std::vector<Seat*> occupiedSeats;
    for (const auto& row : seats) {
        for (const auto& seat : row) {
            if (seat.isOccupied()) {
                occupiedSeats.push_back(const_cast<Seat*>(&seat));
            }
        }
    }
    return occupiedSeats;
}

int Screen::getAvailableSeatCount() const {
    int count = 0;
    for (const auto& row : seats) {
        for (const auto& seat : row) {
            if (seat.isAvailable()) count++;
        }
    }
    return count;
}

int Screen::getReservedSeatCount() const {
    int count = 0;
    for (const auto& row : seats) {
        for (const auto& seat : row) {
            if (seat.isReserved()) count++;
        }
    }
    return count;
}

int Screen::getOccupiedSeatCount() const {
    int count = 0;
    for (const auto& row : seats) {
        for (const auto& seat : row) {
            if (seat.isOccupied()) count++;
        }
    }
    return count;
}

bool Screen::isSeatAvailable(const std::string& seatId) const {
    auto it = seatMap.find(seatId);
    return (it != seatMap.end()) ? it->second->isAvailable() : false;
}

bool Screen::isSeatAvailable(int row, int column) const {
    if (row >= 0 && row < totalRows && column >= 0 && column < totalColumns) {
        return seats[row][column].isAvailable();
    }
    return false;
}

std::string Screen::getSeatLayoutString() const {
    std::ostringstream oss;
    oss << "Screen: " << name << " (" << totalRows << "x" << totalColumns << ")\n";
    oss << "   ";
    for (int col = 0; col < totalColumns; ++col) {
        oss << " " << (col + 1) << " ";
    }
    oss << "\n";
    
    for (int row = 0; row < totalRows; ++row) {
        oss << static_cast<char>('A' + row) << " ";
        for (int col = 0; col < totalColumns; ++col) {
            const Seat& seat = seats[row][col];
            switch (seat.getStatus()) {
                case SeatStatus::AVAILABLE: oss << " [ ]"; break;
                case SeatStatus::RESERVED: oss << " [R]"; break;
                case SeatStatus::OCCUPIED: oss << " [X]"; break;
                case SeatStatus::MAINTENANCE: oss << " [M]"; break;
            }
        }
        oss << "\n";
    }
    return oss.str();
}

void Screen::initializeSeats() {
    seats.resize(totalRows);
    for (int row = 0; row < totalRows; ++row) {
        seats[row].resize(totalColumns);
        for (int col = 0; col < totalColumns; ++col) {
            std::string seatId = id + "_" + std::to_string(row) + "_" + std::to_string(col);
            seats[row][col] = Seat(seatId, row, col);
            seatMap[seatId] = &seats[row][col];
        }
    }
}

void Screen::setSeatType(int row, int column, SeatType type) {
    Seat* seat = getSeat(row, column);
    if (seat) {
        seat->setType(type);
    }
}

void Screen::setSeatPrice(int row, int column, double price) {
    Seat* seat = getSeat(row, column);
    if (seat) {
        seat->setPrice(price);
    }
} 