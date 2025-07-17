#include "../include/Keypad.hpp"
#include <iostream>

Keypad::Keypad() : isOperational(false), isSecureMode(false), maxInputLength(20) {
}

Keypad::~Keypad() = default;

std::string Keypad::getInput() {
    if (!isOperational) {
        std::cout << "[KEYPAD ERROR] Keypad not operational" << std::endl;
        return "";
    }
    
    std::string input;
    std::cout << "[KEYPAD] Enter input: ";
    std::getline(std::cin, input);
    
    if (input.length() > maxInputLength) {
        input = input.substr(0, maxInputLength);
    }
    
    addToHistory(input);
    return input;
}

std::string Keypad::getSecureInput() {
    if (!isOperational) {
        std::cout << "[KEYPAD ERROR] Keypad not operational" << std::endl;
        return "";
    }
    
    setSecureMode(true);
    std::string input;
    std::cout << "[KEYPAD] Enter PIN (masked): ";
    std::getline(std::cin, input);
    
    if (input.length() > maxInputLength) {
        input = input.substr(0, maxInputLength);
    }
    
    setSecureMode(false);
    addToHistory(maskString(input));
    return input;
}

void Keypad::clearInput() {
    currentInput.clear();
}

void Keypad::addDigit(char digit) {
    if (currentInput.length() < maxInputLength) {
        currentInput += digit;
    }
}

void Keypad::removeLastDigit() {
    if (!currentInput.empty()) {
        currentInput.pop_back();
    }
}

bool Keypad::isValidInput(const std::string& input) const {
    return !input.empty() && input.length() <= maxInputLength;
}

bool Keypad::isValidPIN(const std::string& pin) const {
    return pin.length() == 4 && isNumeric(pin);
}

bool Keypad::isValidAmount(const std::string& amount) const {
    if (!isNumeric(amount)) {
        return false;
    }
    
    try {
        double value = std::stod(amount);
        return value > 0 && value <= 10000; // Max $10,000
    } catch (...) {
        return false;
    }
}

std::vector<std::string> Keypad::getInputHistory() const {
    return inputHistory;
}

void Keypad::clearHistory() {
    inputHistory.clear();
}

std::string Keypad::getLastInput() const {
    if (inputHistory.empty()) {
        return "";
    }
    return inputHistory.back();
}

bool Keypad::isKeypadOperational() const {
    return isOperational;
}

void Keypad::setOperationalStatus(bool status) {
    isOperational = status;
}

std::string Keypad::getCurrentInput() const {
    return currentInput;
}

void Keypad::setSecureMode(bool secure) {
    isSecureMode = secure;
}

bool Keypad::isSecureModeEnabled() const {
    return isSecureMode;
}

void Keypad::maskInput() {
    if (isSecureMode) {
        std::cout << "[KEYPAD] Input masked for security" << std::endl;
    }
}

void Keypad::setMaxInputLength(int length) {
    maxInputLength = length;
}

int Keypad::getMaxInputLength() const {
    return maxInputLength;
}

bool Keypad::isDigit(char c) const {
    return std::isdigit(c);
}

bool Keypad::isNumeric(const std::string& input) const {
    for (char c : input) {
        if (!std::isdigit(c) && c != '.') {
            return false;
        }
    }
    return true;
}

std::string Keypad::maskString(const std::string& input) const {
    return std::string(input.length(), '*');
}

void Keypad::addToHistory(const std::string& input) {
    inputHistory.push_back(input);
    
    // Keep only last 50 inputs
    if (inputHistory.size() > 50) {
        inputHistory.erase(inputHistory.begin());
    }
} 