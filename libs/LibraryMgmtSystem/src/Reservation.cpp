#include "../include/Reservation.hpp"
#include <chrono>

Reservation::Reservation(const std::string& reservationId, const std::string& memberId,
                         const std::string& bookIsbn)
    : reservationId(reservationId), memberId(memberId), bookIsbn(bookIsbn),
      reservationDate(std::chrono::system_clock::now()), status(ReservationStatus::PENDING) {
    
    // Set expiry date to 7 days from reservation date
    auto oneWeek = std::chrono::hours(24 * 7);
    expiryDate = reservationDate + oneWeek;
}

bool Reservation::isExpired() const {
    auto now = std::chrono::system_clock::now();
    return now > expiryDate;
}

std::string Reservation::getStatusString() const {
    switch (status) {
        case ReservationStatus::PENDING: return "Pending";
        case ReservationStatus::READY: return "Ready";
        case ReservationStatus::CANCELLED: return "Cancelled";
        case ReservationStatus::EXPIRED: return "Expired";
        default: return "Unknown";
    }
} 