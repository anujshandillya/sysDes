#include "../include/Booking.hpp"
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <random>

Booking::Booking(const std::string& id, User* user, Show* show, 
                 const std::vector<std::string>& seatIds, double totalAmount)
    : id(id), user(user), show(show), seatIds(seatIds), totalAmount(totalAmount),
      bookingTime(std::chrono::system_clock::now()), status(BookingStatus::PENDING), payment(nullptr) {
    calculateExpiryTime();
    bookingCode = generateBookingCode();
}

std::string Booking::getId() const { return id; }
User* Booking::getUser() const { return user; }
Show* Booking::getShow() const { return show; }
std::vector<std::string> Booking::getSeatIds() const { return seatIds; }
std::chrono::system_clock::time_point Booking::getBookingTime() const { return bookingTime; }
std::chrono::system_clock::time_point Booking::getExpiryTime() const { return expiryTime; }
BookingStatus Booking::getStatus() const { return status; }
double Booking::getTotalAmount() const { return totalAmount; }
Payment* Booking::getPayment() const { return payment; }
std::string Booking::getBookingCode() const { return bookingCode; }

void Booking::setStatus(BookingStatus status) { this->status = status; }
void Booking::setTotalAmount(double amount) { this->totalAmount = amount; }
void Booking::setPayment(Payment* payment) { this->payment = payment; }
void Booking::setExpiryTime(const std::chrono::system_clock::time_point& expiryTime) { this->expiryTime = expiryTime; }

void Booking::addSeat(const std::string& seatId) {
    if (std::find(seatIds.begin(), seatIds.end(), seatId) == seatIds.end()) {
        seatIds.push_back(seatId);
    }
}

void Booking::removeSeat(const std::string& seatId) {
    seatIds.erase(std::remove(seatIds.begin(), seatIds.end(), seatId), seatIds.end());
}

bool Booking::hasSeat(const std::string& seatId) const {
    return std::find(seatIds.begin(), seatIds.end(), seatId) != seatIds.end();
}

int Booking::getSeatCount() const { return seatIds.size(); }

bool Booking::confirmBooking() {
    if (status == BookingStatus::PENDING) {
        status = BookingStatus::CONFIRMED;
        return true;
    }
    return false;
}

bool Booking::cancelBooking() {
    if (status == BookingStatus::PENDING || status == BookingStatus::CONFIRMED) {
        status = BookingStatus::CANCELLED;
        return true;
    }
    return false;
}

bool Booking::expireBooking() {
    if (status == BookingStatus::PENDING) {
        status = BookingStatus::EXPIRED;
        return true;
    }
    return false;
}

bool Booking::completeBooking() {
    if (status == BookingStatus::CONFIRMED) {
        status = BookingStatus::COMPLETED;
        return true;
    }
    return false;
}

std::string Booking::getStatusString() const {
    switch (status) {
        case BookingStatus::PENDING: return "Pending";
        case BookingStatus::CONFIRMED: return "Confirmed";
        case BookingStatus::CANCELLED: return "Cancelled";
        case BookingStatus::EXPIRED: return "Expired";
        case BookingStatus::COMPLETED: return "Completed";
        default: return "Unknown";
    }
}

std::string Booking::getBookingTimeString() const {
    auto time_t = std::chrono::system_clock::to_time_t(bookingTime);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Booking::getExpiryTimeString() const {
    auto time_t = std::chrono::system_clock::to_time_t(expiryTime);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

bool Booking::isConfirmed() const { return status == BookingStatus::CONFIRMED; }
bool Booking::isCancelled() const { return status == BookingStatus::CANCELLED; }
bool Booking::isExpired() const { return status == BookingStatus::EXPIRED; }
bool Booking::isCompleted() const { return status == BookingStatus::COMPLETED; }
bool Booking::isPending() const { return status == BookingStatus::PENDING; }

bool Booking::isExpiredNow() const {
    return std::chrono::system_clock::now() > expiryTime;
}

void Booking::calculateExpiryTime(int expiryMinutes) {
    expiryTime = bookingTime + std::chrono::minutes(expiryMinutes);
}

std::string Booking::getBookingInfo() const {
    std::ostringstream oss;
    oss << "Booking ID: " << id << "\n";
    oss << "Booking Code: " << bookingCode << "\n";
    if (user) {
        oss << "User: " << user->getName() << " (" << user->getEmail() << ")\n";
    }
    if (show && show->getMovie()) {
        oss << "Movie: " << show->getMovie()->getTitle() << "\n";
    }
    if (show) {
        oss << "Show Time: " << show->getShowTimeString() << "\n";
    }
    oss << "Seats: ";
    for (size_t i = 0; i < seatIds.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << seatIds[i];
    }
    oss << "\n";
    oss << "Total Amount: $" << std::fixed << std::setprecision(2) << totalAmount << "\n";
    oss << "Status: " << getStatusString() << "\n";
    oss << "Booking Time: " << getBookingTimeString() << "\n";
    oss << "Expiry Time: " << getExpiryTimeString();
    if (payment) {
        oss << "\nPayment Status: " << payment->getStatusString();
    }
    return oss.str();
}

std::string Booking::generateBookingCode() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000, 999999);
    
    std::ostringstream oss;
    oss << "BK" << dis(gen);
    return oss.str();
} 