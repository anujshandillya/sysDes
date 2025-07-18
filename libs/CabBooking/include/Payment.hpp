#pragma once

#include <string>
#include <chrono>

enum class PaymentMethod {
    CASH,
    CREDIT_CARD,
    DEBIT_CARD,
    UPI,
    WALLET,
    NET_BANKING
};

enum class PaymentStatus {
    PENDING,
    PROCESSING,
    COMPLETED,
    FAILED,
    REFUNDED
};

class Payment {
private:
    std::string paymentId;
    std::string tripId;
    std::string customerId;
    std::string driverId;
    double amount;
    PaymentMethod method;
    PaymentStatus status;
    std::chrono::system_clock::time_point timestamp;
    std::string transactionId;
    std::string failureReason;
    double platformFee;
    double driverAmount;
    std::string currency;

public:
    Payment(const std::string& id, const std::string& tripId, const std::string& customerId,
            const std::string& driverId, double amount, PaymentMethod method);
    
    // Getters
    std::string getPaymentId() const;
    std::string getTripId() const;
    std::string getCustomerId() const;
    std::string getDriverId() const;
    double getAmount() const;
    PaymentMethod getMethod() const;
    PaymentStatus getStatus() const;
    std::chrono::system_clock::time_point getTimestamp() const;
    std::string getTransactionId() const;
    std::string getFailureReason() const;
    double getPlatformFee() const;
    double getDriverAmount() const;
    std::string getCurrency() const;
    
    // Setters
    void setStatus(PaymentStatus status);
    void setTransactionId(const std::string& transactionId);
    void setFailureReason(const std::string& reason);
    void setPlatformFee(double fee);
    void setDriverAmount(double amount);
    void setCurrency(const std::string& currency);
    
    // Utility methods
    std::string getMethodString() const;
    std::string getStatusString() const;
    bool isSuccessful() const;
    void processPayment();
    void markAsFailed(const std::string& reason);
    void refund();
    double calculatePlatformFee() const;
    double calculateDriverAmount() const;
    std::string generateReceipt() const;
}; 