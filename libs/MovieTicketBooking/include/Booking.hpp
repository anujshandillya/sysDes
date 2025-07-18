#ifndef BOOKING_HPP
#define BOOKING_HPP

#include "User.hpp"
#include "Show.hpp"
#include "Payment.hpp"
#include <vector>
#include <chrono>
#include <string>

enum class BookingStatus {
    PENDING,
    CONFIRMED,
    CANCELLED,
    EXPIRED,
    COMPLETED
};

class Booking {
private:
    std::string id;
    User* user;
    Show* show;
    std::vector<std::string> seatIds; // List of seat IDs booked
    std::chrono::system_clock::time_point bookingTime;
    std::chrono::system_clock::time_point expiryTime;
    BookingStatus status;
    double totalAmount;
    Payment* payment;
    std::string bookingCode; // Unique code for ticket verification

public:
    Booking(const std::string& id, User* user, Show* show, 
            const std::vector<std::string>& seatIds, double totalAmount);
    
    // Getters
    std::string getId() const;
    User* getUser() const;
    Show* getShow() const;
    std::vector<std::string> getSeatIds() const;
    std::chrono::system_clock::time_point getBookingTime() const;
    std::chrono::system_clock::time_point getExpiryTime() const;
    BookingStatus getStatus() const;
    double getTotalAmount() const;
    Payment* getPayment() const;
    std::string getBookingCode() const;
    
    // Setters
    void setStatus(BookingStatus status);
    void setTotalAmount(double amount);
    void setPayment(Payment* payment);
    void setExpiryTime(const std::chrono::system_clock::time_point& expiryTime);
    
    // Seat management
    void addSeat(const std::string& seatId);
    void removeSeat(const std::string& seatId);
    bool hasSeat(const std::string& seatId) const;
    int getSeatCount() const;
    
    // Booking operations
    bool confirmBooking();
    bool cancelBooking();
    bool expireBooking();
    bool completeBooking();
    
    // Utility methods
    std::string getStatusString() const;
    std::string getBookingTimeString() const;
    std::string getExpiryTimeString() const;
    bool isConfirmed() const;
    bool isCancelled() const;
    bool isExpired() const;
    bool isCompleted() const;
    bool isPending() const;
    bool isExpiredNow() const;
    void calculateExpiryTime(int expiryMinutes = 15); // Default 15 minutes
    std::string getBookingInfo() const;
    std::string generateBookingCode() const;
};

#endif // BOOKING_HPP 