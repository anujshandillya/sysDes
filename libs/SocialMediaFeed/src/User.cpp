#include "../include/User.hpp"
#include "../include/Post.hpp"
#include "../include/Notification.hpp"
#include <algorithm>
#include <sstream>

User::User(const std::string& userId, const std::string& username, const std::string& email)
    : userId(userId), username(username), email(email), displayName(username), 
      isVerified(false), isPrivate(false), followerCount(0), followingCount(0), 
      postCount(0), likeCount(0) {
    createdAt = std::chrono::system_clock::now();
    lastActive = createdAt;
}

User::~User() = default;

// Getters
std::string User::getUserId() const { return userId; }
std::string User::getUsername() const { return username; }
std::string User::getEmail() const { return email; }
std::string User::getDisplayName() const { return displayName; }
std::string User::getBio() const { return bio; }
std::string User::getProfilePicture() const { return profilePicture; }
std::string User::getCoverPhoto() const { return coverPhoto; }
bool User::getIsVerified() const { return isVerified; }
bool User::getIsPrivate() const { return isPrivate; }
std::chrono::system_clock::time_point User::getCreatedAt() const { return createdAt; }
std::chrono::system_clock::time_point User::getLastActive() const { return lastActive; }

// Setters
void User::setDisplayName(const std::string& name) { displayName = name; }
void User::setBio(const std::string& userBio) { bio = userBio; }
void User::setProfilePicture(const std::string& picture) { profilePicture = picture; }
void User::setCoverPhoto(const std::string& photo) { coverPhoto = photo; }
void User::setVerified(bool verified) { isVerified = verified; }
void User::setPrivate(bool isPrivate) { this->isPrivate = isPrivate; }
void User::updateLastActive() { lastActive = std::chrono::system_clock::now(); }

// Social operations
bool User::followUser(const std::string& targetUserId) {
    if (targetUserId == userId) return false; // Can't follow self
    if (following.find(targetUserId) != following.end()) return false; // Already following
    
    following.insert(targetUserId);
    followingCount = following.size();
    updateLastActive();
    return true;
}

bool User::unfollowUser(const std::string& targetUserId) {
    auto it = following.find(targetUserId);
    if (it == following.end()) return false; // Not following
    
    following.erase(it);
    followingCount = following.size();
    updateLastActive();
    return true;
}

bool User::blockUser(const std::string& targetUserId) {
    if (targetUserId == userId) return false; // Can't block self
    
    blockedUsers.insert(targetUserId);
    // Remove from following/followers if present
    following.erase(targetUserId);
    followers.erase(targetUserId);
    updateStatistics();
    return true;
}

bool User::unblockUser(const std::string& targetUserId) {
    auto it = blockedUsers.find(targetUserId);
    if (it == blockedUsers.end()) return false;
    
    blockedUsers.erase(it);
    return true;
}

bool User::muteUser(const std::string& targetUserId) {
    if (targetUserId == userId) return false;
    mutedUsers.insert(targetUserId);
    return true;
}

bool User::unmuteUser(const std::string& targetUserId) {
    auto it = mutedUsers.find(targetUserId);
    if (it == mutedUsers.end()) return false;
    
    mutedUsers.erase(it);
    return true;
}

// Content operations
void User::addPost(std::shared_ptr<Post> post) {
    posts.push_back(post);
    postCount = posts.size();
    updateLastActive();
}

void User::removePost(const std::string& postId) {
    posts.erase(std::remove_if(posts.begin(), posts.end(),
        [&postId](const std::shared_ptr<Post>& post) {
            return post->getPostId() == postId;
        }), posts.end());
    postCount = posts.size();
}

void User::likePost(std::shared_ptr<Post> post) {
    if (std::find(likedPosts.begin(), likedPosts.end(), post) == likedPosts.end()) {
        likedPosts.push_back(post);
        likeCount = likedPosts.size();
    }
}

void User::unlikePost(const std::string& postId) {
    likedPosts.erase(std::remove_if(likedPosts.begin(), likedPosts.end(),
        [&postId](const std::shared_ptr<Post>& post) {
            return post->getPostId() == postId;
        }), likedPosts.end());
    likeCount = likedPosts.size();
}

void User::bookmarkPost(std::shared_ptr<Post> post) {
    if (std::find(bookmarkedPosts.begin(), bookmarkedPosts.end(), post) == bookmarkedPosts.end()) {
        bookmarkedPosts.push_back(post);
    }
}

void User::unbookmarkPost(const std::string& postId) {
    bookmarkedPosts.erase(std::remove_if(bookmarkedPosts.begin(), bookmarkedPosts.end(),
        [&postId](const std::shared_ptr<Post>& post) {
            return post->getPostId() == postId;
        }), bookmarkedPosts.end());
}

void User::retweetPost(std::shared_ptr<Post> post) {
    if (std::find(retweetedPosts.begin(), retweetedPosts.end(), post) == retweetedPosts.end()) {
        retweetedPosts.push_back(post);
    }
}

void User::unretweetPost(const std::string& postId) {
    retweetedPosts.erase(std::remove_if(retweetedPosts.begin(), retweetedPosts.end(),
        [&postId](const std::shared_ptr<Post>& post) {
            return post->getPostId() == postId;
        }), retweetedPosts.end());
}

// Notification operations
void User::addNotification(std::shared_ptr<Notification> notification) {
    notifications.push_back(notification);
}

void User::markNotificationAsRead(const std::string& notificationId) {
    for (auto& notification : notifications) {
        if (notification->getNotificationId() == notificationId) {
            notification->markAsRead();
            break;
        }
    }
}

void User::clearNotifications() {
    notifications.clear();
}

// Queries
bool User::isFollowing(const std::string& targetUserId) const {
    return following.find(targetUserId) != following.end();
}

bool User::isBlocked(const std::string& targetUserId) const {
    return blockedUsers.find(targetUserId) != blockedUsers.end();
}

bool User::isMuted(const std::string& targetUserId) const {
    return mutedUsers.find(targetUserId) != mutedUsers.end();
}

bool User::hasLikedPost(const std::string& postId) const {
    return std::any_of(likedPosts.begin(), likedPosts.end(),
        [&postId](const std::shared_ptr<Post>& post) {
            return post->getPostId() == postId;
        });
}

bool User::hasBookmarkedPost(const std::string& postId) const {
    return std::any_of(bookmarkedPosts.begin(), bookmarkedPosts.end(),
        [&postId](const std::shared_ptr<Post>& post) {
            return post->getPostId() == postId;
        });
}

bool User::hasRetweetedPost(const std::string& postId) const {
    return std::any_of(retweetedPosts.begin(), retweetedPosts.end(),
        [&postId](const std::shared_ptr<Post>& post) {
            return post->getPostId() == postId;
        });
}

// Statistics
int User::getFollowerCount() const { return followerCount; }
int User::getFollowingCount() const { return followingCount; }
int User::getPostCount() const { return postCount; }
int User::getLikeCount() const { return likeCount; }

// Content retrieval
std::vector<std::shared_ptr<Post>> User::getPosts() const { return posts; }
std::vector<std::shared_ptr<Post>> User::getLikedPosts() const { return likedPosts; }
std::vector<std::shared_ptr<Post>> User::getBookmarkedPosts() const { return bookmarkedPosts; }
std::vector<std::shared_ptr<Post>> User::getRetweetedPosts() const { return retweetedPosts; }
std::vector<std::shared_ptr<Notification>> User::getNotifications() const { return notifications; }

std::vector<std::shared_ptr<Notification>> User::getUnreadNotifications() const {
    std::vector<std::shared_ptr<Notification>> unread;
    for (const auto& notification : notifications) {
        if (notification->isUnread()) {
            unread.push_back(notification);
        }
    }
    return unread;
}

// Social lists
std::set<std::string> User::getFollowers() const { return followers; }
std::set<std::string> User::getFollowing() const { return following; }
std::set<std::string> User::getBlockedUsers() const { return blockedUsers; }
std::set<std::string> User::getMutedUsers() const { return mutedUsers; }

// Utility methods
std::string User::toString() const {
    std::stringstream ss;
    ss << "User{"
       << "userId='" << userId << "', "
       << "username='" << username << "', "
       << "displayName='" << displayName << "', "
       << "followers=" << followerCount << ", "
       << "following=" << followingCount << ", "
       << "posts=" << postCount << ", "
       << "verified=" << (isVerified ? "true" : "false") << ", "
       << "private=" << (isPrivate ? "true" : "false")
       << "}";
    return ss.str();
}

bool User::isValid() const {
    return !userId.empty() && !username.empty() && !email.empty();
}

void User::updateStatistics() {
    followerCount = followers.size();
    followingCount = following.size();
    postCount = posts.size();
    likeCount = likedPosts.size();
} 