# URL Shortener System

A comprehensive C++ URL shortening service with user management, analytics, and custom short codes.

## Overview

The URL Shortener system is a complete solution for creating, managing, and tracking shortened URLs. It provides a robust API for URL shortening, user management, and detailed analytics.

## Features

### Core Functionality
- **URL Shortening**: Convert long URLs to short, manageable links
- **Custom Short Codes**: Allow users to create custom short codes
- **URL Expiration**: Set expiration dates for URLs
- **URL Validation**: Validate and sanitize URLs before processing

### User Management
- **User Registration**: Create user accounts with username, email, and password
- **User Authentication**: Secure login system
- **User Profiles**: Manage user information and created URLs
- **User-specific URLs**: Track URLs created by specific users

### Analytics & Tracking
- **Click Tracking**: Monitor how many times each URL is accessed
- **Unique Visitors**: Track unique visitors by IP address
- **Geographic Data**: Track clicks by country and city
- **Device Analytics**: Monitor clicks by device type
- **Referrer Tracking**: Track where clicks are coming from
- **Time-based Analytics**: Analyze clicks by hour and day

### Advanced Features
- **Thread-safe Operations**: All database operations are thread-safe
- **Automatic Cleanup**: Remove expired URLs and old analytics data
- **Comprehensive Statistics**: Detailed analytics for each URL
- **Top URLs**: Find most popular URLs by click count

## Architecture

### Core Classes

#### 1. URL Class (`URL.hpp/cpp`)
Represents a shortened URL with metadata:
- Original and shortened URLs
- Creation and expiration timestamps
- Click count tracking
- User association
- Title and description

#### 2. User Class (`User.hpp/cpp`)
Manages user accounts and their created URLs:
- User authentication
- Profile management
- URL ownership tracking
- User statistics

#### 3. HashGenerator Class (`HashGenerator.hpp/cpp`)
Generates unique short codes:
- Random code generation
- Hash-based generation from URLs
- Custom code validation
- Collision prevention

#### 4. Database Class (`Database.hpp/cpp`)
Thread-safe in-memory storage:
- URL and user storage
- Multiple indexing strategies
- Thread-safe operations
- Query optimization

#### 5. Analytics Class (`Analytics.hpp/cpp`)
Comprehensive analytics tracking:
- Click event recording
- Geographic and device tracking
- Time-based analysis
- Statistical aggregation

#### 6. URLShortener Class (`URLShortener.hpp/cpp`)
Main service orchestrator:
- High-level API for all operations
- Business logic implementation
- Input validation and sanitization
- Error handling

## API Reference

### URL Shortening

```cpp
// Basic URL shortening
ShortenResponse response = shortener.shortenUrl("https://example.com/very/long/url");

// Advanced shortening with custom code
ShortenRequest request;
request.originalUrl = "https://example.com/very/long/url";
request.customCode = "mycode";
request.title = "My Custom URL";
request.description = "A description of this URL";
request.userId = "user123";
request.expirationDays = 30;

ShortenResponse response = shortener.shortenUrl(request);
```

### URL Expansion

```cpp
// Expand a short URL
std::string originalUrl = shortener.expandUrl("abc123", "192.168.1.1");
```

### User Management

```cpp
// Create a new user
auto user = shortener.createUser("john_doe", "john@example.com", "password123");

// Authenticate user
auto authenticatedUser = shortener.authenticateUser("john@example.com", "password123");

// Update user profile
shortener.updateUser("user123", "new_username", "newemail@example.com");
```

### Analytics

```cpp
// Get URL statistics
UrlStats stats = shortener.getUrlAnalytics("abc123");

// Get top URLs by clicks
auto topUrls = shortener.getTopUrls(10);

// Get clicks by country
auto countryStats = shortener.getClicksByCountry("abc123");

// Get clicks by device
auto deviceStats = shortener.getClicksByDevice("abc123");
```

### URL Management

```cpp
// Update URL metadata
shortener.updateUrl("abc123", "New Title", "New Description", 60);

// Delete URL
shortener.deleteUrl("abc123", "user123");

// Get user's URLs
auto userUrls = shortener.getUserUrls("user123");
```

## Data Structures

### ShortenRequest
```cpp
struct ShortenRequest {
    std::string originalUrl;    // The URL to shorten
    std::string customCode;     // Optional custom short code
    std::string title;          // Optional title for the URL
    std::string description;    // Optional description
    std::string userId;         // Optional user ID
    int expirationDays;         // Optional expiration in days
};
```

### ShortenResponse
```cpp
struct ShortenResponse {
    bool success;               // Whether the operation succeeded
    std::string shortCode;      // The generated short code
    std::string shortUrl;       // The complete short URL
    std::string message;        // Success/error message
    std::shared_ptr<URL> url;   // The created URL object
};
```

### ClickEvent
```cpp
struct ClickEvent {
    std::string shortCode;      // The short code that was clicked
    std::string ipAddress;      // Visitor's IP address
    std::string userAgent;      // Browser user agent
    std::string referrer;       // Referrer URL
    std::chrono::system_clock::time_point timestamp; // Click timestamp
    std::string country;        // Visitor's country
    std::string city;           // Visitor's city
    std::string deviceType;     // Device type (mobile, desktop, etc.)
};
```

### UrlStats
```cpp
struct UrlStats {
    std::string shortCode;      // The short code
    int totalClicks;            // Total number of clicks
    int uniqueClicks;           // Number of unique visitors
    std::chrono::system_clock::time_point firstClick;  // First click time
    std::chrono::system_clock::time_point lastClick;   // Last click time
    std::unordered_map<std::string, int> clicksByCountry;   // Clicks by country
    std::unordered_map<std::string, int> clicksByDevice;    // Clicks by device
    std::unordered_map<std::string, int> clicksByReferrer;  // Clicks by referrer
};
```

## Usage Examples

### Basic Usage

```cpp
#include "URLShortener.hpp"

int main() {
    // Create URL shortener instance
    URLShortener shortener("http://short.url");
    
    // Shorten a URL
    auto response = shortener.shortenUrl("https://www.google.com");
    if (response.success) {
        std::cout << "Short URL: " << response.shortUrl << std::endl;
    }
    
    // Expand a URL
    std::string original = shortener.expandUrl("abc123", "192.168.1.1");
    if (!original.empty()) {
        std::cout << "Original URL: " << original << std::endl;
    }
    
    return 0;
}
```

### Advanced Usage with User Management

```cpp
#include "URLShortener.hpp"

int main() {
    URLShortener shortener("http://short.url");
    
    // Create a user
    auto user = shortener.createUser("john_doe", "john@example.com", "password123");
    if (user) {
        std::cout << "User created: " << user->getUsername() << std::endl;
        
        // Create a URL for this user
        ShortenRequest request;
        request.originalUrl = "https://example.com/very/long/url";
        request.customCode = "mycustom";
        request.title = "My Custom URL";
        request.userId = user->getUserId();
        
        auto response = shortener.shortenUrl(request);
        if (response.success) {
            std::cout << "Custom URL: " << response.shortUrl << std::endl;
        }
        
        // Get user's URLs
        auto userUrls = shortener.getUserUrls(user->getUserId());
        std::cout << "User has " << userUrls.size() << " URLs" << std::endl;
    }
    
    return 0;
}
```

### Analytics Usage

```cpp
#include "URLShortener.hpp"

int main() {
    URLShortener shortener("http://short.url");
    
    // Create and access some URLs
    auto response = shortener.shortenUrl("https://example.com");
    if (response.success) {
        // Simulate some clicks
        shortener.expandUrl(response.shortCode, "192.168.1.1");
        shortener.expandUrl(response.shortCode, "192.168.1.2");
        shortener.expandUrl(response.shortCode, "192.168.1.1"); // Same IP
        
        // Get analytics
        auto stats = shortener.getUrlAnalytics(response.shortCode);
        std::cout << "Total clicks: " << stats.totalClicks << std::endl;
        std::cout << "Unique clicks: " << stats.uniqueClicks << std::endl;
        
        // Get top URLs
        auto topUrls = shortener.getTopUrls(5);
        for (const auto& pair : topUrls) {
            std::cout << pair.first << ": " << pair.second << " clicks" << std::endl;
        }
    }
    
    return 0;
}
```

## System Requirements

- **C++17** or later
- **Standard Library**: Uses only standard C++ libraries
- **Threading**: Thread-safe operations using std::mutex
- **Memory**: Smart pointer management for automatic memory cleanup

## Design Patterns

### Singleton Pattern
The URLShortener class acts as a facade for the entire system, providing a single point of access.

### Factory Pattern
The HashGenerator creates different types of short codes based on requirements.

### Observer Pattern
The Analytics system observes URL clicks and updates statistics accordingly.

### Strategy Pattern
Different URL validation and generation strategies can be implemented.

## Performance Considerations

- **In-Memory Storage**: Fast access but limited by available RAM
- **Thread Safety**: All operations are thread-safe using mutexes
- **Efficient Lookups**: Multiple hash maps for fast retrieval
- **Memory Management**: Smart pointers for automatic cleanup

## Security Features

- **Input Validation**: All inputs are validated before processing
- **URL Sanitization**: URLs are sanitized to prevent injection attacks
- **Password Hashing**: Passwords are hashed (basic implementation)
- **Access Control**: Users can only modify their own URLs

## Future Enhancements

- **Persistent Storage**: Database integration for data persistence
- **Rate Limiting**: Prevent abuse with rate limiting
- **Advanced Analytics**: More detailed analytics and reporting
- **API Authentication**: JWT or OAuth for API access
- **URL Categories**: Organize URLs by categories
- **Bulk Operations**: Support for bulk URL shortening
- **QR Code Generation**: Generate QR codes for short URLs
- **Social Media Integration**: Share shortened URLs on social platforms

## Contributing

This system is designed to be extensible. Key areas for contribution:
- Database persistence layer
- Advanced analytics features
- Web API interface
- Performance optimizations
- Security enhancements

## License

This project is part of the System Design examples collection. 