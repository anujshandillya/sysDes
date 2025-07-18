#include "../include/Payment.hpp"
#include <sstream>
#include <iomanip>
#include <random>

Payment::Payment(const std::string& id, const std::string& bookingId, double amount, PaymentMethod method)
    : id(id), bookingId(bookingId), amount(amount), method(method), status(PaymentStatus::PENDING) {}

std::string Payment::getId() const { return id; }
std::string Payment::getBookingId() const { return bookingId; }
double Payment::getAmount() const { return amount; }
PaymentMethod Payment::getMethod() const { return method; }
PaymentStatus Payment::getStatus() const { return status; }
std::string Payment::getTransactionId() const { return transactionId; }
std::chrono::system_clock::time_point Payment::getPaymentTime() const { return paymentTime; }
std::string Payment::getFailureReason() const { return failureReason; }

void Payment::setAmount(double amount) { this->amount = amount; }
void Payment::setMethod(PaymentMethod method) { this->method = method; }
void Payment::setStatus(PaymentStatus status) { this->status = status; }
void Payment::setTransactionId(const std::string& transactionId) { this->transactionId = transactionId; }
void Payment::setPaymentTime(const std::chrono::system_clock::time_point& paymentTime) { this->paymentTime = paymentTime; }
void Payment::setFailureReason(const std::string& reason) { this->failureReason = reason; }

bool Payment::processPayment() {
    if (status == PaymentStatus::PENDING) {
        status = PaymentStatus::PROCESSING;
        
        // Simulate payment processing
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        
        if (dis(gen) > 0.1) { // 90% success rate
            status = PaymentStatus::COMPLETED;
            paymentTime = std::chrono::system_clock::now();
            
            // Generate transaction ID
            std::ostringstream oss;
            oss << "TXN" << std::hex << std::hash<std::string>{}(id + bookingId);
            transactionId = oss.str();
            
            return true;
        } else {
            status = PaymentStatus::FAILED;
            failureReason = "Payment gateway error";
            return false;
        }
    }
    return false;
}

bool Payment::refundPayment() {
    if (status == PaymentStatus::COMPLETED) {
        status = PaymentStatus::REFUNDED;
        return true;
    }
    return false;
}

bool Payment::cancelPayment() {
    if (status == PaymentStatus::PENDING || status == PaymentStatus::PROCESSING) {
        status = PaymentStatus::CANCELLED;
        return true;
    }
    return false;
}

std::string Payment::getMethodString() const {
    switch (method) {
        case PaymentMethod::CREDIT_CARD: return "Credit Card";
        case PaymentMethod::DEBIT_CARD: return "Debit Card";
        case PaymentMethod::NET_BANKING: return "Net Banking";
        case PaymentMethod::UPI: return "UPI";
        case PaymentMethod::WALLET: return "Wallet";
        case PaymentMethod::CASH: return "Cash";
        default: return "Unknown";
    }
}

std::string Payment::getStatusString() const {
    switch (status) {
        case PaymentStatus::PENDING: return "Pending";
        case PaymentStatus::PROCESSING: return "Processing";
        case PaymentStatus::COMPLETED: return "Completed";
        case PaymentStatus::FAILED: return "Failed";
        case PaymentStatus::REFUNDED: return "Refunded";
        case PaymentStatus::CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

std::string Payment::getPaymentTimeString() const {
    if (paymentTime == std::chrono::system_clock::time_point{}) {
        return "Not processed";
    }
    auto time_t = std::chrono::system_clock::to_time_t(paymentTime);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

bool Payment::isCompleted() const { return status == PaymentStatus::COMPLETED; }
bool Payment::isFailed() const { return status == PaymentStatus::FAILED; }
bool Payment::isPending() const { return status == PaymentStatus::PENDING; }
bool Payment::isRefunded() const { return status == PaymentStatus::REFUNDED; }

std::string Payment::getPaymentInfo() const {
    std::ostringstream oss;
    oss << "Payment ID: " << id << "\n";
    oss << "Booking ID: " << bookingId << "\n";
    oss << "Amount: $" << std::fixed << std::setprecision(2) << amount << "\n";
    oss << "Method: " << getMethodString() << "\n";
    oss << "Status: " << getStatusString() << "\n";
    if (!transactionId.empty()) {
        oss << "Transaction ID: " << transactionId << "\n";
    }
    oss << "Payment Time: " << getPaymentTimeString();
    if (!failureReason.empty()) {
        oss << "\nFailure Reason: " << failureReason;
    }
    return oss.str();
} 