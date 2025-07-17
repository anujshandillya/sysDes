#ifndef HASHTAG_HPP
#define HASHTAG_HPP

#include <string>
#include <vector>
#include <set>
#include <memory>
#include <chrono>

class Post;

class Hashtag {
private:
    std::string hashtagId;
    std::string text;
    std::string description;
    
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point lastUsed;
    
    // Statistics
    int postCount;
    int followerCount;
    int viewCount;
    int trendScore;
    
    // Content
    std::vector<std::shared_ptr<Post>> posts;
    std::set<std::string> followers;
    
    // Trending information
    bool isTrending;
    int trendingRank;
    std::chrono::system_clock::time_point trendingStartTime;
    std::chrono::system_clock::time_point trendingEndTime;
    
    // Moderation
    bool isBlocked;
    bool isSensitive;
    std::vector<std::string> contentWarnings;
    
public:
    Hashtag(const std::string& hashtagId, const std::string& text);
    ~Hashtag();
    
    // Getters
    std::string getHashtagId() const;
    std::string getText() const;
    std::string getDescription() const;
    std::chrono::system_clock::time_point getCreatedAt() const;
    std::chrono::system_clock::time_point getLastUsed() const;
    
    // Statistics getters
    int getPostCount() const;
    int getFollowerCount() const;
    int getViewCount() const;
    int getTrendScore() const;
    
    // Content getters
    std::vector<std::shared_ptr<Post>> getPosts() const;
    std::set<std::string> getFollowers() const;
    
    // Trending getters
    bool getIsTrending() const;
    int getTrendingRank() const;
    std::chrono::system_clock::time_point getTrendingStartTime() const;
    std::chrono::system_clock::time_point getTrendingEndTime() const;
    
    // Moderation getters
    bool getIsBlocked() const;
    bool getIsSensitive() const;
    std::vector<std::string> getContentWarnings() const;
    
    // Setters
    void setDescription(const std::string& description);
    void setIsTrending(bool trending);
    void setTrendingRank(int rank);
    void setIsBlocked(bool blocked);
    void setIsSensitive(bool sensitive);
    
    // Content operations
    void addPost(std::shared_ptr<Post> post);
    void removePost(const std::string& postId);
    void addFollower(const std::string& userId);
    void removeFollower(const std::string& userId);
    
    // Trending operations
    void startTrending(int rank);
    void stopTrending();
    void updateTrendScore(int score);
    
    // Moderation operations
    void addContentWarning(const std::string& warning);
    void removeContentWarning(const std::string& warning);
    
    // Statistics operations
    void incrementPostCount();
    void decrementPostCount();
    void incrementViewCount();
    void updateLastUsed();
    
    // Queries
    bool hasFollower(const std::string& userId) const;
    bool containsPost(const std::string& postId) const;
    bool isCurrentlyTrending() const;
    bool hasContentWarning(const std::string& warning) const;
    
    // Utility methods
    std::string toString() const;
    bool isValid() const;
    void updateStatistics();
    std::string getFormattedText() const;
    int getAgeInDays() const;
    double getTrendingDuration() const;
};

#endif // HASHTAG_HPP 