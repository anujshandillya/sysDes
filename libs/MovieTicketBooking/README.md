# Movie Ticket Booking System

A comprehensive C++ implementation of a movie ticket booking system that manages movies, theaters, shows, users, bookings, and payments.

## System Overview

The Movie Ticket Booking System is designed to handle the complete lifecycle of movie ticket booking, from movie management to payment processing. It provides a robust, scalable architecture that can be easily extended and maintained.

## Features

### Core Features
- **Movie Management**: Add, remove, and manage movies with details like title, genre, duration, rating, director, cast, and language
- **Theater Management**: Manage multiple theaters with screens and seating arrangements
- **Show Scheduling**: Schedule movie shows at specific times on specific screens
- **User Management**: Register and manage users with different types (Regular, Premium, VIP)
- **Seat Management**: Dynamic seat allocation with different seat types (Regular, Premium, VIP, Wheelchair Accessible)
- **Booking System**: Create, confirm, cancel, and manage ticket bookings
- **Payment Processing**: Handle multiple payment methods with transaction tracking
- **Search and Query**: Advanced search functionality for movies, shows, and theaters

### Advanced Features
- **Real-time Seat Status**: Track seat availability, reservation, and occupancy
- **Booking Expiry**: Automatic expiry of pending bookings
- **Show Status Management**: Automatic status updates (Scheduled → Running → Completed)
- **Payment Simulation**: Simulated payment processing with success/failure scenarios
- **Booking Codes**: Unique booking codes for ticket verification
- **Price Calculation**: Dynamic pricing based on seat type and base show price

## Architecture

### Class Structure

```
MovieTicketBookingSystem (Main Controller)
├── Movie (Movie Information)
├── Theater (Theater Management)
│   └── Screen (Screen with Seating)
│       └── Seat (Individual Seat)
├── Show (Movie + Time + Screen)
├── User (Customer Information)
├── Booking (Reservation Details)
└── Payment (Payment Processing)
```

### Key Classes

#### 1. Movie
- **Purpose**: Represents a movie with all its details
- **Key Features**: Genre, rating, duration, cast, director information
- **Enums**: `Genre` (Action, Comedy, Drama, etc.), `Rating` (G, PG, PG-13, R, NC-17)

#### 2. Theater
- **Purpose**: Manages theater information and multiple screens
- **Key Features**: Location, address, phone, screen management
- **Relationships**: Contains multiple Screen objects

#### 3. Screen
- **Purpose**: Represents a theater screen with seating arrangement
- **Key Features**: Configurable rows and columns, seat management
- **Visualization**: Provides seat layout string representation

#### 4. Seat
- **Purpose**: Individual seat with status and pricing
- **Key Features**: Seat status (Available, Reserved, Occupied, Maintenance), seat types, pricing
- **Enums**: `SeatStatus`, `SeatType` (Regular, Premium, VIP, Wheelchair Accessible)

#### 5. Show
- **Purpose**: Represents a movie screening at a specific time
- **Key Features**: Show time, end time, status management, booking tracking
- **Enums**: `ShowStatus` (Scheduled, Running, Completed, Cancelled)

#### 6. User
- **Purpose**: Customer information and booking history
- **Key Features**: Authentication, user types, booking tracking
- **Enums**: `UserType` (Regular, Premium, VIP)

#### 7. Booking
- **Purpose**: Ticket reservation with seat selection
- **Key Features**: Booking status, expiry management, booking codes
- **Enums**: `BookingStatus` (Pending, Confirmed, Cancelled, Expired, Completed)

#### 8. Payment
- **Purpose**: Payment processing and transaction management
- **Key Features**: Multiple payment methods, transaction tracking, refund support
- **Enums**: `PaymentMethod` (Credit Card, Debit Card, UPI, etc.), `PaymentStatus`

#### 9. MovieTicketBookingSystem
- **Purpose**: Main controller orchestrating all components
- **Key Features**: CRUD operations, search functionality, system management

## Usage Examples

### Basic Setup

```cpp
#include "MovieTicketBookingSystem.hpp"

// Create system instance
MovieTicketBookingSystem bookingSystem;

// Add a movie
std::vector<std::string> cast = {"Tom Hanks", "Robin Wright"};
Movie* movie = bookingSystem.addMovie(
    "Forrest Gump",
    "The story of a simple man who experiences extraordinary events",
    Genre::DRAMA,
    142,
    Rating::PG13,
    "Robert Zemeckis",
    cast,
    "English"
);

// Add a theater
Theater* theater = bookingSystem.addTheater(
    "Cineplex Downtown",
    "Downtown",
    "123 Main Street, Downtown",
    "+1-555-0123"
);

// Add a screen to the theater
Screen* screen = bookingSystem.addScreenToTheater(
    theater->getId(),
    "Screen 1",
    10,  // rows
    15   // columns
);

// Schedule a show
auto showTime = std::chrono::system_clock::now() + std::chrono::hours(24);
Show* show = bookingSystem.addShow(
    movie->getId(),
    theater->getId(),
    screen->getId(),
    showTime,
    12.50  // base price
);
```

### User Registration and Booking

```cpp
// Register a user
User* user = bookingSystem.addUser(
    "John Doe",
    "john.doe@email.com",
    "+1-555-0124",
    "password123",
    UserType::REGULAR
);

// Get available seats
std::vector<Seat*> availableSeats = bookingSystem.getAvailableSeats(show->getId());

// Select seats
std::vector<std::string> selectedSeats = {availableSeats[0]->getId(), availableSeats[1]->getId()};

// Create booking
Booking* booking = bookingSystem.createBooking(
    user->getId(),
    show->getId(),
    selectedSeats
);

// Process payment
Payment* payment = bookingSystem.createPayment(
    booking->getId(),
    booking->getTotalAmount(),
    PaymentMethod::CREDIT_CARD
);

bool paymentSuccess = bookingSystem.processPayment(payment->getId());
if (paymentSuccess) {
    bookingSystem.confirmBooking(booking->getId());
}
```

### Search and Query

```cpp
// Search for movies
std::vector<Movie*> actionMovies = bookingSystem.searchMovies("", Genre::ACTION);

// Search for shows by location
auto tomorrow = std::chrono::system_clock::now() + std::chrono::hours(24);
std::vector<Show*> downtownShows = bookingSystem.searchShows("", "Downtown", tomorrow);

// Get user's booking history
std::vector<Booking*> userBookings = bookingSystem.getBookingsByUser(user->getId());

// Get theater information
std::vector<Theater*> downtownTheaters = bookingSystem.searchTheaters("Downtown");
```

### System Management

```cpp
// Clean up expired bookings
bookingSystem.cleanupExpiredBookings();

// Update show statuses
bookingSystem.updateShowStatuses();

// Get system status
std::string status = bookingSystem.getSystemStatus();
std::cout << status << std::endl;
```

## File Structure

```
libs/MovieTicketBooking/
├── include/
│   ├── Movie.hpp
│   ├── Theater.hpp
│   ├── Screen.hpp
│   ├── Seat.hpp
│   ├── Show.hpp
│   ├── User.hpp
│   ├── Booking.hpp
│   ├── Payment.hpp
│   └── MovieTicketBookingSystem.hpp
├── src/
│   ├── Movie.cpp
│   ├── Theater.cpp
│   ├── Screen.cpp
│   ├── Seat.cpp
│   ├── Show.cpp
│   ├── User.cpp
│   ├── Booking.cpp
│   ├── Payment.cpp
│   └── MovieTicketBookingSystem.cpp
└── README.md
```

## Design Patterns Used

1. **Singleton-like Pattern**: MovieTicketBookingSystem acts as a central controller
2. **Factory Pattern**: ID generation and object creation
3. **Observer Pattern**: Status updates and notifications
4. **Strategy Pattern**: Different payment methods and seat pricing
5. **Composite Pattern**: Theater contains Screens, Screens contain Seats

## Key Features Implementation

### Seat Management
- Dynamic seat allocation with configurable rows and columns
- Real-time seat status tracking
- Different seat types with varying pricing
- Visual seat layout representation

### Booking System
- Atomic booking operations (all-or-nothing seat reservation)
- Automatic booking expiry (15 minutes default)
- Unique booking codes for verification
- Comprehensive booking status management

### Payment Processing
- Simulated payment gateway integration
- Multiple payment method support
- Transaction ID generation
- Refund and cancellation support

### Search and Query
- Multi-criteria search functionality
- Efficient lookup using hash maps
- Flexible search parameters

## Error Handling

The system includes comprehensive error handling:
- Null pointer checks for all object references
- Validation of input parameters
- Graceful handling of failed operations
- Status checking before operations

## Performance Considerations

- **Efficient Lookups**: Uses hash maps for O(1) object retrieval
- **Memory Management**: Smart pointers for automatic memory management
- **Scalable Design**: Modular architecture allows for easy scaling
- **Optimized Searches**: Efficient search algorithms with early termination

## Future Enhancements

1. **Database Integration**: Persistent storage for all data
2. **Web API**: RESTful API for web/mobile applications
3. **Real-time Notifications**: WebSocket support for live updates
4. **Advanced Analytics**: Booking patterns and revenue analysis
5. **Multi-language Support**: Internationalization
6. **Discount System**: Coupons, loyalty programs, and seasonal discounts
7. **Concurrent Booking**: Thread-safe operations for high-traffic scenarios

## Dependencies

- C++17 or later
- Standard Library components:
  - `<string>`, `<vector>`, `<map>`, `<memory>`
  - `<chrono>`, `<algorithm>`, `<sstream>`
  - `<iomanip>`, `<random>`, `<regex>`

## Compilation

The system is designed to be compiled with any C++17 compliant compiler. No external dependencies are required beyond the standard library.

```bash
# Example compilation command
g++ -std=c++17 -Iinclude src/*.cpp -o movie_booking_system
```

## Testing

The system is designed to be easily testable with unit tests. Each class has clear interfaces and minimal dependencies, making it suitable for comprehensive testing frameworks.

## License

This implementation is provided as a reference design for educational and development purposes. 