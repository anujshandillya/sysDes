#include "../include/Payment.hpp"
#include <sstream>
#include <iomanip>

Payment::Payment(const std::string& id, const std::string& tripId, const std::string& customerId,
                 const std::string& driverId, double amount, PaymentMethod method)
    : paymentId(id), tripId(tripId), customerId(customerId), driverId(driverId),
      amount(amount), method(method), status(PaymentStatus::PENDING),
      timestamp(std::chrono::system_clock::now()), platformFee(0.0), driverAmount(0.0), currency("USD") {
    
    calculatePlatformFee();
    calculateDriverAmount();
}

std::string Payment::getPaymentId() const {
    return paymentId;
}

std::string Payment::getTripId() const {
    return tripId;
}

std::string Payment::getCustomerId() const {
    return customerId;
}

std::string Payment::getDriverId() const {
    return driverId;
}

double Payment::getAmount() const {
    return amount;
}

PaymentMethod Payment::getMethod() const {
    return method;
}

PaymentStatus Payment::getStatus() const {
    return status;
}

std::chrono::system_clock::time_point Payment::getTimestamp() const {
    return timestamp;
}

std::string Payment::getTransactionId() const {
    return transactionId;
}

std::string Payment::getFailureReason() const {
    return failureReason;
}

double Payment::getPlatformFee() const {
    return platformFee;
}

double Payment::getDriverAmount() const {
    return driverAmount;
}

std::string Payment::getCurrency() const {
    return currency;
}

void Payment::setStatus(PaymentStatus status) {
    this->status = status;
}

void Payment::setTransactionId(const std::string& transactionId) {
    this->transactionId = transactionId;
}

void Payment::setFailureReason(const std::string& reason) {
    failureReason = reason;
}

void Payment::setPlatformFee(double fee) {
    platformFee = fee;
}

void Payment::setDriverAmount(double amount) {
    driverAmount = amount;
}

void Payment::setCurrency(const std::string& currency) {
    this->currency = currency;
}

std::string Payment::getMethodString() const {
    switch (method) {
        case PaymentMethod::CASH: return "CASH";
        case PaymentMethod::CREDIT_CARD: return "CREDIT_CARD";
        case PaymentMethod::DEBIT_CARD: return "DEBIT_CARD";
        case PaymentMethod::UPI: return "UPI";
        case PaymentMethod::WALLET: return "WALLET";
        case PaymentMethod::NET_BANKING: return "NET_BANKING";
        default: return "UNKNOWN";
    }
}

std::string Payment::getStatusString() const {
    switch (status) {
        case PaymentStatus::PENDING: return "PENDING";
        case PaymentStatus::PROCESSING: return "PROCESSING";
        case PaymentStatus::COMPLETED: return "COMPLETED";
        case PaymentStatus::FAILED: return "FAILED";
        case PaymentStatus::REFUNDED: return "REFUNDED";
        default: return "UNKNOWN";
    }
}

bool Payment::isSuccessful() const {
    return status == PaymentStatus::COMPLETED;
}

void Payment::processPayment() {
    status = PaymentStatus::PROCESSING;
    
    // Simulate payment processing
    // In a real system, this would integrate with payment gateways
    if (method == PaymentMethod::CASH) {
        status = PaymentStatus::COMPLETED;
    } else {
        // Simulate successful payment for other methods
        status = PaymentStatus::COMPLETED;
        transactionId = "TXN_" + paymentId;
    }
}

void Payment::markAsFailed(const std::string& reason) {
    status = PaymentStatus::FAILED;
    failureReason = reason;
}

void Payment::refund() {
    status = PaymentStatus::REFUNDED;
}

double Payment::calculatePlatformFee() const {
    // Platform fee is typically 10-20% of the total amount
    return amount * 0.15; // 15% platform fee
}

double Payment::calculateDriverAmount() const {
    return amount - platformFee;
}

std::string Payment::generateReceipt() const {
    std::ostringstream oss;
    oss << "=== PAYMENT RECEIPT ===\n";
    oss << "Payment ID: " << paymentId << "\n";
    oss << "Trip ID: " << tripId << "\n";
    oss << "Customer ID: " << customerId << "\n";
    oss << "Driver ID: " << driverId << "\n";
    oss << "Amount: " << std::fixed << std::setprecision(2) << amount << " " << currency << "\n";
    oss << "Payment Method: " << getMethodString() << "\n";
    oss << "Status: " << getStatusString() << "\n";
    oss << "Platform Fee: " << platformFee << " " << currency << "\n";
    oss << "Driver Amount: " << driverAmount << " " << currency << "\n";
    oss << "Transaction ID: " << (transactionId.empty() ? "N/A" : transactionId) << "\n";
    oss << "Timestamp: " << std::chrono::duration_cast<std::chrono::seconds>(timestamp.time_since_epoch()).count() << "\n";
    oss << "======================";
    return oss.str();
} 