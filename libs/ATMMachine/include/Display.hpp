#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <string>
#include <vector>

class Display {
private:
    bool isOperational;
    std::string currentMessage;
    std::vector<std::string> messageHistory;
    int maxMessageLength;
    bool isBacklit;
    
public:
    Display();
    ~Display();
    
    // Display operations
    void showMessage(const std::string& message);
    void showError(const std::string& error);
    void showWelcome();
    void showGoodbye();
    void clear();
    
    // Menu operations
    void showMainMenu();
    void showTransactionMenu();
    void showAccountMenu();
    void showAmountMenu();
    void showConfirmationMenu();
    
    // Status operations
    bool isDisplayOperational() const;
    void setOperationalStatus(bool status);
    std::string getCurrentMessage() const;
    
    // History operations
    std::vector<std::string> getMessageHistory() const;
    void clearHistory();
    
    // Display settings
    void setBacklight(bool on);
    bool isBacklitOn() const;
    void setMaxMessageLength(int length);
    int getMaxMessageLength() const;
    
private:
    std::string formatMessage(const std::string& message) const;
    void addToHistory(const std::string& message);
};

#endif // DISPLAY_HPP 