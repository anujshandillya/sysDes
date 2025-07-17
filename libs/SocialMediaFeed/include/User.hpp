#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
#include <set>
#include <memory>
#include <chrono>

class Post;
class Comment;
class Notification;

class User {
private:
    std::string userId;
    std::string username;
    std::string email;
    std::string displayName;
    std::string bio;
    std::string profilePicture;
    std::string coverPhoto;
    
    bool isVerified;
    bool isPrivate;
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point lastActive;
    
    // Social connections
    std::set<std::string> followers;
    std::set<std::string> following;
    std::set<std::string> blockedUsers;
    std::set<std::string> mutedUsers;
    
    // Content
    std::vector<std::shared_ptr<Post>> posts;
    std::vector<std::shared_ptr<Post>> likedPosts;
    std::vector<std::shared_ptr<Post>> bookmarkedPosts;
    std::vector<std::shared_ptr<Post>> retweetedPosts;
    
    // Notifications
    std::vector<std::shared_ptr<Notification>> notifications;
    
    // Statistics
    int followerCount;
    int followingCount;
    int postCount;
    int likeCount;
    
public:
    User(const std::string& userId, const std::string& username, const std::string& email);
    ~User();
    
    // Getters
    std::string getUserId() const;
    std::string getUsername() const;
    std::string getEmail() const;
    std::string getDisplayName() const;
    std::string getBio() const;
    std::string getProfilePicture() const;
    std::string getCoverPhoto() const;
    bool getIsVerified() const;
    bool getIsPrivate() const;
    std::chrono::system_clock::time_point getCreatedAt() const;
    std::chrono::system_clock::time_point getLastActive() const;
    
    // Setters
    void setDisplayName(const std::string& name);
    void setBio(const std::string& bio);
    void setProfilePicture(const std::string& picture);
    void setCoverPhoto(const std::string& photo);
    void setVerified(bool verified);
    void setPrivate(bool isPrivate);
    void updateLastActive();
    
    // Social operations
    bool followUser(const std::string& targetUserId);
    bool unfollowUser(const std::string& targetUserId);
    bool blockUser(const std::string& targetUserId);
    bool unblockUser(const std::string& targetUserId);
    bool muteUser(const std::string& targetUserId);
    bool unmuteUser(const std::string& targetUserId);
    
    // Content operations
    void addPost(std::shared_ptr<Post> post);
    void removePost(const std::string& postId);
    void likePost(std::shared_ptr<Post> post);
    void unlikePost(const std::string& postId);
    void bookmarkPost(std::shared_ptr<Post> post);
    void unbookmarkPost(const std::string& postId);
    void retweetPost(std::shared_ptr<Post> post);
    void unretweetPost(const std::string& postId);
    
    // Notification operations
    void addNotification(std::shared_ptr<Notification> notification);
    void markNotificationAsRead(const std::string& notificationId);
    void clearNotifications();
    
    // Queries
    bool isFollowing(const std::string& targetUserId) const;
    bool isBlocked(const std::string& targetUserId) const;
    bool isMuted(const std::string& targetUserId) const;
    bool hasLikedPost(const std::string& postId) const;
    bool hasBookmarkedPost(const std::string& postId) const;
    bool hasRetweetedPost(const std::string& postId) const;
    
    // Statistics
    int getFollowerCount() const;
    int getFollowingCount() const;
    int getPostCount() const;
    int getLikeCount() const;
    
    // Content retrieval
    std::vector<std::shared_ptr<Post>> getPosts() const;
    std::vector<std::shared_ptr<Post>> getLikedPosts() const;
    std::vector<std::shared_ptr<Post>> getBookmarkedPosts() const;
    std::vector<std::shared_ptr<Post>> getRetweetedPosts() const;
    std::vector<std::shared_ptr<Notification>> getNotifications() const;
    std::vector<std::shared_ptr<Notification>> getUnreadNotifications() const;
    
    // Social lists
    std::set<std::string> getFollowers() const;
    std::set<std::string> getFollowing() const;
    std::set<std::string> getBlockedUsers() const;
    std::set<std::string> getMutedUsers() const;
    
    // Utility methods
    std::string toString() const;
    bool isValid() const;
    void updateStatistics();
};

#endif // USER_HPP 