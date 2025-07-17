#ifndef FEED_HPP
#define FEED_HPP

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <mutex>

class User;
class Post;

enum class FeedType {
    HOME,
    PROFILE,
    EXPLORE,
    TRENDING,
    HASHTAG,
    USER_TIMELINE,
    BOOKMARKS,
    LIKES
};

enum class FeedAlgorithm {
    CHRONOLOGICAL,
    RELEVANCE,
    ENGAGEMENT,
    PERSONALIZED,
    TRENDING
};

class Feed {
private:
    std::string feedId;
    std::string userId;
    FeedType type;
    FeedAlgorithm algorithm;
    
    std::vector<std::shared_ptr<Post>> posts;
    std::map<std::string, double> postScores;
    std::set<std::string> seenPosts;
    std::queue<std::string> postQueue;
    
    // Pagination
    int pageSize;
    int currentPage;
    bool hasMorePosts;
    std::string lastPostId;
    
    // Filtering
    std::set<std::string> excludedUsers;
    std::set<std::string> excludedHashtags;
    std::set<std::string> includedHashtags;
    bool includeRetweets;
    bool includeReplies;
    bool includeSensitiveContent;
    
    // Personalization
    std::map<std::string, double> userWeights;
    std::map<std::string, double> hashtagWeights;
    std::map<std::string, double> topicWeights;
    
    // Caching
    std::map<int, std::vector<std::shared_ptr<Post>>> pageCache;
    std::chrono::system_clock::time_point lastRefresh;
    int cacheExpiryMinutes;
    
    // Thread safety
    mutable std::mutex feedMutex;
    
public:
    Feed(const std::string& feedId, const std::string& userId, FeedType type = FeedType::HOME);
    ~Feed();
    
    // Getters
    std::string getFeedId() const;
    std::string getUserId() const;
    FeedType getType() const;
    FeedAlgorithm getAlgorithm() const;
    int getPageSize() const;
    int getCurrentPage() const;
    bool getHasMorePosts() const;
    std::string getLastPostId() const;
    
    // Setters
    void setAlgorithm(FeedAlgorithm algorithm);
    void setPageSize(int size);
    void setCacheExpiry(int minutes);
    
    // Feed operations
    std::vector<std::shared_ptr<Post>> getPosts(int page = 0);
    std::vector<std::shared_ptr<Post>> getNextPage();
    std::vector<std::shared_ptr<Post>> refreshFeed();
    void clearFeed();
    void addPost(std::shared_ptr<Post> post);
    void removePost(const std::string& postId);
    void markPostAsSeen(const std::string& postId);
    
    // Filtering operations
    void excludeUser(const std::string& userId);
    void includeUser(const std::string& userId);
    void excludeHashtag(const std::string& hashtag);
    void includeHashtag(const std::string& hashtag);
    void setIncludeRetweets(bool include);
    void setIncludeReplies(bool include);
    void setIncludeSensitiveContent(bool include);
    
    // Personalization operations
    void setUserWeight(const std::string& userId, double weight);
    void setHashtagWeight(const std::string& hashtag, double weight);
    void setTopicWeight(const std::string& topic, double weight);
    void updateUserPreferences(const std::map<std::string, double>& preferences);
    
    // Algorithm operations
    void switchToChronological();
    void switchToRelevance();
    void switchToEngagement();
    void switchToPersonalized();
    void switchToTrending();
    
    // Scoring and ranking
    double calculatePostScore(std::shared_ptr<Post> post) const;
    void updatePostScores();
    void sortPostsByScore();
    void sortPostsByTime();
    void sortPostsByEngagement();
    
    // Queries
    bool hasSeenPost(const std::string& postId) const;
    bool isUserExcluded(const std::string& userId) const;
    bool isHashtagExcluded(const std::string& hashtag) const;
    bool isHashtagIncluded(const std::string& hashtag) const;
    bool shouldIncludePost(std::shared_ptr<Post> post) const;
    
    // Statistics
    int getTotalPosts() const;
    int getSeenPostsCount() const;
    int getUnseenPostsCount() const;
    double getAverageEngagement() const;
    std::map<std::string, int> getTopHashtags() const;
    std::map<std::string, int> getTopUsers() const;
    
    // Cache operations
    void clearCache();
    void refreshCache();
    bool isCacheValid() const;
    void preloadNextPage();
    
    // Utility methods
    std::string toString() const;
    bool isValid() const;
    void resetPagination();
    void optimizeForUser(const std::string& userId);
    
private:
    // Helper methods
    std::vector<std::shared_ptr<Post>> fetchPostsFromSource();
    std::vector<std::shared_ptr<Post>> applyFilters(const std::vector<std::shared_ptr<Post>>& posts);
    std::vector<std::shared_ptr<Post>> applyAlgorithm(const std::vector<std::shared_ptr<Post>>& posts);
    void updateCache(int page, const std::vector<std::shared_ptr<Post>>& posts);
    double calculateRelevanceScore(std::shared_ptr<Post> post) const;
    double calculateEngagementScore(std::shared_ptr<Post> post) const;
    double calculatePersonalizationScore(std::shared_ptr<Post> post) const;
    double calculateTrendingScore(std::shared_ptr<Post> post) const;
    bool isPostEligible(std::shared_ptr<Post> post) const;
    void logFeedActivity(const std::string& action, const std::string& details);
};

#endif // FEED_HPP 