# Elevator System Design

A comprehensive C++ implementation of an elevator control system with multiple scheduling algorithms, real-time simulation, and detailed analytics.

## 🏗️ System Architecture

The elevator system is designed using object-oriented principles with the following core components:

### Core Classes

#### 1. **Building** (`Building.hpp/cpp`)
- **Purpose**: Central controller that manages the entire elevator system
- **Responsibilities**:
  - Manages multiple elevators and floors
  - Coordinates request handling and scheduling
  - Provides simulation capabilities
  - Generates system statistics and reports
  - Handles emergency situations and maintenance

#### 2. **Elevator** (`Elevator.hpp/cpp`)
- **Purpose**: Represents individual elevator units
- **States**: IDLE, MOVING_UP, MOVING_DOWN, DOOR_OPENING, DOOR_OPEN, DOOR_CLOSING, MAINTENANCE, EMERGENCY
- **Responsibilities**:
  - Manages elevator movement and door operations
  - Handles internal floor button requests
  - Tracks passenger capacity and load
  - Maintains request queue and completion history

#### 3. **Floor** (`Floor.hpp/cpp`)
- **Purpose**: Represents building floors with call buttons
- **Responsibilities**:
  - Manages UP/DOWN call buttons
  - Tracks request states and directions
  - Provides floor-specific information

#### 4. **Request** (`Request.hpp/cpp`)
- **Purpose**: Represents elevator service requests
- **Types**: EXTERNAL (floor button), INTERNAL (elevator button)
- **Status**: PENDING, ASSIGNED, COMPLETED, CANCELLED
- **Responsibilities**:
  - Tracks request timing and completion
  - Manages request priority and assignment
  - Provides wait time and travel time calculations

#### 5. **Scheduler** (`Scheduler.hpp/cpp`)
- **Purpose**: Implements various elevator scheduling algorithms
- **Algorithms**:
  - **FCFS** (First Come First Serve): Simple queue-based scheduling
  - **SCAN** (Elevator Algorithm): Services requests in one direction until end, then reverses
  - **LOOK** (Improved SCAN): Similar to SCAN but changes direction when no more requests in current direction
  - **SSTF** (Shortest Seek Time First): Prioritizes nearest requests
  - **PRIORITY**: Priority-based scheduling for special cases

## 🔄 System Flow

### Request Processing Flow
1. **Request Creation**: External requests (floor buttons) or internal requests (elevator buttons)
2. **Request Submission**: Requests are submitted to the building controller
3. **Scheduling**: The scheduler assigns requests to appropriate elevators based on the selected algorithm
4. **Execution**: Elevators process assigned requests in their queue
5. **Completion**: Requests are marked as completed with timing information

### Simulation Flow
1. **Initialization**: Building, floors, and elevators are created and configured
2. **Request Generation**: External and internal requests are created
3. **Step-by-step Processing**: Each simulation step processes elevator movements and request assignments
4. **Statistics Collection**: Performance metrics are continuously updated
5. **Reporting**: Final statistics and analytics are generated

## 🚀 Key Features

### 1. **Multiple Scheduling Algorithms**
- **FCFS**: Simple and fair, but may not be optimal for efficiency
- **SCAN**: Good for reducing total travel time
- **LOOK**: Improved version of SCAN with better efficiency
- **SSTF**: Minimizes individual request wait times
- **PRIORITY**: Supports priority-based scheduling for special cases

### 2. **Real-time Simulation**
- Step-by-step simulation with configurable time steps
- Real-time elevator movement and door operations
- Passenger boarding and alighting simulation
- Emergency stop and maintenance mode support

### 3. **Comprehensive Analytics**
- Average wait time and travel time calculations
- System utilization metrics
- Elevator-specific performance tracking
- Most requested floors analysis
- Trip count and efficiency statistics

### 4. **Flexible Configuration**
- Configurable number of floors and basements
- Multiple elevator support with different capacities
- Adjustable elevator speeds and door operation times
- Customizable scheduling algorithms

### 5. **Safety and Maintenance**
- Emergency stop functionality
- Maintenance mode for individual elevators
- System reset capabilities
- Operational status monitoring

## 📊 Performance Metrics

The system tracks various performance indicators:

- **Wait Time**: Time from request creation to elevator arrival
- **Travel Time**: Total time from request to completion
- **System Utilization**: Percentage of time elevators are in use
- **Trip Count**: Number of completed requests per elevator
- **Floor Request Analysis**: Most frequently requested floors

## 🛠️ Usage Example

```cpp
#include "Building.hpp"
#include "Elevator.hpp"
#include "Scheduler.hpp"

int main() {
    // Create a building with 20 floors and 2 basements
    auto building = std::make_shared<Building>("B001", "Office Tower", 20, 2);
    
    // Add elevators
    auto elevator1 = std::make_shared<Elevator>("E001", 20, -2, 1.0, 3.0, 3.0, 8);
    auto elevator2 = std::make_shared<Elevator>("E002", 20, -2, 1.0, 3.0, 3.0, 8);
    building->addElevator(elevator1);
    building->addElevator(elevator2);
    
    // Set scheduling algorithm
    building->setSchedulingAlgorithm(SchedulingAlgorithm::LOOK);
    
    // Create and submit requests
    auto request1 = building->createExternalRequest(1, Direction::UP);
    auto request2 = building->createInternalRequest(1, 15);
    building->submitRequest(request1);
    building->submitRequest(request2);
    
    // Run simulation
    building->startSimulation();
    building->runSimulation(100);
    
    // Print statistics
    building->printStatistics();
    
    return 0;
}
```

## 🔧 Building and Compilation

### Prerequisites
- C++17 or later
- CMake 3.10 or later
- Standard C++ libraries

### Compilation
```bash
mkdir build
cd build
cmake ..
make
```

## 📈 System Design Patterns

### 1. **Observer Pattern**
- Elevators observe floor requests
- Building observes elevator states
- Scheduler observes request queue

### 2. **Strategy Pattern**
- Different scheduling algorithms can be swapped at runtime
- Each algorithm implements the same interface

### 3. **State Pattern**
- Elevators maintain different states (IDLE, MOVING, etc.)
- State transitions are managed internally

### 4. **Factory Pattern**
- Request creation through factory methods
- Elevator and floor instantiation

## 🎯 Design Principles

### 1. **Single Responsibility Principle**
- Each class has a single, well-defined responsibility
- Building manages overall system
- Elevator manages individual unit operations
- Scheduler handles request assignment

### 2. **Open/Closed Principle**
- System is open for extension (new algorithms) but closed for modification
- New scheduling algorithms can be added without changing existing code

### 3. **Dependency Inversion**
- High-level modules (Building) don't depend on low-level modules (Elevator)
- Both depend on abstractions (interfaces)

### 4. **Encapsulation**
- Internal state is protected
- Public interfaces provide controlled access
- Implementation details are hidden

## 🔍 Future Enhancements

### Potential Improvements
1. **Machine Learning Integration**: AI-based scheduling optimization
2. **Predictive Analytics**: Anticipate peak usage patterns
3. **Energy Optimization**: Minimize energy consumption
4. **Multi-building Support**: Coordinate elevators across multiple buildings
5. **Real-time Monitoring**: Web-based dashboard for system monitoring
6. **Load Balancing**: Dynamic elevator allocation based on demand
7. **Priority Queues**: Support for VIP and emergency requests

### Scalability Considerations
- **Horizontal Scaling**: Support for multiple elevator banks
- **Vertical Scaling**: Support for very tall buildings (100+ floors)
- **Performance Optimization**: Efficient algorithms for large-scale systems
- **Memory Management**: Optimized data structures for high-frequency operations

## 📝 License

This project is part of the System Design Library and follows the same licensing terms as the parent project.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

---

*This elevator system design demonstrates advanced software engineering principles while providing a practical solution for elevator control and optimization.* 