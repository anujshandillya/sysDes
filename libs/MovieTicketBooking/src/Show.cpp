#include "../include/Show.hpp"
#include <sstream>
#include <algorithm>
#include <iomanip>

Show::Show(const std::string& id, Movie* movie, Screen* screen, 
           const std::chrono::system_clock::time_point& showTime, double basePrice)
    : id(id), movie(movie), screen(screen), showTime(showTime), basePrice(basePrice), 
      status(ShowStatus::SCHEDULED) {
    calculateEndTime();
}

std::string Show::getId() const { return id; }
Movie* Show::getMovie() const { return movie; }
Screen* Show::getScreen() const { return screen; }
std::chrono::system_clock::time_point Show::getShowTime() const { return showTime; }
std::chrono::system_clock::time_point Show::getEndTime() const { return endTime; }
ShowStatus Show::getStatus() const { return status; }
double Show::getBasePrice() const { return basePrice; }
std::vector<std::string> Show::getBookingIds() const { return bookingIds; }

void Show::setShowTime(const std::chrono::system_clock::time_point& showTime) { 
    this->showTime = showTime; 
    calculateEndTime();
}
void Show::setStatus(ShowStatus status) { this->status = status; }
void Show::setBasePrice(double basePrice) { this->basePrice = basePrice; }

void Show::addBooking(const std::string& bookingId) {
    if (std::find(bookingIds.begin(), bookingIds.end(), bookingId) == bookingIds.end()) {
        bookingIds.push_back(bookingId);
    }
}

void Show::removeBooking(const std::string& bookingId) {
    bookingIds.erase(std::remove(bookingIds.begin(), bookingIds.end(), bookingId), bookingIds.end());
}

bool Show::hasBooking(const std::string& bookingId) const {
    return std::find(bookingIds.begin(), bookingIds.end(), bookingId) != bookingIds.end();
}

int Show::getBookingCount() const { return bookingIds.size(); }

std::string Show::getStatusString() const {
    switch (status) {
        case ShowStatus::SCHEDULED: return "Scheduled";
        case ShowStatus::RUNNING: return "Running";
        case ShowStatus::COMPLETED: return "Completed";
        case ShowStatus::CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

std::string Show::getShowTimeString() const {
    auto time_t = std::chrono::system_clock::to_time_t(showTime);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M");
    return oss.str();
}

std::string Show::getEndTimeString() const {
    auto time_t = std::chrono::system_clock::to_time_t(endTime);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M");
    return oss.str();
}

std::string Show::getDurationString() const {
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(endTime - showTime);
    int hours = duration.count() / 60;
    int minutes = duration.count() % 60;
    std::ostringstream oss;
    if (hours > 0) {
        oss << hours << "h ";
    }
    if (minutes > 0 || hours == 0) {
        oss << minutes << "m";
    }
    return oss.str();
}

bool Show::isUpcoming() const { return status == ShowStatus::SCHEDULED; }
bool Show::isRunning() const { return status == ShowStatus::RUNNING; }
bool Show::isCompleted() const { return status == ShowStatus::COMPLETED; }
bool Show::isCancelled() const { return status == ShowStatus::CANCELLED; }

double Show::getSeatPrice(const std::string& seatId) const {
    if (!screen) return basePrice;
    Seat* seat = screen->getSeat(seatId);
    if (seat) {
        return basePrice + seat->getPrice();
    }
    return basePrice;
}

double Show::getSeatPrice(int row, int column) const {
    if (!screen) return basePrice;
    Seat* seat = screen->getSeat(row, column);
    if (seat) {
        return basePrice + seat->getPrice();
    }
    return basePrice;
}

void Show::calculateEndTime() {
    if (movie) {
        endTime = showTime + std::chrono::minutes(movie->getDuration());
    } else {
        endTime = showTime + std::chrono::hours(2); // Default 2 hours
    }
}

std::string Show::getShowInfo() const {
    std::ostringstream oss;
    oss << "Show ID: " << id << "\n";
    if (movie) {
        oss << "Movie: " << movie->getTitle() << "\n";
        oss << "Genre: " << movie->getGenreString() << "\n";
        oss << "Duration: " << movie->getDurationString() << "\n";
    }
    if (screen) {
        oss << "Screen: " << screen->getName() << "\n";
    }
    oss << "Show Time: " << getShowTimeString() << "\n";
    oss << "End Time: " << getEndTimeString() << "\n";
    oss << "Status: " << getStatusString() << "\n";
    oss << "Base Price: $" << std::fixed << std::setprecision(2) << basePrice << "\n";
    oss << "Bookings: " << getBookingCount();
    return oss.str();
} 