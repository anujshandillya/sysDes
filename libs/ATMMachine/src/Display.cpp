#include "../include/Display.hpp"
#include <iostream>

Display::Display() : isOperational(false), maxMessageLength(80), isBacklit(true) {
}

Display::~Display() = default;

void Display::showMessage(const std::string& message) {
    if (!isOperational) {
        std::cout << "[DISPLAY ERROR] Display not operational" << std::endl;
        return;
    }
    
    currentMessage = formatMessage(message);
    addToHistory(currentMessage);
    
    std::cout << "[DISPLAY] " << currentMessage << std::endl;
}

void Display::showError(const std::string& error) {
    if (!isOperational) {
        std::cout << "[DISPLAY ERROR] Display not operational" << std::endl;
        return;
    }
    
    std::string errorMsg = "ERROR: " + formatMessage(error);
    currentMessage = errorMsg;
    addToHistory(errorMsg);
    
    std::cout << "[DISPLAY ERROR] " << errorMsg << std::endl;
}

void Display::showWelcome() {
    showMessage("Welcome to ATM Banking");
}

void Display::showGoodbye() {
    showMessage("Thank you for using ATM Banking");
}

void Display::clear() {
    currentMessage.clear();
    std::cout << "[DISPLAY] Screen cleared" << std::endl;
}

void Display::showMainMenu() {
    std::string menu = "Select Account Type:\n1. Savings\n2. Checking\n3. Credit";
    showMessage(menu);
}

void Display::showTransactionMenu() {
    std::string menu = "Select Transaction:\n1. Withdraw\n2. Deposit\n3. Transfer\n4. Balance Inquiry\n5. Mini Statement";
    showMessage(menu);
}

void Display::showAccountMenu() {
    std::string menu = "Select Account:\n1. Primary Account\n2. Secondary Account";
    showMessage(menu);
}

void Display::showAmountMenu() {
    std::string menu = "Enter Amount:";
    showMessage(menu);
}

void Display::showConfirmationMenu() {
    std::string menu = "Confirm Transaction:\n1. Yes\n2. No";
    showMessage(menu);
}

bool Display::isDisplayOperational() const {
    return isOperational;
}

void Display::setOperationalStatus(bool status) {
    isOperational = status;
}

std::string Display::getCurrentMessage() const {
    return currentMessage;
}

std::vector<std::string> Display::getMessageHistory() const {
    return messageHistory;
}

void Display::clearHistory() {
    messageHistory.clear();
}

void Display::setBacklight(bool on) {
    isBacklit = on;
}

bool Display::isBacklitOn() const {
    return isBacklit;
}

void Display::setMaxMessageLength(int length) {
    maxMessageLength = length;
}

int Display::getMaxMessageLength() const {
    return maxMessageLength;
}

std::string Display::formatMessage(const std::string& message) const {
    if (message.length() <= maxMessageLength) {
        return message;
    }
    
    // Truncate message if too long
    return message.substr(0, maxMessageLength - 3) + "...";
}

void Display::addToHistory(const std::string& message) {
    messageHistory.push_back(message);
    
    // Keep only last 100 messages
    if (messageHistory.size() > 100) {
        messageHistory.erase(messageHistory.begin());
    }
} 