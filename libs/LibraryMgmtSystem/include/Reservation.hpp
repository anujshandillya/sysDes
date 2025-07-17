#pragma once

#include <string>
#include <chrono>

enum class ReservationStatus {
    PENDING,
    READY,
    CANCELLED,
    EXPIRED
};

class Reservation {
private:
    std::string reservationId;
    std::string memberId;
    std::string bookIsbn;
    std::chrono::system_clock::time_point reservationDate;
    std::chrono::system_clock::time_point expiryDate;
    ReservationStatus status;
    std::string notes;

public:
    Reservation(const std::string& reservationId, const std::string& memberId,
               const std::string& bookIsbn);

    // Getters
    std::string getReservationId() const { return reservationId; }
    std::string getMemberId() const { return memberId; }
    std::string getBookIsbn() const { return bookIsbn; }
    std::chrono::system_clock::time_point getReservationDate() const { return reservationDate; }
    std::chrono::system_clock::time_point getExpiryDate() const { return expiryDate; }
    ReservationStatus getStatus() const { return status; }
    std::string getNotes() const { return notes; }

    // Setters
    void setStatus(ReservationStatus newStatus) { status = newStatus; }
    void setExpiryDate(const std::chrono::system_clock::time_point& date) { expiryDate = date; }
    void setNotes(const std::string& note) { notes = note; }

    // Utility methods
    bool isExpired() const;
    bool isReady() const { return status == ReservationStatus::READY; }
    std::string getStatusString() const;
}; 