#ifndef ANALYTICS_HPP
#define ANALYTICS_HPP

#include <string>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct ClickEvent {
    std::string shortCode;
    std::string ipAddress;
    std::string userAgent;
    std::string referrer;
    std::chrono::system_clock::time_point timestamp;
    std::string country;
    std::string city;
    std::string deviceType;
};

struct UrlStats {
    std::string shortCode;
    int totalClicks;
    int uniqueClicks;
    std::chrono::system_clock::time_point firstClick;
    std::chrono::system_clock::time_point lastClick;
    std::unordered_map<std::string, int> clicksByCountry;
    std::unordered_map<std::string, int> clicksByDevice;
    std::unordered_map<std::string, int> clicksByReferrer;
};

class Analytics {
private:
    std::unordered_map<std::string, std::vector<ClickEvent>> clickEvents; // shortCode -> events
    std::unordered_map<std::string, UrlStats> urlStats; // shortCode -> stats
    std::unordered_map<std::string, std::unordered_set<std::string>> uniqueVisitors; // shortCode -> set of IPs

public:
    Analytics();
    
    // Record click events
    void recordClick(const std::string& shortCode, const std::string& ipAddress, 
                    const std::string& userAgent = "", const std::string& referrer = "",
                    const std::string& country = "", const std::string& city = "",
                    const std::string& deviceType = "");
    
    // Get statistics
    UrlStats getUrlStats(const std::string& shortCode);
    std::vector<ClickEvent> getClickEvents(const std::string& shortCode, int limit = 100);
    int getTotalClicks(const std::string& shortCode);
    int getUniqueClicks(const std::string& shortCode);
    
    // Analytics queries
    std::vector<std::pair<std::string, int>> getTopUrlsByClicks(int limit = 10);
    std::vector<std::pair<std::string, int>> getClicksByCountry(const std::string& shortCode);
    std::vector<std::pair<std::string, int>> getClicksByDevice(const std::string& shortCode);
    std::vector<std::pair<std::string, int>> getClicksByReferrer(const std::string& shortCode);
    
    // Time-based analytics
    std::vector<std::pair<std::string, int>> getClicksByHour(const std::string& shortCode, int days = 7);
    std::vector<std::pair<std::string, int>> getClicksByDay(const std::string& shortCode, int days = 30);
    
    // Cleanup
    void cleanupOldEvents(int daysToKeep = 90);
    void clearUrlStats(const std::string& shortCode);
    void clearAll();
};

#endif // ANALYTICS_HPP 