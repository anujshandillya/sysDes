#include "../include/NotificationService.hpp"
#include <iostream>

Notification::Notification(const std::string& id, const std::string& recipientId,
                           const std::string& title, const std::string& message,
                           NotificationType type, NotificationPriority priority)
    : notificationId(id), recipientId(recipientId), title(title), message(message),
      type(type), priority(priority), status("PENDING"), timestamp(std::chrono::system_clock::now()) {}

std::string Notification::getNotificationId() const {
    return notificationId;
}

std::string Notification::getRecipientId() const {
    return recipientId;
}

std::string Notification::getTitle() const {
    return title;
}

std::string Notification::getMessage() const {
    return message;
}

NotificationType Notification::getType() const {
    return type;
}

NotificationPriority Notification::getPriority() const {
    return priority;
}

std::string Notification::getStatus() const {
    return status;
}

std::chrono::system_clock::time_point Notification::getTimestamp() const {
    return timestamp;
}

std::string Notification::getMetadata() const {
    return metadata;
}

void Notification::setStatus(const std::string& status) {
    this->status = status;
}

void Notification::setMetadata(const std::string& metadata) {
    this->metadata = metadata;
}

std::string Notification::getTypeString() const {
    switch (type) {
        case NotificationType::SMS: return "SMS";
        case NotificationType::EMAIL: return "EMAIL";
        case NotificationType::PUSH: return "PUSH";
        case NotificationType::IN_APP: return "IN_APP";
        default: return "UNKNOWN";
    }
}

std::string Notification::getPriorityString() const {
    switch (priority) {
        case NotificationPriority::LOW: return "LOW";
        case NotificationPriority::NORMAL: return "NORMAL";
        case NotificationPriority::HIGH: return "HIGH";
        case NotificationPriority::URGENT: return "URGENT";
        default: return "UNKNOWN";
    }
}

bool Notification::isSent() const {
    return status == "SENT";
}

bool Notification::isFailed() const {
    return status == "FAILED";
}

NotificationService::NotificationService() 
    : smsEnabled(true), emailEnabled(true), pushEnabled(true),
      smsProvider("Twilio"), emailProvider("SendGrid") {}

void NotificationService::sendNotification(std::shared_ptr<Notification> notification) {
    notificationQueue.push_back(notification);
    
    // Simulate sending notification based on type
    switch (notification->getType()) {
        case NotificationType::SMS:
            if (smsEnabled) {
                sendSMS(notification->getRecipientId(), notification->getMessage());
                notification->setStatus("SENT");
            } else {
                notification->setStatus("FAILED");
            }
            break;
        case NotificationType::EMAIL:
            if (emailEnabled) {
                sendEmail(notification->getRecipientId(), notification->getTitle(), notification->getMessage());
                notification->setStatus("SENT");
            } else {
                notification->setStatus("FAILED");
            }
            break;
        case NotificationType::PUSH:
            if (pushEnabled) {
                sendPushNotification(notification->getRecipientId(), notification->getTitle(), notification->getMessage());
                notification->setStatus("SENT");
            } else {
                notification->setStatus("FAILED");
            }
            break;
        case NotificationType::IN_APP:
            notification->setStatus("SENT");
            break;
    }
}

void NotificationService::sendSMS(const std::string& phoneNumber, const std::string& message) {
    // Simulate SMS sending
    // In a real system, this would integrate with SMS providers like Twilio
    std::cout << "SMS sent to " << phoneNumber << ": " << message << std::endl;
}

void NotificationService::sendEmail(const std::string& email, const std::string& subject, const std::string& body) {
    // Simulate email sending
    // In a real system, this would integrate with email providers like SendGrid
    std::cout << "Email sent to " << email << " - Subject: " << subject << std::endl;
    std::cout << "Body: " << body << std::endl;
}

void NotificationService::sendPushNotification(const std::string& userId, const std::string& title, const std::string& message) {
    // Simulate push notification
    // In a real system, this would integrate with push notification services
    std::cout << "Push notification sent to user " << userId << " - Title: " << title << std::endl;
    std::cout << "Message: " << message << std::endl;
}

void NotificationService::notifyBookingConfirmation(const std::string& customerId, const std::string& driverId, 
                                                    const std::string& vehicleDetails, int estimatedWaitTime) {
    std::string message = "Your booking has been confirmed! Driver: " + driverId + 
                         ", Vehicle: " + vehicleDetails + ", ETA: " + std::to_string(estimatedWaitTime) + " minutes";
    
    auto notification = std::make_shared<Notification>("notif_" + customerId, customerId, 
                                                      "Booking Confirmed", message, 
                                                      NotificationType::SMS, NotificationPriority::HIGH);
    sendNotification(notification);
}

void NotificationService::notifyDriverAssigned(const std::string& customerId, const std::string& driverName, 
                                               const std::string& vehicleDetails, int estimatedArrival) {
    std::string message = "Your driver " + driverName + " is on the way! Vehicle: " + vehicleDetails + 
                         ", Arrival in " + std::to_string(estimatedArrival) + " minutes";
    
    auto notification = std::make_shared<Notification>("notif_" + customerId, customerId, 
                                                      "Driver Assigned", message, 
                                                      NotificationType::PUSH, NotificationPriority::HIGH);
    sendNotification(notification);
}

void NotificationService::notifyTripStarted(const std::string& customerId, const std::string& driverName) {
    std::string message = "Your trip with " + driverName + " has started. Have a safe journey!";
    
    auto notification = std::make_shared<Notification>("notif_" + customerId, customerId, 
                                                      "Trip Started", message, 
                                                      NotificationType::IN_APP, NotificationPriority::NORMAL);
    sendNotification(notification);
}

void NotificationService::notifyTripCompleted(const std::string& customerId, const std::string& driverId, 
                                              double fare, const std::string& paymentMethod) {
    std::string message = "Trip completed! Fare: $" + std::to_string(fare) + 
                         ", Payment: " + paymentMethod + ". Please rate your experience.";
    
    auto notification = std::make_shared<Notification>("notif_" + customerId, customerId, 
                                                      "Trip Completed", message, 
                                                      NotificationType::PUSH, NotificationPriority::NORMAL);
    sendNotification(notification);
}

void NotificationService::notifyPaymentSuccess(const std::string& customerId, const std::string& driverId, 
                                               double amount, const std::string& transactionId) {
    std::string message = "Payment successful! Amount: $" + std::to_string(amount) + 
                         ", Transaction ID: " + transactionId;
    
    auto notification = std::make_shared<Notification>("notif_" + customerId, customerId, 
                                                      "Payment Successful", message, 
                                                      NotificationType::EMAIL, NotificationPriority::NORMAL);
    sendNotification(notification);
}

void NotificationService::notifyBookingCancelled(const std::string& customerId, const std::string& reason) {
    std::string message = "Your booking has been cancelled. Reason: " + reason;
    
    auto notification = std::make_shared<Notification>("notif_" + customerId, customerId, 
                                                      "Booking Cancelled", message, 
                                                      NotificationType::SMS, NotificationPriority::HIGH);
    sendNotification(notification);
}

void NotificationService::notifyNewBooking(const std::string& driverId, const std::string& pickupLocation, 
                                           const std::string& dropLocation, double estimatedFare) {
    std::string message = "New booking request! Pickup: " + pickupLocation + 
                         ", Drop: " + dropLocation + ", Estimated fare: $" + std::to_string(estimatedFare);
    
    auto notification = std::make_shared<Notification>("notif_" + driverId, driverId, 
                                                      "New Booking", message, 
                                                      NotificationType::PUSH, NotificationPriority::URGENT);
    sendNotification(notification);
}

void NotificationService::notifyBookingAccepted(const std::string& driverId, const std::string& customerName) {
    std::string message = "Booking accepted! Customer: " + customerName + ". Please proceed to pickup location.";
    
    auto notification = std::make_shared<Notification>("notif_" + driverId, driverId, 
                                                      "Booking Accepted", message, 
                                                      NotificationType::SMS, NotificationPriority::HIGH);
    sendNotification(notification);
}

void NotificationService::notifyBookingCancelledByCustomer(const std::string& driverId) {
    std::string message = "Booking cancelled by customer.";
    
    auto notification = std::make_shared<Notification>("notif_" + driverId, driverId, 
                                                      "Booking Cancelled", message, 
                                                      NotificationType::PUSH, NotificationPriority::NORMAL);
    sendNotification(notification);
}

void NotificationService::setSMSEnabled(bool enabled) {
    smsEnabled = enabled;
}

void NotificationService::setEmailEnabled(bool enabled) {
    emailEnabled = enabled;
}

void NotificationService::setPushEnabled(bool enabled) {
    pushEnabled = enabled;
}

void NotificationService::setSMSProvider(const std::string& provider) {
    smsProvider = provider;
}

void NotificationService::setEmailProvider(const std::string& provider) {
    emailProvider = provider;
}

std::vector<std::shared_ptr<Notification>> NotificationService::getNotificationQueue() const {
    return notificationQueue;
}

void NotificationService::clearQueue() {
    notificationQueue.clear();
}

bool NotificationService::isSMSEnabled() const {
    return smsEnabled;
}

bool NotificationService::isEmailEnabled() const {
    return emailEnabled;
}

bool NotificationService::isPushEnabled() const {
    return pushEnabled;
}

std::string NotificationService::getSMSProvider() const {
    return smsProvider;
}

std::string NotificationService::getEmailProvider() const {
    return emailProvider;
} 