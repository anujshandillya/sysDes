#include "../include/Ticket.hpp"
#include <chrono>

Ticket::Ticket(std::string id, std::string license): ticketId(id), licenseNumber(std::move(license)), entryTime(std::chrono::system_clock::now()), amount(0.0), isPaid(false) {}

void Ticket::setExitTime() {
    exitTime = std::chrono::system_clock::now();
}

double Ticket::calculateAmount(double hourlyRate) {
    double hours = getDurationInHours();
    amount = hours * hourlyRate;
    return amount;
}

double Ticket::getDurationInHours() const {
    auto endTime = (exitTime.time_since_epoch().count() == 0) ? std::chrono::system_clock::now() : exitTime;

    auto duration = endTime - entryTime;
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration) %60;

    return hours.count() + (minutes.count() / 60.0);
}