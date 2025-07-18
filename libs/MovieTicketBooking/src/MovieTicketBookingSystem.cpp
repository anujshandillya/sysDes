#include "../include/MovieTicketBookingSystem.hpp"
#include <algorithm>
#include <sstream>
#include <chrono>

MovieTicketBookingSystem::MovieTicketBookingSystem() {}

MovieTicketBookingSystem::~MovieTicketBookingSystem() {}

// Movie management
Movie* MovieTicketBookingSystem::addMovie(const std::string& title, const std::string& description,
                                         Genre genre, int duration, Rating rating, const std::string& director,
                                         const std::vector<std::string>& cast, const std::string& language) {
    std::string id = generateId();
    auto movie = std::make_unique<Movie>(id, title, description, genre, duration, rating, director, cast, language);
    Movie* moviePtr = movie.get();
    movies.push_back(std::move(movie));
    movieMap[id] = moviePtr;
    return moviePtr;
}

bool MovieTicketBookingSystem::removeMovie(const std::string& movieId) {
    auto it = std::find_if(movies.begin(), movies.end(),
                          [&movieId](const std::unique_ptr<Movie>& movie) {
                              return movie->getId() == movieId;
                          });
    if (it != movies.end()) {
        movieMap.erase(movieId);
        movies.erase(it);
        return true;
    }
    return false;
}

Movie* MovieTicketBookingSystem::getMovie(const std::string& movieId) {
    auto it = movieMap.find(movieId);
    return (it != movieMap.end()) ? it->second : nullptr;
}

std::vector<Movie*> MovieTicketBookingSystem::getAllMovies() const {
    std::vector<Movie*> result;
    for (const auto& movie : movies) {
        result.push_back(movie.get());
    }
    return result;
}

std::vector<Movie*> MovieTicketBookingSystem::getMoviesByGenre(Genre genre) const {
    std::vector<Movie*> result;
    for (const auto& movie : movies) {
        if (movie->getGenre() == genre && movie->getIsActive()) {
            result.push_back(movie.get());
        }
    }
    return result;
}

std::vector<Movie*> MovieTicketBookingSystem::getActiveMovies() const {
    std::vector<Movie*> result;
    for (const auto& movie : movies) {
        if (movie->getIsActive()) {
            result.push_back(movie.get());
        }
    }
    return result;
}

// Theater management
Theater* MovieTicketBookingSystem::addTheater(const std::string& name, const std::string& location,
                                             const std::string& address, const std::string& phone) {
    std::string id = generateId();
    auto theater = std::make_unique<Theater>(id, name, location, address, phone);
    Theater* theaterPtr = theater.get();
    theaters.push_back(std::move(theater));
    theaterMap[id] = theaterPtr;
    return theaterPtr;
}

bool MovieTicketBookingSystem::removeTheater(const std::string& theaterId) {
    auto it = std::find_if(theaters.begin(), theaters.end(),
                          [&theaterId](const std::unique_ptr<Theater>& theater) {
                              return theater->getId() == theaterId;
                          });
    if (it != theaters.end()) {
        theaterMap.erase(theaterId);
        theaters.erase(it);
        return true;
    }
    return false;
}

Theater* MovieTicketBookingSystem::getTheater(const std::string& theaterId) {
    auto it = theaterMap.find(theaterId);
    return (it != theaterMap.end()) ? it->second : nullptr;
}

std::vector<Theater*> MovieTicketBookingSystem::getAllTheaters() const {
    std::vector<Theater*> result;
    for (const auto& theater : theaters) {
        result.push_back(theater.get());
    }
    return result;
}

std::vector<Theater*> MovieTicketBookingSystem::getTheatersByLocation(const std::string& location) const {
    std::vector<Theater*> result;
    for (const auto& theater : theaters) {
        if (theater->getLocation() == location && theater->getIsActive()) {
            result.push_back(theater.get());
        }
    }
    return result;
}

// Screen management
Screen* MovieTicketBookingSystem::addScreenToTheater(const std::string& theaterId, const std::string& screenName,
                                                    int totalRows, int totalColumns) {
    Theater* theater = getTheater(theaterId);
    if (!theater) return nullptr;
    
    std::string screenId = generateId();
    Screen screen(screenId, screenName, totalRows, totalColumns);
    theater->addScreen(screen);
    return theater->getScreen(screenId);
}

bool MovieTicketBookingSystem::removeScreenFromTheater(const std::string& theaterId, const std::string& screenId) {
    Theater* theater = getTheater(theaterId);
    if (!theater) return false;
    return theater->removeScreen(screenId);
}

Screen* MovieTicketBookingSystem::getScreen(const std::string& theaterId, const std::string& screenId) {
    Theater* theater = getTheater(theaterId);
    if (!theater) return nullptr;
    return theater->getScreen(screenId);
}

// Show management
Show* MovieTicketBookingSystem::addShow(const std::string& movieId, const std::string& theaterId,
                                       const std::string& screenId, const std::chrono::system_clock::time_point& showTime,
                                       double basePrice) {
    Movie* movie = getMovie(movieId);
    Screen* screen = getScreen(theaterId, screenId);
    if (!movie || !screen) return nullptr;
    
    std::string id = generateId();
    auto show = std::make_unique<Show>(id, movie, screen, showTime, basePrice);
    Show* showPtr = show.get();
    shows.push_back(std::move(show));
    showMap[id] = showPtr;
    return showPtr;
}

bool MovieTicketBookingSystem::removeShow(const std::string& showId) {
    auto it = std::find_if(shows.begin(), shows.end(),
                          [&showId](const std::unique_ptr<Show>& show) {
                              return show->getId() == showId;
                          });
    if (it != shows.end()) {
        showMap.erase(showId);
        shows.erase(it);
        return true;
    }
    return false;
}

Show* MovieTicketBookingSystem::getShow(const std::string& showId) {
    auto it = showMap.find(showId);
    return (it != showMap.end()) ? it->second : nullptr;
}

std::vector<Show*> MovieTicketBookingSystem::getAllShows() const {
    std::vector<Show*> result;
    for (const auto& show : shows) {
        result.push_back(show.get());
    }
    return result;
}

std::vector<Show*> MovieTicketBookingSystem::getShowsByMovie(const std::string& movieId) const {
    std::vector<Show*> result;
    for (const auto& show : shows) {
        if (show->getMovie() && show->getMovie()->getId() == movieId) {
            result.push_back(show.get());
        }
    }
    return result;
}

std::vector<Show*> MovieTicketBookingSystem::getShowsByTheater(const std::string& theaterId) const {
    std::vector<Show*> result;
    auto it = theaterMap.find(theaterId);
    Theater* theater = (it != theaterMap.end()) ? it->second : nullptr;
    
    for (const auto& show : shows) {
        if (show->getScreen() && theater) {
            if (theater->hasScreen(show->getScreen()->getId())) {
                result.push_back(show.get());
            }
        }
    }
    return result;
}

std::vector<Show*> MovieTicketBookingSystem::getUpcomingShows() const {
    std::vector<Show*> result;
    auto now = std::chrono::system_clock::now();
    for (const auto& show : shows) {
        if (show->getShowTime() > now && show->isUpcoming()) {
            result.push_back(show.get());
        }
    }
    return result;
}

// User management
User* MovieTicketBookingSystem::addUser(const std::string& name, const std::string& email, const std::string& phone,
                                       const std::string& password, UserType type) {
    std::string id = generateId();
    auto user = std::make_unique<User>(id, name, email, phone, password, type);
    User* userPtr = user.get();
    users.push_back(std::move(user));
    userMap[id] = userPtr;
    return userPtr;
}

bool MovieTicketBookingSystem::removeUser(const std::string& userId) {
    auto it = std::find_if(users.begin(), users.end(),
                          [&userId](const std::unique_ptr<User>& user) {
                              return user->getId() == userId;
                          });
    if (it != users.end()) {
        userMap.erase(userId);
        users.erase(it);
        return true;
    }
    return false;
}

User* MovieTicketBookingSystem::getUser(const std::string& userId) {
    auto it = userMap.find(userId);
    return (it != userMap.end()) ? it->second : nullptr;
}

User* MovieTicketBookingSystem::getUserByEmail(const std::string& email) {
    for (const auto& user : users) {
        if (user->getEmail() == email) {
            return user.get();
        }
    }
    return nullptr;
}

std::vector<User*> MovieTicketBookingSystem::getAllUsers() const {
    std::vector<User*> result;
    for (const auto& user : users) {
        result.push_back(user.get());
    }
    return result;
}

bool MovieTicketBookingSystem::authenticateUser(const std::string& email, const std::string& password) {
    User* user = getUserByEmail(email);
    if (user && user->getPassword() == password && user->getIsActive()) {
        return true;
    }
    return false;
}

// Booking management
Booking* MovieTicketBookingSystem::createBooking(const std::string& userId, const std::string& showId,
                                                const std::vector<std::string>& seatIds) {
    User* user = getUser(userId);
    Show* show = getShow(showId);
    if (!user || !show) return nullptr;
    
    // Calculate total amount
    double totalAmount = calculateTotalAmount(showId, seatIds);
    
    std::string id = generateId();
    auto booking = std::make_unique<Booking>(id, user, show, seatIds, totalAmount);
    Booking* bookingPtr = booking.get();
    bookings.push_back(std::move(booking));
    bookingMap[id] = bookingPtr;
    
    // Add booking to user and show
    user->addBooking(id);
    show->addBooking(id);
    
    return bookingPtr;
}

bool MovieTicketBookingSystem::cancelBooking(const std::string& bookingId) {
    Booking* booking = getBooking(bookingId);
    if (!booking) return false;
    
    if (booking->cancelBooking()) {
        // Release seats
        Show* show = booking->getShow();
        if (show && show->getScreen()) {
            for (const auto& seatId : booking->getSeatIds()) {
                show->getScreen()->releaseSeat(seatId);
            }
        }
        return true;
    }
    return false;
}

bool MovieTicketBookingSystem::confirmBooking(const std::string& bookingId) {
    Booking* booking = getBooking(bookingId);
    if (!booking) return false;
    return booking->confirmBooking();
}

Booking* MovieTicketBookingSystem::getBooking(const std::string& bookingId) {
    auto it = bookingMap.find(bookingId);
    return (it != bookingMap.end()) ? it->second : nullptr;
}

std::vector<Booking*> MovieTicketBookingSystem::getBookingsByUser(const std::string& userId) const {
    std::vector<Booking*> result;
    for (const auto& booking : bookings) {
        if (booking->getUser() && booking->getUser()->getId() == userId) {
            result.push_back(booking.get());
        }
    }
    return result;
}

std::vector<Booking*> MovieTicketBookingSystem::getBookingsByShow(const std::string& showId) const {
    std::vector<Booking*> result;
    for (const auto& booking : bookings) {
        if (booking->getShow() && booking->getShow()->getId() == showId) {
            result.push_back(booking.get());
        }
    }
    return result;
}

std::vector<Booking*> MovieTicketBookingSystem::getAllBookings() const {
    std::vector<Booking*> result;
    for (const auto& booking : bookings) {
        result.push_back(booking.get());
    }
    return result;
}

// Payment management
Payment* MovieTicketBookingSystem::createPayment(const std::string& bookingId, double amount, PaymentMethod method) {
    std::string id = generateId();
    auto payment = std::make_unique<Payment>(id, bookingId, amount, method);
    Payment* paymentPtr = payment.get();
    payments.push_back(std::move(payment));
    paymentMap[id] = paymentPtr;
    
    // Link payment to booking
    Booking* booking = getBooking(bookingId);
    if (booking) {
        booking->setPayment(paymentPtr);
    }
    
    return paymentPtr;
}

bool MovieTicketBookingSystem::processPayment(const std::string& paymentId) {
    Payment* payment = getPayment(paymentId);
    if (!payment) return false;
    return payment->processPayment();
}

bool MovieTicketBookingSystem::refundPayment(const std::string& paymentId) {
    Payment* payment = getPayment(paymentId);
    if (!payment) return false;
    return payment->refundPayment();
}

Payment* MovieTicketBookingSystem::getPayment(const std::string& paymentId) {
    auto it = paymentMap.find(paymentId);
    return (it != paymentMap.end()) ? it->second : nullptr;
}

std::vector<Payment*> MovieTicketBookingSystem::getPaymentsByBooking(const std::string& bookingId) const {
    std::vector<Payment*> result;
    for (const auto& payment : payments) {
        if (payment->getBookingId() == bookingId) {
            result.push_back(payment.get());
        }
    }
    return result;
}

// Seat management
bool MovieTicketBookingSystem::reserveSeats(const std::string& showId, const std::vector<std::string>& seatIds,
                                           const std::string& bookingId) {
    Show* show = getShow(showId);
    if (!show || !show->getScreen()) return false;
    
    bool allReserved = true;
    for (const auto& seatId : seatIds) {
        if (!show->getScreen()->reserveSeat(seatId, bookingId)) {
            allReserved = false;
            break;
        }
    }
    
    if (!allReserved) {
        // Release any seats that were reserved
        for (const auto& seatId : seatIds) {
            show->getScreen()->releaseSeat(seatId);
        }
    }
    
    return allReserved;
}

bool MovieTicketBookingSystem::releaseSeats(const std::string& showId, const std::vector<std::string>& seatIds) {
    Show* show = getShow(showId);
    if (!show || !show->getScreen()) return false;
    
    for (const auto& seatId : seatIds) {
        show->getScreen()->releaseSeat(seatId);
    }
    return true;
}

std::vector<Seat*> MovieTicketBookingSystem::getAvailableSeats(const std::string& showId) const {
    auto it = showMap.find(showId);
    Show* show = (it != showMap.end()) ? it->second : nullptr;
    if (!show || !show->getScreen()) return {};
    return show->getScreen()->getAvailableSeats();
}

std::vector<Seat*> MovieTicketBookingSystem::getReservedSeats(const std::string& showId) const {
    auto it = showMap.find(showId);
    Show* show = (it != showMap.end()) ? it->second : nullptr;
    if (!show || !show->getScreen()) return {};
    return show->getScreen()->getReservedSeats();
}

bool MovieTicketBookingSystem::isSeatAvailable(const std::string& showId, const std::string& seatId) const {
    auto it = showMap.find(showId);
    Show* show = (it != showMap.end()) ? it->second : nullptr;
    if (!show || !show->getScreen()) return false;
    return show->getScreen()->isSeatAvailable(seatId);
}

// Search and query methods
std::vector<Show*> MovieTicketBookingSystem::searchShows(const std::string& movieTitle, const std::string& location,
                                                        const std::chrono::system_clock::time_point& date) const {
    std::vector<Show*> result;
    for (const auto& show : shows) {
        bool matches = true;
        
        if (!movieTitle.empty() && show->getMovie()) {
            if (show->getMovie()->getTitle().find(movieTitle) == std::string::npos) {
                matches = false;
            }
        }
        
        if (!location.empty() && show->getScreen()) {
            // Check if screen belongs to a theater in the specified location
            bool foundLocation = false;
            for (const auto& theater : theaters) {
                if (theater->getLocation() == location && theater->hasScreen(show->getScreen()->getId())) {
                    foundLocation = true;
                    break;
                }
            }
            if (!foundLocation) matches = false;
        }
        
        if (date != std::chrono::system_clock::time_point{}) {
            auto showDate = std::chrono::system_clock::to_time_t(show->getShowTime());
            auto searchDate = std::chrono::system_clock::to_time_t(date);
            auto showTm = *std::localtime(&showDate);
            auto searchTm = *std::localtime(&searchDate);
            
            if (showTm.tm_year != searchTm.tm_year || 
                showTm.tm_mon != searchTm.tm_mon || 
                showTm.tm_mday != searchTm.tm_mday) {
                matches = false;
            }
        }
        
        if (matches) {
            result.push_back(show.get());
        }
    }
    return result;
}

std::vector<Movie*> MovieTicketBookingSystem::searchMovies(const std::string& title, Genre genre) const {
    std::vector<Movie*> result;
    for (const auto& movie : movies) {
        bool matches = true;
        
        if (!title.empty()) {
            if (movie->getTitle().find(title) == std::string::npos) {
                matches = false;
            }
        }
        
        if (genre != Genre::ACTION) { // Assuming ACTION is not a valid search genre
            if (movie->getGenre() != genre) {
                matches = false;
            }
        }
        
        if (matches && movie->getIsActive()) {
            result.push_back(movie.get());
        }
    }
    return result;
}

std::vector<Theater*> MovieTicketBookingSystem::searchTheaters(const std::string& location) const {
    return getTheatersByLocation(location);
}

// Utility methods
void MovieTicketBookingSystem::cleanupExpiredBookings() {
    auto now = std::chrono::system_clock::now();
    for (auto& booking : bookings) {
        if (booking->isPending() && booking->isExpiredNow()) {
            booking->expireBooking();
        }
    }
}

void MovieTicketBookingSystem::updateShowStatuses() {
    auto now = std::chrono::system_clock::now();
    for (auto& show : shows) {
        if (show->isUpcoming() && now >= show->getShowTime()) {
            show->setStatus(ShowStatus::RUNNING);
        } else if (show->isRunning() && now >= show->getEndTime()) {
            show->setStatus(ShowStatus::COMPLETED);
        }
    }
}

std::string MovieTicketBookingSystem::generateId() const {
    static int counter = 0;
    std::ostringstream oss;
    oss << "ID" << std::hex << std::hash<std::string>{}(std::to_string(counter++));
    return oss.str();
}

double MovieTicketBookingSystem::calculateTotalAmount(const std::string& showId, const std::vector<std::string>& seatIds) const {
    auto it = showMap.find(showId);
    Show* show = (it != showMap.end()) ? it->second : nullptr;
    if (!show) return 0.0;
    
    double total = 0.0;
    for (const auto& seatId : seatIds) {
        total += show->getSeatPrice(seatId);
    }
    return total;
}

std::string MovieTicketBookingSystem::getSystemStatus() const {
    std::ostringstream oss;
    oss << "=== Movie Ticket Booking System Status ===\n";
    oss << "Movies: " << movies.size() << "\n";
    oss << "Theaters: " << theaters.size() << "\n";
    oss << "Shows: " << shows.size() << "\n";
    oss << "Users: " << users.size() << "\n";
    oss << "Bookings: " << bookings.size() << "\n";
    oss << "Payments: " << payments.size() << "\n";
    oss << "Active Movies: " << getActiveMovies().size() << "\n";
    oss << "Upcoming Shows: " << getUpcomingShows().size() << "\n";
    return oss.str();
}