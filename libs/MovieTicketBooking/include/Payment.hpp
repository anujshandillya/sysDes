#ifndef PAYMENT_HPP
#define PAYMENT_HPP

#include <string>
#include <chrono>

enum class PaymentMethod {
    CREDIT_CARD,
    DEBIT_CARD,
    NET_BANKING,
    UPI,
    WALLET,
    CASH
};

enum class PaymentStatus {
    PENDING,
    PROCESSING,
    COMPLETED,
    FAILED,
    REFUNDED,
    CANCELLED
};

class Payment {
private:
    std::string id;
    std::string bookingId;
    double amount;
    PaymentMethod method;
    PaymentStatus status;
    std::string transactionId;
    std::chrono::system_clock::time_point paymentTime;
    std::string failureReason;

public:
    Payment(const std::string& id, const std::string& bookingId, double amount, 
            PaymentMethod method);
    
    // Getters
    std::string getId() const;
    std::string getBookingId() const;
    double getAmount() const;
    PaymentMethod getMethod() const;
    PaymentStatus getStatus() const;
    std::string getTransactionId() const;
    std::chrono::system_clock::time_point getPaymentTime() const;
    std::string getFailureReason() const;
    
    // Setters
    void setAmount(double amount);
    void setMethod(PaymentMethod method);
    void setStatus(PaymentStatus status);
    void setTransactionId(const std::string& transactionId);
    void setPaymentTime(const std::chrono::system_clock::time_point& paymentTime);
    void setFailureReason(const std::string& reason);
    
    // Payment processing
    bool processPayment();
    bool refundPayment();
    bool cancelPayment();
    
    // Utility methods
    std::string getMethodString() const;
    std::string getStatusString() const;
    std::string getPaymentTimeString() const;
    bool isCompleted() const;
    bool isFailed() const;
    bool isPending() const;
    bool isRefunded() const;
    std::string getPaymentInfo() const;
};

#endif // PAYMENT_HPP 