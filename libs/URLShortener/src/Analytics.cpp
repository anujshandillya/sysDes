#include "../include/Analytics.hpp"
#include <algorithm>
#include <map>
#include <sstream>
#include <iomanip>

Analytics::Analytics() {
}

void Analytics::recordClick(const std::string& shortCode, const std::string& ipAddress,
                          const std::string& userAgent, const std::string& referrer,
                          const std::string& country, const std::string& city,
                          const std::string& deviceType) {
    ClickEvent event;
    event.shortCode = shortCode;
    event.ipAddress = ipAddress;
    event.userAgent = userAgent;
    event.referrer = referrer;
    event.timestamp = std::chrono::system_clock::now();
    event.country = country;
    event.city = city;
    event.deviceType = deviceType;
    
    // Add to click events
    clickEvents[shortCode].push_back(event);
    
    // Update unique visitors
    uniqueVisitors[shortCode].insert(ipAddress);
    
    // Update URL stats
    if (urlStats.find(shortCode) == urlStats.end()) {
        urlStats[shortCode] = UrlStats{shortCode, 0, 0, event.timestamp, event.timestamp};
    }
    
    UrlStats& stats = urlStats[shortCode];
    stats.totalClicks++;
    stats.uniqueClicks = static_cast<int>(uniqueVisitors[shortCode].size());
    stats.lastClick = event.timestamp;
    
    // Update country stats
    if (!country.empty()) {
        stats.clicksByCountry[country]++;
    }
    
    // Update device stats
    if (!deviceType.empty()) {
        stats.clicksByDevice[deviceType]++;
    }
    
    // Update referrer stats
    if (!referrer.empty()) {
        stats.clicksByReferrer[referrer]++;
    }
}

UrlStats Analytics::getUrlStats(const std::string& shortCode) {
    auto it = urlStats.find(shortCode);
    if (it != urlStats.end()) {
        return it->second;
    }
    return UrlStats{shortCode, 0, 0, std::chrono::system_clock::now(), std::chrono::system_clock::now()};
}

std::vector<ClickEvent> Analytics::getClickEvents(const std::string& shortCode, int limit) {
    auto it = clickEvents.find(shortCode);
    if (it == clickEvents.end()) {
        return {};
    }
    
    std::vector<ClickEvent> events = it->second;
    
    // Sort by timestamp (newest first)
    std::sort(events.begin(), events.end(),
              [](const ClickEvent& a, const ClickEvent& b) {
                  return a.timestamp > b.timestamp;
              });
    
    if (events.size() > static_cast<size_t>(limit)) {
        events.resize(limit);
    }
    
    return events;
}

int Analytics::getTotalClicks(const std::string& shortCode) {
    auto it = urlStats.find(shortCode);
    if (it != urlStats.end()) {
        return it->second.totalClicks;
    }
    return 0;
}

int Analytics::getUniqueClicks(const std::string& shortCode) {
    auto it = uniqueVisitors.find(shortCode);
    if (it != uniqueVisitors.end()) {
        return static_cast<int>(it->second.size());
    }
    return 0;
}

std::vector<std::pair<std::string, int>> Analytics::getTopUrlsByClicks(int limit) {
    std::vector<std::pair<std::string, int>> topUrls;
    
    for (const auto& pair : urlStats) {
        topUrls.emplace_back(pair.first, pair.second.totalClicks);
    }
    
    // Sort by click count (descending)
    std::sort(topUrls.begin(), topUrls.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  return a.second > b.second;
              });
    
    if (topUrls.size() > static_cast<size_t>(limit)) {
        topUrls.resize(limit);
    }
    
    return topUrls;
}

std::vector<std::pair<std::string, int>> Analytics::getClicksByCountry(const std::string& shortCode) {
    auto it = urlStats.find(shortCode);
    if (it == urlStats.end()) {
        return {};
    }
    
    std::vector<std::pair<std::string, int>> countryStats;
    for (const auto& pair : it->second.clicksByCountry) {
        countryStats.emplace_back(pair.first, pair.second);
    }
    
    // Sort by click count (descending)
    std::sort(countryStats.begin(), countryStats.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  return a.second > b.second;
              });
    
    return countryStats;
}

std::vector<std::pair<std::string, int>> Analytics::getClicksByDevice(const std::string& shortCode) {
    auto it = urlStats.find(shortCode);
    if (it == urlStats.end()) {
        return {};
    }
    
    std::vector<std::pair<std::string, int>> deviceStats;
    for (const auto& pair : it->second.clicksByDevice) {
        deviceStats.emplace_back(pair.first, pair.second);
    }
    
    // Sort by click count (descending)
    std::sort(deviceStats.begin(), deviceStats.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  return a.second > b.second;
              });
    
    return deviceStats;
}

std::vector<std::pair<std::string, int>> Analytics::getClicksByReferrer(const std::string& shortCode) {
    auto it = urlStats.find(shortCode);
    if (it == urlStats.end()) {
        return {};
    }
    
    std::vector<std::pair<std::string, int>> referrerStats;
    for (const auto& pair : it->second.clicksByReferrer) {
        referrerStats.emplace_back(pair.first, pair.second);
    }
    
    // Sort by click count (descending)
    std::sort(referrerStats.begin(), referrerStats.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  return a.second > b.second;
              });
    
    return referrerStats;
}

std::vector<std::pair<std::string, int>> Analytics::getClicksByHour(const std::string& shortCode, int days) {
    auto it = clickEvents.find(shortCode);
    if (it == clickEvents.end()) {
        return {};
    }
    
    std::map<int, int> hourlyClicks; // hour -> count
    
    auto cutoffTime = std::chrono::system_clock::now() - std::chrono::hours(24 * days);
    
    for (const auto& event : it->second) {
        if (event.timestamp >= cutoffTime) {
            auto time_t = std::chrono::system_clock::to_time_t(event.timestamp);
            std::tm* tm = std::localtime(&time_t);
            int hour = tm->tm_hour;
            hourlyClicks[hour]++;
        }
    }
    
    std::vector<std::pair<std::string, int>> hourlyStats;
    for (const auto& pair : hourlyClicks) {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << pair.first << ":00";
        hourlyStats.emplace_back(oss.str(), pair.second);
    }
    
    return hourlyStats;
}

std::vector<std::pair<std::string, int>> Analytics::getClicksByDay(const std::string& shortCode, int days) {
    auto it = clickEvents.find(shortCode);
    if (it == clickEvents.end()) {
        return {};
    }
    
    std::map<std::string, int> dailyClicks; // date -> count
    
    auto cutoffTime = std::chrono::system_clock::now() - std::chrono::hours(24 * days);
    
    for (const auto& event : it->second) {
        if (event.timestamp >= cutoffTime) {
            auto time_t = std::chrono::system_clock::to_time_t(event.timestamp);
            std::tm* tm = std::localtime(&time_t);
            std::ostringstream oss;
            oss << std::setfill('0') << std::setw(4) << (tm->tm_year + 1900) << "-"
                << std::setfill('0') << std::setw(2) << (tm->tm_mon + 1) << "-"
                << std::setfill('0') << std::setw(2) << tm->tm_mday;
            dailyClicks[oss.str()]++;
        }
    }
    
    std::vector<std::pair<std::string, int>> dailyStats;
    for (const auto& pair : dailyClicks) {
        dailyStats.emplace_back(pair.first, pair.second);
    }
    
    return dailyStats;
}

void Analytics::cleanupOldEvents(int daysToKeep) {
    auto cutoffTime = std::chrono::system_clock::now() - std::chrono::hours(24 * daysToKeep);
    
    for (auto& pair : clickEvents) {
        auto& events = pair.second;
        events.erase(
            std::remove_if(events.begin(), events.end(),
                          [cutoffTime](const ClickEvent& event) {
                              return event.timestamp < cutoffTime;
                          }),
            events.end()
        );
    }
}

void Analytics::clearUrlStats(const std::string& shortCode) {
    clickEvents.erase(shortCode);
    urlStats.erase(shortCode);
    uniqueVisitors.erase(shortCode);
}

void Analytics::clearAll() {
    clickEvents.clear();
    urlStats.clear();
    uniqueVisitors.clear();
} 