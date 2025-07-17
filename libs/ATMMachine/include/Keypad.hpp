#ifndef KEYPAD_HPP
#define KEYPAD_HPP

#include <string>
#include <vector>

class Keypad {
private:
    bool isOperational;
    std::vector<std::string> inputHistory;
    std::string currentInput;
    bool isSecureMode; // For PIN entry
    int maxInputLength;
    
public:
    Keypad();
    ~Keypad();
    
    // Input operations
    std::string getInput();
    std::string getSecureInput(); // For PIN entry
    void clearInput();
    void addDigit(char digit);
    void removeLastDigit();
    
    // Validation operations
    bool isValidInput(const std::string& input) const;
    bool isValidPIN(const std::string& pin) const;
    bool isValidAmount(const std::string& amount) const;
    
    // History operations
    void addToHistory(const std::string& input);
    std::vector<std::string> getInputHistory() const;
    std::string getLastInput() const;
    void clearHistory();
    
    // Status operations
    bool isKeypadOperational() const;
    void setOperationalStatus(bool status);
    std::string getCurrentInput() const;
    
    // Security operations
    void setSecureMode(bool secure);
    bool isSecureModeEnabled() const;
    void maskInput();
    
    // Configuration
    void setMaxInputLength(int length);
    int getMaxInputLength() const;
    
private:
    bool isDigit(char c) const;
    bool isNumeric(const std::string& input) const;
    std::string maskString(const std::string& input) const;
};

#endif // KEYPAD_HPP 