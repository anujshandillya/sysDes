#ifndef MOVIE_TICKET_BOOKING_SYSTEM_HPP
#define MOVIE_TICKET_BOOKING_SYSTEM_HPP

#include "Movie.hpp"
#include "Theater.hpp"
#include "Show.hpp"
#include "User.hpp"
#include "Booking.hpp"
#include "Payment.hpp"
#include <vector>
#include <map>
#include <memory>
#include <string>

class MovieTicketBookingSystem {
private:
    std::vector<std::unique_ptr<Movie>> movies;
    std::vector<std::unique_ptr<Theater>> theaters;
    std::vector<std::unique_ptr<Show>> shows;
    std::vector<std::unique_ptr<User>> users;
    std::vector<std::unique_ptr<Booking>> bookings;
    std::vector<std::unique_ptr<Payment>> payments;
    
    // Maps for quick lookups
    std::map<std::string, Movie*> movieMap;
    std::map<std::string, Theater*> theaterMap;
    std::map<std::string, Show*> showMap;
    std::map<std::string, User*> userMap;
    std::map<std::string, Booking*> bookingMap;
    std::map<std::string, Payment*> paymentMap;

public:
    MovieTicketBookingSystem();
    ~MovieTicketBookingSystem();
    
    // Movie management
    Movie* addMovie(const std::string& title, const std::string& description,
                   Genre genre, int duration, Rating rating, const std::string& director,
                   const std::vector<std::string>& cast, const std::string& language);
    bool removeMovie(const std::string& movieId);
    Movie* getMovie(const std::string& movieId);
    std::vector<Movie*> getAllMovies() const;
    std::vector<Movie*> getMoviesByGenre(Genre genre) const;
    std::vector<Movie*> getActiveMovies() const;
    
    // Theater management
    Theater* addTheater(const std::string& name, const std::string& location,
                       const std::string& address, const std::string& phone);
    bool removeTheater(const std::string& theaterId);
    Theater* getTheater(const std::string& theaterId);
    std::vector<Theater*> getAllTheaters() const;
    std::vector<Theater*> getTheatersByLocation(const std::string& location) const;
    
    // Screen management
    Screen* addScreenToTheater(const std::string& theaterId, const std::string& screenName,
                              int totalRows, int totalColumns);
    bool removeScreenFromTheater(const std::string& theaterId, const std::string& screenId);
    Screen* getScreen(const std::string& theaterId, const std::string& screenId);
    
    // Show management
    Show* addShow(const std::string& movieId, const std::string& theaterId,
                  const std::string& screenId, const std::chrono::system_clock::time_point& showTime,
                  double basePrice);
    bool removeShow(const std::string& showId);
    Show* getShow(const std::string& showId);
    std::vector<Show*> getAllShows() const;
    std::vector<Show*> getShowsByMovie(const std::string& movieId) const;
    std::vector<Show*> getShowsByTheater(const std::string& theaterId) const;
    std::vector<Show*> getUpcomingShows() const;
    
    // User management
    User* addUser(const std::string& name, const std::string& email, const std::string& phone,
                  const std::string& password, UserType type = UserType::REGULAR);
    bool removeUser(const std::string& userId);
    User* getUser(const std::string& userId);
    User* getUserByEmail(const std::string& email);
    std::vector<User*> getAllUsers() const;
    bool authenticateUser(const std::string& email, const std::string& password);
    
    // Booking management
    Booking* createBooking(const std::string& userId, const std::string& showId,
                          const std::vector<std::string>& seatIds);
    bool cancelBooking(const std::string& bookingId);
    bool confirmBooking(const std::string& bookingId);
    Booking* getBooking(const std::string& bookingId);
    std::vector<Booking*> getBookingsByUser(const std::string& userId) const;
    std::vector<Booking*> getBookingsByShow(const std::string& showId) const;
    std::vector<Booking*> getAllBookings() const;
    
    // Payment management
    Payment* createPayment(const std::string& bookingId, double amount, PaymentMethod method);
    bool processPayment(const std::string& paymentId);
    bool refundPayment(const std::string& paymentId);
    Payment* getPayment(const std::string& paymentId);
    std::vector<Payment*> getPaymentsByBooking(const std::string& bookingId) const;
    
    // Seat management
    bool reserveSeats(const std::string& showId, const std::vector<std::string>& seatIds,
                     const std::string& bookingId);
    bool releaseSeats(const std::string& showId, const std::vector<std::string>& seatIds);
    std::vector<Seat*> getAvailableSeats(const std::string& showId) const;
    std::vector<Seat*> getReservedSeats(const std::string& showId) const;
    bool isSeatAvailable(const std::string& showId, const std::string& seatId) const;
    
    // Search and query methods
    std::vector<Show*> searchShows(const std::string& movieTitle, const std::string& location,
                                  const std::chrono::system_clock::time_point& date) const;
    std::vector<Movie*> searchMovies(const std::string& title, Genre genre) const;
    std::vector<Theater*> searchTheaters(const std::string& location) const;
    
    // Utility methods
    void cleanupExpiredBookings();
    void updateShowStatuses();
    std::string generateId() const;
    double calculateTotalAmount(const std::string& showId, const std::vector<std::string>& seatIds) const;
    std::string getSystemStatus() const;
};

#endif // MOVIE_TICKET_BOOKING_SYSTEM_HPP 