# Cab Booking System

A comprehensive C++ implementation of a cab booking system that simulates real-world ride-hailing services like Uber, Lyft, or Ola. This system provides complete functionality for managing customers, drivers, vehicles, bookings, trips, payments, and notifications.

## 🚗 Features

### Core Functionality
- **User Management**: Customer and driver registration, authentication, and profile management
- **Vehicle Management**: Support for multiple vehicle types (Sedan, SUV, Hatchback, Bike, Auto, Premium)
- **Location Services**: GPS-based location tracking and distance calculation using Haversine formula
- **Booking System**: Real-time booking creation, driver assignment, and status tracking
- **Trip Management**: Complete trip lifecycle from request to completion
- **Payment Processing**: Multiple payment methods with transaction tracking
- **Dynamic Pricing**: Smart fare calculation based on distance, time, demand, and vehicle type
- **Notification System**: SMS, email, and push notifications for all events
- **Rating & Feedback**: Customer and driver rating system

### Advanced Features
- **Real-time Tracking**: Live location updates for drivers and vehicles
- **Nearest Driver Algorithm**: Intelligent driver assignment based on proximity
- **Fare Estimation**: Accurate fare calculation with dynamic pricing
- **Booking Expiry**: Automatic booking cancellation after timeout
- **Revenue Management**: Platform fee calculation and driver earnings tracking
- **Trip History**: Complete trip and payment history for users

## 📁 Project Structure

```
libs/CabBooking/
├── include/                    # Header files
│   ├── User.hpp               # Base user class
│   ├── Customer.hpp           # Customer-specific functionality
│   ├── Driver.hpp             # Driver-specific functionality
│   ├── Vehicle.hpp            # Vehicle management
│   ├── Location.hpp           # GPS and address management
│   ├── Trip.hpp               # Trip lifecycle management
│   ├── Booking.hpp            # Booking request management
│   ├── Payment.hpp            # Payment processing
│   ├── FareCalculator.hpp     # Dynamic pricing engine
│   ├── NotificationService.hpp # Communication system
│   └── CabBookingSystem.hpp   # Main system orchestrator
├── src/                       # Source files
│   ├── User.cpp
│   ├── Customer.cpp
│   ├── Driver.cpp
│   ├── Vehicle.cpp
│   ├── Location.cpp
│   ├── Trip.cpp
│   ├── Booking.cpp
│   ├── Payment.cpp
│   ├── FareCalculator.cpp
│   ├── NotificationService.cpp
│   └── CabBookingSystem.cpp
└── README.md                  # This file
```

## 🏗️ Architecture

### Design Patterns Used
- **Object-Oriented Design**: Clean inheritance hierarchy with base classes
- **Factory Pattern**: Centralized object creation in CabBookingSystem
- **Strategy Pattern**: Dynamic pricing algorithms in FareCalculator
- **Observer Pattern**: Notification system for real-time updates
- **Singleton-like**: Central system orchestrator

### Key Classes

#### Core Entities
- **User**: Base class for both customers and drivers
- **Customer**: Manages customer-specific data and booking history
- **Driver**: Handles driver operations, earnings, and vehicle assignment
- **Vehicle**: Represents different vehicle types with pricing and status
- **Location**: GPS coordinates and address management

#### Business Logic
- **Trip**: Complete trip lifecycle management
- **Booking**: Booking request and driver assignment
- **Payment**: Payment processing and transaction management
- **FareCalculator**: Dynamic pricing with multiple factors
- **NotificationService**: Multi-channel communication system

#### System Orchestration
- **CabBookingSystem**: Main system that coordinates all components

## 🚀 Getting Started

### Prerequisites
- C++17 or later
- CMake 3.10 or later
- Standard C++ libraries

### Building the System
```bash
# Navigate to the project directory
cd libs/CabBooking

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project
make
```

### Usage Example

```cpp
#include "CabBookingSystem.hpp"

int main() {
    // Initialize the system
    CabBookingSystem system;
    
    // Register a customer
    auto customer = system.registerCustomer("John Doe", "john@example.com", 
                                          "+1234567890", "password123");
    
    // Register a driver
    auto driver = system.registerDriver("Mike Smith", "mike@example.com", 
                                      "+0987654321", "password456", "DL123456");
    
    // Add a vehicle
    auto vehicle = system.addVehicle("ABC123", "Civic", "Honda", 2020, 
                                   VehicleType::SEDAN, 4, 50.0, 12.0, true);
    
    // Assign vehicle to driver
    system.assignVehicleToDriver(vehicle->getVehicleId(), driver->getUserId());
    
    // Add locations
    auto pickup = system.addLocation("123 Main St", 40.7128, -74.0060, 
                                   "New York", "NY", "USA", "10001");
    auto drop = system.addLocation("456 Park Ave", 40.7589, -73.9851, 
                                 "New York", "NY", "USA", "10022");
    
    // Create a booking
    auto booking = system.createBooking(customer->getUserId(), 
                                      pickup->getLocationId(), 
                                      drop->getLocationId(), "SEDAN");
    
    // Find and assign nearest driver
    auto nearestDriver = system.findNearestDriver(pickup->getLocationId(), "SEDAN");
    if (nearestDriver) {
        system.assignDriverToBooking(booking->getBookingId(), nearestDriver->getUserId());
        
        // Start the trip
        auto trip = system.startTrip(booking->getBookingId());
        
        // Complete the trip
        auto completedTrip = system.completeTrip(trip->getTripId(), 25.50);
        
        // Process payment
        auto payment = system.processPayment(trip->getTripId(), PaymentMethod::CREDIT_CARD);
        
        // Rate the trip
        system.rateTrip(trip->getTripId(), 5.0, "Great service!", 5.0, "Good customer");
    }
    
    return 0;
}
```

## 💰 Pricing Model

### Base Rates (USD)
- **Sedan**: $50 base + $12/km
- **SUV**: $80 base + $18/km
- **Hatchback**: $40 base + $10/km
- **Bike**: $20 base + $6/km
- **Auto**: $30 base + $8/km
- **Premium**: $120 base + $25/km

### Dynamic Pricing Factors
- **Peak Hours** (7-9 AM, 5-7 PM): 1.5x multiplier
- **Night Hours** (10 PM - 6 AM): 1.3x multiplier
- **Weather Conditions**: Configurable multiplier
- **Demand Surge**: Real-time demand-based pricing
- **Distance Multiplier**: Long-distance adjustments

### Additional Charges
- **Platform Fee**: 15% of base fare
- **Waiting Charge**: $1 per minute
- **Cancellation Fee**: $50

## 🔔 Notification System

### Customer Notifications
- Booking confirmation
- Driver assignment
- Trip start/end
- Payment confirmation
- Booking cancellation

### Driver Notifications
- New booking requests
- Booking acceptance
- Customer cancellations
- Payment processing

### Notification Channels
- **SMS**: Critical updates and confirmations
- **Email**: Detailed receipts and summaries
- **Push**: Real-time updates
- **In-App**: Status updates and ratings

## 📊 System Statistics

The system provides comprehensive analytics:
- Total customers, drivers, and vehicles
- Trip completion rates
- Revenue tracking
- Driver earnings
- Customer satisfaction metrics

## 🔧 Configuration

### Fare Calculator Settings
```cpp
auto fareCalc = system.getFareCalculator();
fareCalc->setPeakHourMultiplier(1.5);
fareCalc->setNightMultiplier(1.3);
fareCalc->setPlatformFee(0.15);
fareCalc->setWaitingCharge(1.0);
```

### Notification Settings
```cpp
auto notifService = system.getNotificationService();
notifService->setSMSEnabled(true);
notifService->setEmailEnabled(true);
notifService->setPushEnabled(true);
```

## 🧪 Testing

The system is designed to be easily testable with:
- Clear separation of concerns
- Dependency injection
- Mockable interfaces
- Comprehensive error handling

## 🔒 Security Features

- Password authentication
- User session management
- Secure payment processing
- Data validation and sanitization
- Access control mechanisms

## 🚀 Future Enhancements

### Planned Features
- **Real-time GPS Integration**: Live location tracking
- **Machine Learning**: Predictive pricing and demand forecasting
- **Multi-language Support**: Internationalization
- **Advanced Analytics**: Business intelligence dashboard
- **API Integration**: Third-party service integrations
- **Mobile App Backend**: RESTful API endpoints

### Scalability Considerations
- **Database Integration**: Persistent storage
- **Microservices Architecture**: Service decomposition
- **Load Balancing**: High availability setup
- **Caching**: Performance optimization
- **Message Queues**: Asynchronous processing

## 📝 License

This project is part of the System Design Library and is available for educational and development purposes.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## 📞 Support

For questions or support, please refer to the main project documentation or create an issue in the repository.

---

**Note**: This is a simulation system designed for educational purposes. For production use, additional security, scalability, and compliance features would be required. 