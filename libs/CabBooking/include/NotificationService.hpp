#pragma once

#include <string>
#include <vector>
#include <memory>

enum class NotificationType {
    SMS,
    EMAIL,
    PUSH,
    IN_APP
};

enum class NotificationPriority {
    LOW,
    NORMAL,
    HIGH,
    URGENT
};

class Notification {
private:
    std::string notificationId;
    std::string recipientId;
    std::string title;
    std::string message;
    NotificationType type;
    NotificationPriority priority;
    std::string status; // "PENDING", "SENT", "FAILED"
    std::chrono::system_clock::time_point timestamp;
    std::string metadata; // JSON string for additional data

public:
    Notification(const std::string& id, const std::string& recipientId,
                 const std::string& title, const std::string& message,
                 NotificationType type, NotificationPriority priority = NotificationPriority::NORMAL);
    
    // Getters
    std::string getNotificationId() const;
    std::string getRecipientId() const;
    std::string getTitle() const;
    std::string getMessage() const;
    NotificationType getType() const;
    NotificationPriority getPriority() const;
    std::string getStatus() const;
    std::chrono::system_clock::time_point getTimestamp() const;
    std::string getMetadata() const;
    
    // Setters
    void setStatus(const std::string& status);
    void setMetadata(const std::string& metadata);
    
    // Utility methods
    std::string getTypeString() const;
    std::string getPriorityString() const;
    bool isSent() const;
    bool isFailed() const;
};

class NotificationService {
private:
    std::vector<std::shared_ptr<Notification>> notificationQueue;
    bool smsEnabled;
    bool emailEnabled;
    bool pushEnabled;
    std::string smsProvider;
    std::string emailProvider;

public:
    NotificationService();
    
    // Core notification methods
    void sendNotification(std::shared_ptr<Notification> notification);
    void sendSMS(const std::string& phoneNumber, const std::string& message);
    void sendEmail(const std::string& email, const std::string& subject, const std::string& body);
    void sendPushNotification(const std::string& userId, const std::string& title, const std::string& message);
    
    // Trip-related notifications
    void notifyBookingConfirmation(const std::string& customerId, const std::string& driverId, 
                                  const std::string& vehicleDetails, int estimatedWaitTime);
    void notifyDriverAssigned(const std::string& customerId, const std::string& driverName, 
                             const std::string& vehicleDetails, int estimatedArrival);
    void notifyTripStarted(const std::string& customerId, const std::string& driverName);
    void notifyTripCompleted(const std::string& customerId, const std::string& driverId, 
                            double fare, const std::string& paymentMethod);
    void notifyPaymentSuccess(const std::string& customerId, const std::string& driverId, 
                             double amount, const std::string& transactionId);
    void notifyBookingCancelled(const std::string& customerId, const std::string& reason);
    
    // Driver notifications
    void notifyNewBooking(const std::string& driverId, const std::string& pickupLocation, 
                         const std::string& dropLocation, double estimatedFare);
    void notifyBookingAccepted(const std::string& driverId, const std::string& customerName);
    void notifyBookingCancelledByCustomer(const std::string& driverId);
    
    // Configuration methods
    void setSMSEnabled(bool enabled);
    void setEmailEnabled(bool enabled);
    void setPushEnabled(bool enabled);
    void setSMSProvider(const std::string& provider);
    void setEmailProvider(const std::string& provider);
    
    // Utility methods
    std::vector<std::shared_ptr<Notification>> getNotificationQueue() const;
    void clearQueue();
    bool isSMSEnabled() const;
    bool isEmailEnabled() const;
    bool isPushEnabled() const;
    std::string getSMSProvider() const;
    std::string getEmailProvider() const;
}; 