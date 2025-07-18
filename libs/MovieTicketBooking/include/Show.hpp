#ifndef SHOW_HPP
#define SHOW_HPP

#include "Movie.hpp"
#include "Screen.hpp"
#include <string>
#include <chrono>
#include <vector>

enum class ShowStatus {
    SCHEDULED,
    RUNNING,
    COMPLETED,
    CANCELLED
};

class Show {
private:
    std::string id;
    Movie* movie;
    Screen* screen;
    std::chrono::system_clock::time_point showTime;
    std::chrono::system_clock::time_point endTime;
    ShowStatus status;
    double basePrice;
    std::vector<std::string> bookingIds; // List of booking IDs for this show

public:
    Show(const std::string& id, Movie* movie, Screen* screen, 
         const std::chrono::system_clock::time_point& showTime, double basePrice);
    
    // Getters
    std::string getId() const;
    Movie* getMovie() const;
    Screen* getScreen() const;
    std::chrono::system_clock::time_point getShowTime() const;
    std::chrono::system_clock::time_point getEndTime() const;
    ShowStatus getStatus() const;
    double getBasePrice() const;
    std::vector<std::string> getBookingIds() const;
    
    // Setters
    void setShowTime(const std::chrono::system_clock::time_point& showTime);
    void setStatus(ShowStatus status);
    void setBasePrice(double basePrice);
    
    // Booking management
    void addBooking(const std::string& bookingId);
    void removeBooking(const std::string& bookingId);
    bool hasBooking(const std::string& bookingId) const;
    int getBookingCount() const;
    
    // Utility methods
    std::string getStatusString() const;
    std::string getShowTimeString() const;
    std::string getEndTimeString() const;
    std::string getDurationString() const;
    bool isUpcoming() const;
    bool isRunning() const;
    bool isCompleted() const;
    bool isCancelled() const;
    double getSeatPrice(const std::string& seatId) const;
    double getSeatPrice(int row, int column) const;
    void calculateEndTime();
    std::string getShowInfo() const;
};

#endif // SHOW_HPP 