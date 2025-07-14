#pragma once

#include <chrono>
#include <string>
enum class PaymentMethod { CASH, CREDIT_CARD, DEBIT_CARD, DIGITAL_WALLET };

class Payment {
    std::string paymentId;
    double amount;
    PaymentMethod method;
    std::chrono::system_clock::time_point paymentTime;
    std::string ticketId;

public:
    Payment(std::string id, double amt, PaymentMethod payMethod, std::string ticket);

    std::string getPaymentId() const { return paymentId; }
    double getAmount() const { return amount; }
    PaymentMethod getMethod() const { return method; }
    std::string getTicketId() const { return ticketId; }
    bool processPayment();
};