# ParkingLot System Design

A comprehensive C++ implementation of a parking lot management system designed for system design interview preparation. This project demonstrates various design patterns, object-oriented principles, and real-world system considerations.

## 🏗️ System Architecture

### Class Hierarchy

```
Vehicle (Abstract Base)
├── Car
├── Bike
└── Truck

ParkingSpot (Abstract Base)
├── CarSpot
├── BikeSpot
└── TruckSpot

ParkingFloor
├── Manages multiple ParkingSpots
└── Handles vehicle assignment

ParkingLot (Main Controller)
├── Manages multiple ParkingFloors
├── Handles ParkingTickets
└── Integrates PaymentSystem

PaymentSystem
├── Payment
└── PricingStrategy (Strategy Pattern)
    ├── StandardPricing
    └── PremiumPricing
```

## 📁 Project Structure

```
libs/ParkingLot/
├── include/                    # Header files
│   ├── Vehicle.hpp            # Base vehicle class
│   ├── Car.hpp               # Car implementation
│   ├── Bike.hpp              # Bike implementation
│   ├── Truck.hpp             # Truck implementation
│   ├── ParkingSpot.hpp       # Base parking spot class
│   ├── CarSpot.hpp           # Car spot implementation
│   ├── BikeSpot.hpp          # Bike spot implementation
│   ├── TruckSpot.hpp         # Truck spot implementation
│   ├── ParkingFloor.hpp      # Floor management
│   ├── ParkingTicket.hpp     # Ticket system
│   ├── Payment.hpp           # Payment processing
│   ├── PricingStrategy.hpp   # Pricing strategies
│   └── ParkingLot.hpp        # Main controller
├── src/                      # Implementation files
│   ├── Vehicle.cpp
│   ├── Car.cpp
│   ├── Bike.cpp
│   ├── Truck.cpp
│   ├── ParkingSpot.cpp
│   ├── CarSpot.cpp
│   ├── BikeSpot.cpp
│   ├── TruckSpot.cpp
│   ├── ParkingFloor.cpp
│   ├── ParkingTicket.cpp
│   ├── Payment.cpp
│   ├── PricingStrategy.cpp
│   └── ParkingLot.cpp
└── CMakeLists.txt           # Build configuration

src/
└── main.cpp                 # Demo application
```

## 🎯 Key Features

### 1. **Vehicle Management**
- **Vehicle Types**: Car, Bike, Truck
- **Spot Requirements**: Different vehicles require different numbers of spots
- **License Tracking**: Unique identification for each vehicle

### 2. **Parking Spot System**
- **Spot Types**: Dedicated spots for different vehicle types
- **Occupancy Tracking**: Real-time availability status
- **Vehicle Compatibility**: Spots can only accommodate compatible vehicles

### 3. **Multi-Floor Support**
- **Scalable Architecture**: Support for multiple parking floors
- **Floor Management**: Each floor manages its own spots independently
- **Load Distribution**: Automatic spot allocation across floors

### 4. **Ticket System**
- **Unique Ticket IDs**: Auto-generated ticket identifiers
- **Time Tracking**: Entry and exit time recording
- **Duration Calculation**: Automatic fee calculation based on duration

### 5. **Payment Processing**
- **Multiple Payment Methods**: Cash, Credit Card, Debit Card, Digital Wallet
- **Flexible Pricing**: Strategy pattern for different pricing models
- **Payment Validation**: Transaction processing and confirmation

### 6. **Thread Safety**
- **Concurrent Access**: Mutex-protected operations
- **Race Condition Prevention**: Safe multi-threaded parking operations
- **Data Consistency**: Thread-safe state management

## 🏛️ Design Patterns Used

### 1. **Strategy Pattern**
```cpp
// Pricing strategies
class PricingStrategy {
    virtual double calculatePrice(const Vehicle& vehicle, double hours) const = 0;
};

class StandardPricing : public PricingStrategy { /* ... */ };
class PremiumPricing : public PricingStrategy { /* ... */ };
```

### 2. **Factory Pattern**
```cpp
// Vehicle creation
auto car = std::make_shared<Car>("ABC123");
auto bike = std::make_shared<Bike>("XYZ789");
auto truck = std::make_shared<Truck>("DEF456");
```

### 3. **Observer Pattern** (Ready for Implementation)
- Notification system for spot availability
- Real-time updates for parking status

### 4. **Singleton Pattern** (Ready for Implementation)
- Global parking lot instance management
- Centralized configuration

## 💡 Usage Examples

### Basic Parking Operations

```cpp
// Create parking lot
auto parkingLot = std::make_unique<ParkingLot>("Central Parking", 2, 10, 5.0);

// Park a vehicle
auto car = std::make_shared<Car>("ABC123");
auto ticket = parkingLot->parkVehicle(car);

if (ticket) {
    std::cout << "Parked! Ticket: " << ticket->getTicketId() << std::endl;
    
    // Calculate fee
    double fee = parkingLot->calculateFee(ticket->getTicketId());
    
    // Process payment
    auto payment = parkingLot->processPayment(ticket->getTicketId(), 
                                             PaymentMethod::CREDIT_CARD);
    
    // Exit vehicle
    parkingLot->exitVehicle("ABC123");
}
```

### Pricing Strategy Usage

```cpp
// Set different pricing strategies
parkingLot->setPricingStrategy(std::make_unique<StandardPricing>());
// or
parkingLot->setPricingStrategy(std::make_unique<PremiumPricing>());
```

### Status Queries

```cpp
// Check availability
int available = parkingLot->getAvailableSpots();
int total = parkingLot->getTotalSpots();

// Check if vehicle is parked
bool isParked = parkingLot->isVehicleParked("ABC123");
```

## 🔧 System Design Interview Points

### 1. **Scalability Considerations**
- **Horizontal Scaling**: Multiple floors and parking lots
- **Vertical Scaling**: Efficient spot allocation algorithms
- **Load Balancing**: Distributed parking across floors

### 2. **Concurrency & Thread Safety**
- **Mutex Protection**: Thread-safe operations
- **Atomic Operations**: Safe state transitions
- **Deadlock Prevention**: Proper locking order

### 3. **Fault Tolerance**
- **Error Handling**: Graceful failure management
- **Data Recovery**: Transaction rollback capabilities
- **System Resilience**: Continuous operation during failures

### 4. **Monitoring & Observability**
- **Logging**: Comprehensive operation logging
- **Metrics**: Performance and usage statistics
- **Health Checks**: System status monitoring

### 5. **Extensibility**
- **Plugin Architecture**: Easy addition of new vehicle types
- **API Design**: Clean interfaces for integration
- **Configuration Management**: Flexible system configuration

## 🚀 Future Enhancements

### 1. **Reservation System**
- Pre-booking parking spots
- Time-based reservations
- VIP parking allocation

### 2. **Real-time Notifications**
- Spot availability alerts
- Payment confirmations
- System status updates

### 3. **Analytics & Reporting**
- Usage statistics
- Revenue reports
- Peak hour analysis

### 4. **Integration Capabilities**
- Payment gateway integration
- Mobile app support
- IoT sensor integration

### 5. **Advanced Features**
- Electric vehicle charging stations
- Automated parking systems
- Multi-location management

## 📊 Performance Characteristics

### Time Complexity
- **Parking**: O(n) where n is total number of spots
- **Exit**: O(1) with hash map lookup
- **Fee Calculation**: O(1) constant time

### Space Complexity
- **Storage**: O(n) for n parking spots
- **Memory**: Efficient object pooling
- **Cache**: LRU cache for frequent operations

## 🛡️ Security Considerations

### 1. **Data Protection**
- Encrypted payment information
- Secure ticket generation
- Privacy-compliant data handling

### 2. **Access Control**
- Role-based permissions
- Authentication mechanisms
- Authorization policies

### 3. **Audit Trail**
- Complete operation logging
- Transaction history
- Compliance reporting

## 📝 Contributing

This project serves as a learning resource for system design interviews. Feel free to:

1. **Extend functionality** with new features
2. **Improve performance** with optimizations
3. **Add test cases** for better coverage
4. **Enhance documentation** for clarity

## 📚 Learning Resources

### System Design Concepts Demonstrated
- **Object-Oriented Design**: Inheritance, polymorphism, encapsulation
- **Design Patterns**: Strategy, Factory, Observer patterns
- **Concurrency**: Thread safety and synchronization
- **Data Structures**: Hash maps, vectors, smart pointers
- **Error Handling**: Exception safety and recovery

### Interview Preparation Topics
- **Scalability**: Horizontal vs vertical scaling
- **Performance**: Time and space complexity analysis
- **Reliability**: Fault tolerance and error handling
- **Maintainability**: Clean code and documentation
- **Extensibility**: Future-proof architecture design

---

**Note**: This implementation focuses on demonstrating system design principles and is suitable for educational purposes and interview preparation. For production use, additional considerations like database persistence, network communication, and security hardening would be required. 