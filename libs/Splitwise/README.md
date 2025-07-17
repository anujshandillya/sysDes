# Splitwise System

A comprehensive C++ implementation of a Splitwise-like expense sharing system designed for managing group expenses, debt tracking, and settlement management. This system provides complete expense management, user tracking, group organization, and debt calculation capabilities.

## 🏗️ System Architecture

The Splitwise System is built using object-oriented design principles with the following core components:

### Core Classes

1. **`User`** - Manages user information, preferences, and group memberships
2. **`Group`** - Handles expense groups with member management and balance tracking
3. **`Expense`** - Manages individual expenses with flexible splitting options
4. **`Settlement`** - Tracks debt settlements between users
5. **`SplitwiseApp`** - Main orchestrator class that manages all operations

## 📁 Project Structure

```
libs/Splitwise/
├── include/
│   ├── User.hpp              # User class definition
│   ├── Group.hpp             # Group class definition
│   ├── Expense.hpp           # Expense class definition
│   ├── Settlement.hpp        # Settlement class definition
│   └── SplitwiseApp.hpp      # Main application class definition
├── src/
│   ├── User.cpp              # User class implementation
│   ├── Group.cpp             # Group class implementation
│   ├── Expense.cpp           # Expense class implementation
│   ├── Settlement.cpp        # Settlement class implementation
│   └── SplitwiseApp.cpp      # Main application class implementation
└── README.md                 # This file
```

## 🚀 Features

### User Management
- **User Registration**: Complete user profile management
- **Profile Customization**: Name, email, phone, profile picture, preferred currency
- **Status Management**: Active, Inactive, Suspended user states
- **Group Memberships**: Track all groups a user belongs to
- **Friend System**: Manage user friendships and connections
- **Balance Tracking**: Individual and group-wise balance management

### Group Management
- **Group Types**: Trip, House, Couple, Event, Project, Other
- **Member Management**: Add/remove members with automatic balance tracking
- **Group Settings**: Currency, description, creation date tracking
- **Balance Calculation**: Real-time member balance updates
- **Group Analytics**: Member count, expense count, total spending

### Expense Management
- **Expense Categories**: Food, Transport, Accommodation, Entertainment, Shopping, Utilities, Health, Education, Travel, Other
- **Payment Methods**: Cash, Card, Bank Transfer, Digital Wallet, Check, Other
- **Flexible Splitting**: Equal, Percentage, Shares, Custom splits
- **Receipt Management**: Image upload and storage
- **Notes and Descriptions**: Detailed expense tracking
- **Date Tracking**: Creation and modification timestamps

### Split Calculation Methods

1. **Equal Split**: Divide expense equally among all participants
2. **Percentage Split**: Split based on custom percentages
3. **Shares Split**: Split based on share counts (e.g., 2:1:1 ratio)
4. **Custom Split**: Manual amount assignment per person

### Settlement Management
- **Debt Tracking**: Automatic debt calculation and summary
- **Settlement Creation**: Create settlements between users
- **Status Management**: Pending, Completed, Cancelled, Expired
- **Payment Methods**: Multiple settlement options
- **Transaction History**: Complete settlement audit trail

### Balance and Debt Management
- **Real-time Balances**: Automatic balance calculation
- **Debt Summary**: Who owes what to whom
- **Simplified Debts**: Optimized debt resolution suggestions
- **Cross-group Balances**: Total user balance across all groups
- **Settlement Integration**: Balance updates after settlements

### Reporting and Analytics
- **Group Summaries**: Total expenses, settlements, net balance
- **Top Expenses**: Highest spending items in a group
- **Expense Breakdown**: Category-wise spending analysis
- **Top Spenders**: Users with highest total spending
- **Debt Reports**: Comprehensive debt analysis

## 🛠️ Implementation Details

### Design Patterns Used

1. **Strategy Pattern**: Different split calculation methods
2. **Observer Pattern**: Balance updates when expenses/settlements change
3. **Factory Pattern**: ID generation for all entities
4. **Singleton-like**: Central SplitwiseApp class managing all operations

### Data Structures

- **Unordered Maps**: For O(1) lookups of users, groups, expenses, and settlements
- **Vectors**: For member lists, expense lists, and split details
- **Smart Pointers**: Memory management using `std::shared_ptr`
- **Structs**: For complex data like SplitDetail and DebtSummary

### Key Algorithms

- **Balance Calculation**: Expense-based balance computation
- **Debt Simplification**: Optimal debt resolution algorithm
- **Split Validation**: Ensures total split amount matches expense amount
- **Search Algorithm**: Case-insensitive user search

## 📋 Usage Examples

### Basic Splitwise Operations

```cpp
#include "SplitwiseApp.hpp"
#include <memory>

int main() {
    // Create Splitwise app instance
    auto splitwise = std::make_shared<SplitwiseApp>("MySplitwise", "1.0.0", "USD");
    
    // Create users
    auto alice = splitwise->createUser("Alice Johnson", "alice@example.com", "555-0001");
    auto bob = splitwise->createUser("Bob Smith", "bob@example.com", "555-0002");
    auto charlie = splitwise->createUser("Charlie Brown", "charlie@example.com", "555-0003");
    
    // Create a group
    auto tripGroup = splitwise->createGroup("Summer Trip", alice->getUserId(), 
                                           GroupType::TRIP, "Vacation to Hawaii");
    
    // Add members to group
    splitwise->addUserToGroup(bob->getUserId(), tripGroup->getGroupId());
    splitwise->addUserToGroup(charlie->getUserId(), tripGroup->getGroupId());
    
    // Create an expense
    auto dinnerExpense = splitwise->createExpense("Dinner at Restaurant", 150.0, 
                                                 alice->getUserId(), tripGroup->getGroupId(),
                                                 ExpenseCategory::FOOD);
    
    // Split expense equally
    std::vector<std::string> participants = {alice->getUserId(), bob->getUserId(), charlie->getUserId()};
    dinnerExpense->splitEqually(participants);
    
    // Calculate balances
    splitwise->calculateBalances(tripGroup->getGroupId());
    
    // Get debt summary
    auto debts = splitwise->getDebtSummary(tripGroup->getGroupId());
    
    return 0;
}
```

### Advanced Operations

```cpp
// Create expense with custom split
auto hotelExpense = splitwise->createExpense("Hotel Booking", 300.0, 
                                            bob->getUserId(), tripGroup->getGroupId(),
                                            ExpenseCategory::ACCOMMODATION);

// Custom split: Alice and Bob share room, Charlie has separate room
std::vector<SplitDetail> customSplits = {
    SplitDetail(alice->getUserId(), 100.0, 33.33, 1, false),
    SplitDetail(bob->getUserId(), 100.0, 33.33, 1, false),
    SplitDetail(charlie->getUserId(), 100.0, 33.33, 1, false)
};
hotelExpense->splitCustom(customSplits);

// Create settlement
auto settlement = splitwise->createSettlement(charlie->getUserId(), alice->getUserId(),
                                             tripGroup->getGroupId(), 50.0,
                                             SettlementMethod::BANK_TRANSFER);

// Complete settlement
splitwise->completeSettlement(settlement->getSettlementId());

// Get group summary
auto summary = splitwise->getGroupSummary(tripGroup->getGroupId());

// Get expense breakdown by category
auto breakdown = splitwise->getExpenseBreakdown(tripGroup->getGroupId());

// Get top spenders
auto topSpenders = splitwise->getTopSpenders(tripGroup->getGroupId(), 3);
```

### Different Split Methods

```cpp
// Equal split
expense->splitEqually({"U1", "U2", "U3", "U4"});

// Percentage split
std::vector<std::string> users = {"U1", "U2", "U3"};
std::vector<double> percentages = {50.0, 30.0, 20.0};
expense->splitByPercentage(users, percentages);

// Shares split
std::vector<std::string> users = {"U1", "U2", "U3"};
std::vector<int> shares = {3, 2, 1}; // 3:2:1 ratio
expense->splitByShares(users, shares);

// Custom split
std::vector<SplitDetail> customSplits = {
    SplitDetail("U1", 25.0, 25.0, 1, false),
    SplitDetail("U2", 50.0, 50.0, 2, false),
    SplitDetail("U3", 25.0, 25.0, 1, false)
};
expense->splitCustom(customSplits);
```

## 🔧 Configuration

The Splitwise System supports various configuration options:

```cpp
// Create app with custom settings
auto splitwise = std::make_shared<SplitwiseApp>("MyApp", "2.0.0", "EUR");

// Set user preferences
alice->setPreferredCurrency(Currency::EUR);
alice->setStatus(UserStatus::ACTIVE);

// Configure group settings
tripGroup->setCurrency("USD");
tripGroup->setType(GroupType::TRIP);
```

## 📊 Group Types and Use Cases

| Group Type | Description | Typical Use Cases |
|------------|-------------|-------------------|
| Trip       | Travel expenses | Vacations, business trips, weekend getaways |
| House      | Household expenses | Roommates, family, shared accommodation |
| Couple     | Couple expenses | Dating, married couples, partners |
| Event      | Event expenses | Parties, celebrations, conferences |
| Project    | Project expenses | Work projects, collaborative activities |
| Other      | General expenses | Any other expense sharing scenario |

## 💰 Expense Categories

| Category | Description | Examples |
|----------|-------------|----------|
| Food | Food and dining expenses | Restaurants, groceries, takeout |
| Transport | Transportation costs | Gas, public transport, rideshare |
| Accommodation | Lodging expenses | Hotels, Airbnb, rent |
| Entertainment | Entertainment costs | Movies, concerts, activities |
| Shopping | Shopping expenses | Clothes, electronics, gifts |
| Utilities | Utility bills | Electricity, water, internet |
| Health | Healthcare expenses | Medical bills, insurance |
| Education | Educational expenses | Courses, books, tuition |
| Travel | Travel-related costs | Flights, visas, travel insurance |
| Other | Miscellaneous expenses | Any other category |

## 🔐 User Status Management

| Status | Description | Permissions |
|--------|-------------|-------------|
| Active | Normal user status | Full access to all features |
| Inactive | Temporarily disabled | Read-only access |
| Suspended | Banned user | No access to system |

## 🧪 Testing

To test the Splitwise System:

1. **Compile the code** using your preferred C++ compiler
2. **Create test instances** of all classes
3. **Perform operations** like creating users, groups, expenses, and settlements
4. **Verify balance calculations** and debt summaries
5. **Test different split methods** and edge cases
6. **Validate error conditions** like invalid operations

### Sample Test Scenario

```cpp
// Test scenario: Roommate expense sharing
auto splitwise = std::make_shared<SplitwiseApp>();

// Create roommates
auto john = splitwise->createUser("John Doe", "john@example.com");
auto jane = splitwise->createUser("Jane Smith", "jane@example.com");
auto mike = splitwise->createUser("Mike Johnson", "mike@example.com");

// Create house group
auto houseGroup = splitwise->createGroup("Apartment 3B", john->getUserId(), GroupType::HOUSE);
splitwise->addUserToGroup(jane->getUserId(), houseGroup->getGroupId());
splitwise->addUserToGroup(mike->getUserId(), houseGroup->getGroupId());

// Add various expenses
auto rent = splitwise->createExpense("Monthly Rent", 1800.0, john->getUserId(), 
                                    houseGroup->getGroupId(), ExpenseCategory::ACCOMMODATION);
rent->splitEqually({john->getUserId(), jane->getUserId(), mike->getUserId()});

auto groceries = splitwise->createExpense("Weekly Groceries", 120.0, jane->getUserId(),
                                         houseGroup->getGroupId(), ExpenseCategory::FOOD);
groceries->splitEqually({john->getUserId(), jane->getUserId(), mike->getUserId()});

// Calculate and display balances
splitwise->calculateBalances(houseGroup->getGroupId());
auto debts = splitwise->getDebtSummary(houseGroup->getGroupId());

// Create settlements
for (const auto& debt : debts) {
    auto settlement = splitwise->createSettlement(debt.fromUserId, debt.toUserId,
                                                 houseGroup->getGroupId(), debt.amount);
    splitwise->completeSettlement(settlement->getSettlementId());
}
```

## 🔮 Future Enhancements

Potential improvements for the Splitwise System:

1. **Database Integration**: Persistent storage using SQL/NoSQL databases
2. **Web Interface**: REST API and web-based user interface
3. **Mobile App**: Native mobile applications for iOS and Android
4. **Real-time Notifications**: Push notifications for expense updates
5. **Currency Conversion**: Automatic currency conversion for international groups
6. **Receipt OCR**: Automatic expense extraction from receipt images
7. **Budget Tracking**: Budget limits and spending alerts
8. **Recurring Expenses**: Automatic recurring expense creation
9. **Export Features**: PDF/Excel export of expense reports
10. **Integration APIs**: Connect with banking and payment apps
11. **Advanced Analytics**: Machine learning for spending insights
12. **Multi-language Support**: Internationalization for global users

## 📝 Requirements

- **C++17** or later
- **Standard Library**: `<string>`, `<vector>`, `<unordered_map>`, `<memory>`, `<chrono>`, `<algorithm>`
- **Compiler**: GCC 7+, Clang 5+, or MSVC 2017+

## 🤝 Contributing

To contribute to this project:

1. Fork the repository
2. Create a feature branch
3. Implement your changes
4. Add appropriate tests
5. Submit a pull request

## 📄 License

This project is open source and available under the MIT License.

## 👥 Authors

- **System Design Implementation**: Splitwise System Team

## 🎯 Key Features Summary

- ✅ **User Management**: Complete user profiles and preferences
- ✅ **Group Management**: Flexible group creation and member management
- ✅ **Expense Tracking**: Comprehensive expense management with categories
- ✅ **Flexible Splitting**: Multiple split calculation methods
- ✅ **Balance Calculation**: Real-time balance and debt tracking
- ✅ **Settlement Management**: Complete settlement workflow
- ✅ **Reporting**: Detailed analytics and reporting features
- ✅ **Error Handling**: Robust error handling and validation
- ✅ **Extensible Design**: Easy to extend with new features 