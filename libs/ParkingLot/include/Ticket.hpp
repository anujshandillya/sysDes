#pragma once

#include <chrono>
#include <string>
class Ticket {
    std::string ticketId;
    std::string licenseNumber;
    std::chrono::system_clock::time_point entryTime;
    std::chrono::system_clock::time_point exitTime;
    double amount;
    bool isPaid;

public:
    Ticket(std::string id, std::string license);

    std::string getTicketId() const { return ticketId; }
    std::string getLicenseNumber() const { return licenseNumber; }
    std::chrono::system_clock::time_point getEntryTime() const { return entryTime; }
    std::chrono::system_clock::time_point getExitTime() const { return exitTime; }

    void setExitTime();
    double calculateAmount(double hourlyRate);
    void markAsPaid() { isPaid = true; }
    bool getIsPaid() const { return isPaid; }

    double getDurationInHours() const;
};