#ifndef SOCIAL_MEDIA_FEED_HPP
#define SOCIAL_MEDIA_FEED_HPP

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include "User.hpp"
#include "Post.hpp"
#include "Feed.hpp"
#include "Comment.hpp"
#include "Notification.hpp"
#include "Hashtag.hpp"
#include "Media.hpp"

class SocialMediaFeed {
private:
    std::string systemId;
    std::string systemName;
    std::string version;
    
    // Core data storage
    std::map<std::string, std::shared_ptr<User>> users;
    std::map<std::string, std::shared_ptr<Post>> posts;
    std::map<std::string, std::shared_ptr<Feed>> feeds;
    std::map<std::string, std::shared_ptr<Comment>> comments;
    std::map<std::string, std::shared_ptr<Notification>> notifications;
    std::map<std::string, std::shared_ptr<Hashtag>> hashtags;
    std::map<std::string, std::shared_ptr<Media>> media;
    
    // System state
    bool isRunning;
    std::atomic<int> totalUsers;
    std::atomic<int> totalPosts;
    std::atomic<int> totalComments;
    std::atomic<int> totalNotifications;
    
    // Thread safety
    mutable std::mutex systemMutex;
    std::map<std::string, std::mutex> userMutexes;
    std::map<std::string, std::mutex> postMutexes;
    
    // Background services
    std::thread notificationService;
    std::thread trendingService;
    std::thread moderationService;
    std::thread analyticsService;
    
    // Configuration
    int maxPostLength;
    int maxCommentLength;
    int maxHashtagsPerPost;
    int maxMentionsPerPost;
    int maxMediaPerPost;
    int feedPageSize;
    int notificationBatchSize;
    
    // Rate limiting
    std::map<std::string, std::chrono::system_clock::time_point> userLastPost;
    std::map<std::string, std::chrono::system_clock::time_point> userLastComment;
    int postRateLimitSeconds;
    int commentRateLimitSeconds;
    
public:
    SocialMediaFeed(const std::string& systemId, const std::string& systemName);
    ~SocialMediaFeed();
    
    // System management
    bool start();
    void stop();
    bool getIsRunning() const;
    std::string getSystemInfo() const;
    
    // User management
    std::shared_ptr<User> createUser(const std::string& username, const std::string& email);
    std::shared_ptr<User> getUser(const std::string& userId);
    std::shared_ptr<User> getUserByUsername(const std::string& username);
    bool deleteUser(const std::string& userId);
    std::vector<std::shared_ptr<User>> searchUsers(const std::string& query);
    
    // Post management
    std::shared_ptr<Post> createPost(const std::string& authorId, const std::string& content, PostType type = PostType::TEXT);
    std::shared_ptr<Post> getPost(const std::string& postId);
    bool deletePost(const std::string& postId, const std::string& authorId);
    std::vector<std::shared_ptr<Post>> searchPosts(const std::string& query);
    std::vector<std::shared_ptr<Post>> getPostsByUser(const std::string& userId);
    std::vector<std::shared_ptr<Post>> getPostsByHashtag(const std::string& hashtag);
    
    // Feed management
    std::shared_ptr<Feed> createFeed(const std::string& userId, FeedType type);
    std::shared_ptr<Feed> getFeed(const std::string& feedId);
    std::vector<std::shared_ptr<Post>> getFeedPosts(const std::string& feedId, int page = 0);
    void refreshFeed(const std::string& feedId);
    
    // Comment management
    std::shared_ptr<Comment> createComment(const std::string& postId, const std::string& authorId, const std::string& content);
    std::shared_ptr<Comment> getComment(const std::string& commentId);
    bool deleteComment(const std::string& commentId, const std::string& authorId);
    std::vector<std::shared_ptr<Comment>> getCommentsForPost(const std::string& postId);
    
    // Social interactions
    bool likePost(const std::string& postId, const std::string& userId);
    bool unlikePost(const std::string& postId, const std::string& userId);
    bool retweetPost(const std::string& postId, const std::string& userId);
    bool unretweetPost(const std::string& postId, const std::string& userId);
    bool bookmarkPost(const std::string& postId, const std::string& userId);
    bool unbookmarkPost(const std::string& postId, const std::string& userId);
    bool followUser(const std::string& followerId, const std::string& followeeId);
    bool unfollowUser(const std::string& followerId, const std::string& followeeId);
    
    // Notification management
    std::shared_ptr<Notification> createNotification(const std::string& userId, const std::string& senderId, 
                                                    NotificationType type, const std::string& title, const std::string& message);
    std::vector<std::shared_ptr<Notification>> getUserNotifications(const std::string& userId);
    std::vector<std::shared_ptr<Notification>> getUnreadNotifications(const std::string& userId);
    bool markNotificationAsRead(const std::string& notificationId);
    void sendNotification(const std::string& userId, const std::string& message);
    
    // Hashtag management
    std::shared_ptr<Hashtag> getOrCreateHashtag(const std::string& text);
    std::shared_ptr<Hashtag> getHashtag(const std::string& hashtagId);
    std::vector<std::shared_ptr<Hashtag>> getTrendingHashtags();
    std::vector<std::shared_ptr<Hashtag>> searchHashtags(const std::string& query);
    
    // Media management
    std::shared_ptr<Media> uploadMedia(const std::string& url, MediaType type);
    std::shared_ptr<Media> getMedia(const std::string& mediaId);
    bool deleteMedia(const std::string& mediaId);
    
    // Search functionality
    std::vector<std::shared_ptr<Post>> searchContent(const std::string& query);
    std::vector<std::shared_ptr<User>> searchUsersByLocation(const std::string& location);
    std::vector<std::shared_ptr<Post>> getTrendingPosts();
    std::vector<std::shared_ptr<Post>> getRecommendedPosts(const std::string& userId);
    
    // Analytics and statistics
    std::map<std::string, int> getSystemStatistics() const;
    std::map<std::string, int> getUserStatistics(const std::string& userId) const;
    std::map<std::string, int> getPostStatistics(const std::string& postId) const;
    std::vector<std::string> getTopUsers() const;
    std::vector<std::string> getTopPosts() const;
    std::vector<std::string> getTopHashtags() const;
    
    // Moderation
    bool reportPost(const std::string& postId, const std::string& reporterId, const std::string& reason);
    bool reportUser(const std::string& userId, const std::string& reporterId, const std::string& reason);
    bool blockUser(const std::string& blockerId, const std::string& blockedId);
    bool unblockUser(const std::string& blockerId, const std::string& blockedId);
    std::vector<std::shared_ptr<Post>> getReportedPosts();
    std::vector<std::shared_ptr<User>> getReportedUsers();
    
    // Rate limiting
    bool canUserPost(const std::string& userId) const;
    bool canUserComment(const std::string& userId) const;
    void updateUserActivity(const std::string& userId, const std::string& activityType);
    
    // Utility methods
    std::string generateId() const;
    bool isValidPostContent(const std::string& content) const;
    bool isValidCommentContent(const std::string& content) const;
    std::vector<std::string> extractHashtags(const std::string& content) const;
    std::vector<std::string> extractMentions(const std::string& content) const;
    void logActivity(const std::string& userId, const std::string& action, const std::string& details);
    
private:
    // Background service methods
    void runNotificationService();
    void runTrendingService();
    void runModerationService();
    void runAnalyticsService();
    
    // Helper methods
    void processNotifications();
    void updateTrendingTopics();
    void moderateContent();
    void generateAnalytics();
    void cleanupExpiredData();
    void validateUserPermissions(const std::string& userId, const std::string& action);
    void notifyFollowers(const std::string& userId, const std::string& action, const std::string& postId);
    void updateUserFeed(const std::string& userId, std::shared_ptr<Post> post);
    void processHashtags(std::shared_ptr<Post> post);
    void processMentions(std::shared_ptr<Post> post);
    std::vector<std::shared_ptr<Post>> getPostsForFeed(const std::string& userId, FeedType type);
    double calculatePostRelevance(std::shared_ptr<Post> post, const std::string& userId) const;
    void updatePostScores();
    void updateUserScores();
    void updateHashtagScores();
};

#endif // SOCIAL_MEDIA_FEED_HPP 