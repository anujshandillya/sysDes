#include "../include/Payment.hpp"
#include <chrono>

Payment::Payment(std::string id, double amt, PaymentMethod payMethod, std::string ticket): paymentId(id), amount(amt), method(payMethod), paymentTime(std::chrono::system_clock::now()), ticketId(ticket) {}

bool Payment::processPayment() {
    return true;
}