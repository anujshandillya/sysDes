#include "../include/SocialMediaFeed.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <random>
#include <regex>

SocialMediaFeed::SocialMediaFeed(const std::string& systemId, const std::string& systemName)
    : systemId(systemId), systemName(systemName), version("1.0.0"), isRunning(false),
      totalUsers(0), totalPosts(0), totalComments(0), totalNotifications(0),
      maxPostLength(280), maxCommentLength(1000), maxHashtagsPerPost(30), maxMentionsPerPost(50),
      maxMediaPerPost(4), feedPageSize(20), notificationBatchSize(50),
      postRateLimitSeconds(10), commentRateLimitSeconds(5) {
}

SocialMediaFeed::~SocialMediaFeed() {
    stop();
}

// System management
bool SocialMediaFeed::start() {
    std::lock_guard<std::mutex> lock(systemMutex);
    if (isRunning) return false;
    
    isRunning = true;
    
    // Start background services
    notificationService = std::thread(&SocialMediaFeed::runNotificationService, this);
    trendingService = std::thread(&SocialMediaFeed::runTrendingService, this);
    moderationService = std::thread(&SocialMediaFeed::runModerationService, this);
    analyticsService = std::thread(&SocialMediaFeed::runAnalyticsService, this);
    
    return true;
}

void SocialMediaFeed::stop() {
    std::lock_guard<std::mutex> lock(systemMutex);
    if (!isRunning) return;
    
    isRunning = false;
    
    // Wait for background services to finish
    if (notificationService.joinable()) notificationService.join();
    if (trendingService.joinable()) trendingService.join();
    if (moderationService.joinable()) moderationService.join();
    if (analyticsService.joinable()) analyticsService.join();
}

bool SocialMediaFeed::getIsRunning() const {
    return isRunning;
}

std::string SocialMediaFeed::getSystemInfo() const {
    std::stringstream ss;
    ss << "SocialMediaFeed{"
       << "systemId='" << systemId << "', "
       << "systemName='" << systemName << "', "
       << "version='" << version << "', "
       << "running=" << (isRunning ? "true" : "false") << ", "
       << "users=" << totalUsers.load() << ", "
       << "posts=" << totalPosts.load() << ", "
       << "comments=" << totalComments.load() << ", "
       << "notifications=" << totalNotifications.load()
       << "}";
    return ss.str();
}

// User management
std::shared_ptr<User> SocialMediaFeed::createUser(const std::string& username, const std::string& email) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    // Check if username already exists
    for (const auto& pair : users) {
        if (pair.second->getUsername() == username) {
            return nullptr; // Username already exists
        }
    }
    
    std::string userId = generateId();
    auto user = std::make_shared<User>(userId, username, email);
    users[userId] = user;
    totalUsers++;
    
    return user;
}

std::shared_ptr<User> SocialMediaFeed::getUser(const std::string& userId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    auto it = users.find(userId);
    return it != users.end() ? it->second : nullptr;
}

std::shared_ptr<User> SocialMediaFeed::getUserByUsername(const std::string& username) {
    std::lock_guard<std::mutex> lock(systemMutex);
    for (const auto& pair : users) {
        if (pair.second->getUsername() == username) {
            return pair.second;
        }
    }
    return nullptr;
}

bool SocialMediaFeed::deleteUser(const std::string& userId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    auto it = users.find(userId);
    if (it == users.end()) return false;
    
    users.erase(it);
    totalUsers--;
    return true;
}

std::vector<std::shared_ptr<User>> SocialMediaFeed::searchUsers(const std::string& query) {
    std::lock_guard<std::mutex> lock(systemMutex);
    std::vector<std::shared_ptr<User>> results;
    
    for (const auto& pair : users) {
        const auto& user = pair.second;
        if (user->getUsername().find(query) != std::string::npos ||
            user->getDisplayName().find(query) != std::string::npos ||
            user->getBio().find(query) != std::string::npos) {
            results.push_back(user);
        }
    }
    
    return results;
}

// Post management
std::shared_ptr<Post> SocialMediaFeed::createPost(const std::string& authorId, const std::string& content, PostType type) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    // Validate user exists
    auto user = getUser(authorId);
    if (!user) return nullptr;
    
    // Check rate limiting
    if (!canUserPost(authorId)) return nullptr;
    
    // Validate content
    if (!isValidPostContent(content)) return nullptr;
    
    std::string postId = generateId();
    auto post = std::make_shared<Post>(postId, authorId, content, type);
    
    posts[postId] = post;
    user->addPost(post);
    totalPosts++;
    
    // Update user activity
    updateUserActivity(authorId, "post");
    
    // Process hashtags and mentions
    processHashtags(post);
    processMentions(post);
    
    // Update feeds
    updateUserFeed(authorId, post);
    
    return post;
}

std::shared_ptr<Post> SocialMediaFeed::getPost(const std::string& postId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    auto it = posts.find(postId);
    return it != posts.end() ? it->second : nullptr;
}

bool SocialMediaFeed::deletePost(const std::string& postId, const std::string& authorId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    auto post = getPost(postId);
    if (!post || post->getAuthorId() != authorId) return false;
    
    // Remove from user's posts
    auto user = getUser(authorId);
    if (user) {
        user->removePost(postId);
    }
    
    posts.erase(postId);
    totalPosts--;
    
    return true;
}

std::vector<std::shared_ptr<Post>> SocialMediaFeed::searchPosts(const std::string& query) {
    std::lock_guard<std::mutex> lock(systemMutex);
    std::vector<std::shared_ptr<Post>> results;
    
    for (const auto& pair : posts) {
        const auto& post = pair.second;
        if (post->getContent().find(query) != std::string::npos) {
            results.push_back(post);
        }
    }
    
    return results;
}

std::vector<std::shared_ptr<Post>> SocialMediaFeed::getPostsByUser(const std::string& userId) {
    auto user = getUser(userId);
    return user ? user->getPosts() : std::vector<std::shared_ptr<Post>>();
}

std::vector<std::shared_ptr<Post>> SocialMediaFeed::getPostsByHashtag(const std::string& hashtag) {
    std::lock_guard<std::mutex> lock(systemMutex);
    std::vector<std::shared_ptr<Post>> results;
    
    for (const auto& pair : posts) {
        const auto& post = pair.second;
        if (post->containsHashtag(hashtag)) {
            results.push_back(post);
        }
    }
    
    return results;
}

// Feed management
std::shared_ptr<Feed> SocialMediaFeed::createFeed(const std::string& userId, FeedType type) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    std::string feedId = generateId();
    auto feed = std::make_shared<Feed>(feedId, userId, type);
    feeds[feedId] = feed;
    
    return feed;
}

std::shared_ptr<Feed> SocialMediaFeed::getFeed(const std::string& feedId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    auto it = feeds.find(feedId);
    return it != feeds.end() ? it->second : nullptr;
}

std::vector<std::shared_ptr<Post>> SocialMediaFeed::getFeedPosts(const std::string& feedId, int page) {
    auto feed = getFeed(feedId);
    return feed ? feed->getPosts(page) : std::vector<std::shared_ptr<Post>>();
}

void SocialMediaFeed::refreshFeed(const std::string& feedId) {
    auto feed = getFeed(feedId);
    if (feed) {
        feed->refreshFeed();
    }
}

// Comment management
std::shared_ptr<Comment> SocialMediaFeed::createComment(const std::string& postId, const std::string& authorId, const std::string& content) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    // Validate post exists
    auto post = getPost(postId);
    if (!post) return nullptr;
    
    // Check rate limiting
    if (!canUserComment(authorId)) return nullptr;
    
    // Validate content
    if (!isValidCommentContent(content)) return nullptr;
    
    std::string commentId = generateId();
    auto comment = std::make_shared<Comment>(commentId, postId, authorId, content);
    
    comments[commentId] = comment;
    post->addComment(comment);
    totalComments++;
    
    // Update user activity
    updateUserActivity(authorId, "comment");
    
    return comment;
}

std::shared_ptr<Comment> SocialMediaFeed::getComment(const std::string& commentId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    auto it = comments.find(commentId);
    return it != comments.end() ? it->second : nullptr;
}

bool SocialMediaFeed::deleteComment(const std::string& commentId, const std::string& authorId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    auto comment = getComment(commentId);
    if (!comment || comment->getAuthorId() != authorId) return false;
    
    // Remove from post
    auto post = getPost(comment->getPostId());
    if (post) {
        post->removeComment(commentId);
    }
    
    comments.erase(commentId);
    totalComments--;
    
    return true;
}

std::vector<std::shared_ptr<Comment>> SocialMediaFeed::getCommentsForPost(const std::string& postId) {
    auto post = getPost(postId);
    return post ? post->getComments() : std::vector<std::shared_ptr<Comment>>();
}

// Social interactions
bool SocialMediaFeed::likePost(const std::string& postId, const std::string& userId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    auto post = getPost(postId);
    if (!post) return false;
    
    if (post->likePost(userId)) {
        // Create notification for post author
        auto notification = Notification::createLikeNotification(post->getAuthorId(), userId, postId);
        if (notification) {
            auto author = getUser(post->getAuthorId());
            if (author) {
                author->addNotification(notification);
                totalNotifications++;
            }
        }
        return true;
    }
    return false;
}

bool SocialMediaFeed::unlikePost(const std::string& postId, const std::string& userId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    auto post = getPost(postId);
    return post ? post->unlikePost(userId) : false;
}

bool SocialMediaFeed::retweetPost(const std::string& postId, const std::string& userId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    auto post = getPost(postId);
    if (!post) return false;
    
    if (post->retweetPost(userId)) {
        // Create notification for post author
        auto notification = Notification::createRetweetNotification(post->getAuthorId(), userId, postId);
        if (notification) {
            auto author = getUser(post->getAuthorId());
            if (author) {
                author->addNotification(notification);
                totalNotifications++;
            }
        }
        return true;
    }
    return false;
}

bool SocialMediaFeed::unretweetPost(const std::string& postId, const std::string& userId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    auto post = getPost(postId);
    return post ? post->unretweetPost(userId) : false;
}

bool SocialMediaFeed::bookmarkPost(const std::string& postId, const std::string& userId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    auto post = getPost(postId);
    if (!post) return false;
    
    auto user = getUser(userId);
    if (!user) return false;
    
    user->bookmarkPost(post);
    return true;
}

bool SocialMediaFeed::unbookmarkPost(const std::string& postId, const std::string& userId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    auto user = getUser(userId);
    if (!user) return false;
    
    user->unbookmarkPost(postId);
    return true;
}

bool SocialMediaFeed::followUser(const std::string& followerId, const std::string& followeeId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    auto follower = getUser(followerId);
    auto followee = getUser(followeeId);
    if (!follower || !followee) return false;
    
    if (follower->followUser(followeeId)) {
        // Add to followee's followers list
        followee->getFollowers().insert(followerId);
        
        // Create notification
        auto notification = Notification::createFollowNotification(followeeId, followerId);
        if (notification) {
            followee->addNotification(notification);
            totalNotifications++;
        }
        return true;
    }
    return false;
}

bool SocialMediaFeed::unfollowUser(const std::string& followerId, const std::string& followeeId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    auto follower = getUser(followerId);
    auto followee = getUser(followeeId);
    if (!follower || !followee) return false;
    
    if (follower->unfollowUser(followeeId)) {
        // Remove from followee's followers list
        followee->getFollowers().erase(followerId);
        return true;
    }
    return false;
}

// Notification management
std::shared_ptr<Notification> SocialMediaFeed::createNotification(const std::string& userId, const std::string& senderId, 
                                                                 NotificationType type, const std::string& title, const std::string& message) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    std::string notificationId = generateId();
    auto notification = std::make_shared<Notification>(notificationId, userId, senderId, type, title, message);
    notifications[notificationId] = notification;
    totalNotifications++;
    
    return notification;
}

std::vector<std::shared_ptr<Notification>> SocialMediaFeed::getUserNotifications(const std::string& userId) {
    auto user = getUser(userId);
    return user ? user->getNotifications() : std::vector<std::shared_ptr<Notification>>();
}

std::vector<std::shared_ptr<Notification>> SocialMediaFeed::getUnreadNotifications(const std::string& userId) {
    auto user = getUser(userId);
    return user ? user->getUnreadNotifications() : std::vector<std::shared_ptr<Notification>>();
}

bool SocialMediaFeed::markNotificationAsRead(const std::string& notificationId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    auto it = notifications.find(notificationId);
    if (it == notifications.end()) return false;
    
    it->second->markAsRead();
    return true;
}

void SocialMediaFeed::sendNotification(const std::string& userId, const std::string& message) {
    auto notification = createNotification(userId, "system", NotificationType::SYSTEM, "System Message", message);
}

// Hashtag management
std::shared_ptr<Hashtag> SocialMediaFeed::getOrCreateHashtag(const std::string& text) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    // Remove # if present
    std::string cleanText = text;
    if (!cleanText.empty() && cleanText[0] == '#') {
        cleanText = cleanText.substr(1);
    }
    
    // Check if hashtag already exists
    for (const auto& pair : hashtags) {
        if (pair.second->getText() == cleanText) {
            return pair.second;
        }
    }
    
    // Create new hashtag
    std::string hashtagId = generateId();
    auto hashtag = std::make_shared<Hashtag>(hashtagId, cleanText);
    hashtags[hashtagId] = hashtag;
    
    return hashtag;
}

std::shared_ptr<Hashtag> SocialMediaFeed::getHashtag(const std::string& hashtagId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    auto it = hashtags.find(hashtagId);
    return it != hashtags.end() ? it->second : nullptr;
}

std::vector<std::shared_ptr<Hashtag>> SocialMediaFeed::getTrendingHashtags() {
    std::lock_guard<std::mutex> lock(systemMutex);
    std::vector<std::shared_ptr<Hashtag>> trending;
    
    for (const auto& pair : hashtags) {
        if (pair.second->getIsTrending()) {
            trending.push_back(pair.second);
        }
    }
    
    // Sort by trending rank
    std::sort(trending.begin(), trending.end(),
        [](const std::shared_ptr<Hashtag>& a, const std::shared_ptr<Hashtag>& b) {
            return a->getTrendingRank() < b->getTrendingRank();
        });
    
    return trending;
}

std::vector<std::shared_ptr<Hashtag>> SocialMediaFeed::searchHashtags(const std::string& query) {
    std::lock_guard<std::mutex> lock(systemMutex);
    std::vector<std::shared_ptr<Hashtag>> results;
    
    for (const auto& pair : hashtags) {
        const auto& hashtag = pair.second;
        if (hashtag->getText().find(query) != std::string::npos) {
            results.push_back(hashtag);
        }
    }
    
    return results;
}

// Media management
std::shared_ptr<Media> SocialMediaFeed::uploadMedia(const std::string& url, MediaType type) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    std::string mediaId = generateId();
    auto media = std::make_shared<Media>(mediaId, url, type);
    this->media[mediaId] = media;
    
    return media;
}

std::shared_ptr<Media> SocialMediaFeed::getMedia(const std::string& mediaId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    auto it = media.find(mediaId);
    return it != media.end() ? it->second : nullptr;
}

bool SocialMediaFeed::deleteMedia(const std::string& mediaId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    auto it = media.find(mediaId);
    if (it == media.end()) return false;
    
    media.erase(it);
    return true;
}

// Search functionality
std::vector<std::shared_ptr<Post>> SocialMediaFeed::searchContent(const std::string& query) {
    return searchPosts(query);
}

std::vector<std::shared_ptr<User>> SocialMediaFeed::searchUsersByLocation(const std::string& location) {
    // This would search users by location if location data was stored
    // For now, return empty vector
    return std::vector<std::shared_ptr<User>>();
}

std::vector<std::shared_ptr<Post>> SocialMediaFeed::getTrendingPosts() {
    std::lock_guard<std::mutex> lock(systemMutex);
    std::vector<std::shared_ptr<Post>> trending;
    
    for (const auto& pair : posts) {
        const auto& post = pair.second;
        int engagement = post->getLikeCount() + post->getRetweetCount() * 2 + post->getCommentCount() * 3;
        if (engagement > 10) { // Threshold for trending
            trending.push_back(post);
        }
    }
    
    // Sort by engagement
    std::sort(trending.begin(), trending.end(),
        [](const std::shared_ptr<Post>& a, const std::shared_ptr<Post>& b) {
            int engagementA = a->getLikeCount() + a->getRetweetCount() * 2 + a->getCommentCount() * 3;
            int engagementB = b->getLikeCount() + b->getRetweetCount() * 2 + b->getCommentCount() * 3;
            return engagementA > engagementB;
        });
    
    return trending;
}

std::vector<std::shared_ptr<Post>> SocialMediaFeed::getRecommendedPosts(const std::string& userId) {
    // This would implement recommendation algorithm
    // For now, return trending posts
    return getTrendingPosts();
}

// Analytics and statistics
std::map<std::string, int> SocialMediaFeed::getSystemStatistics() const {
    std::map<std::string, int> stats;
    stats["total_users"] = totalUsers.load();
    stats["total_posts"] = totalPosts.load();
    stats["total_comments"] = totalComments.load();
    stats["total_notifications"] = totalNotifications.load();
    return stats;
}

std::map<std::string, int> SocialMediaFeed::getUserStatistics(const std::string& userId) const {
    std::map<std::string, int> stats;
    auto user = const_cast<SocialMediaFeed*>(this)->getUser(userId);
    if (user) {
        stats["followers"] = user->getFollowerCount();
        stats["following"] = user->getFollowingCount();
        stats["posts"] = user->getPostCount();
        stats["likes"] = user->getLikeCount();
    }
    return stats;
}

std::map<std::string, int> SocialMediaFeed::getPostStatistics(const std::string& postId) const {
    std::map<std::string, int> stats;
    auto post = const_cast<SocialMediaFeed*>(this)->getPost(postId);
    if (post) {
        stats["likes"] = post->getLikeCount();
        stats["retweets"] = post->getRetweetCount();
        stats["comments"] = post->getCommentCount();
        stats["bookmarks"] = post->getBookmarkCount();
        stats["views"] = post->getViewCount();
        stats["shares"] = post->getShareCount();
    }
    return stats;
}

std::vector<std::string> SocialMediaFeed::getTopUsers() const {
    std::vector<std::pair<std::string, int>> userScores;
    
    for (const auto& pair : const_cast<SocialMediaFeed*>(this)->users) {
        const auto& user = pair.second;
        int score = user->getFollowerCount() + user->getPostCount() * 10;
        userScores.push_back({user->getUserId(), score});
    }
    
    std::sort(userScores.begin(), userScores.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    std::vector<std::string> topUsers;
    for (const auto& pair : userScores) {
        topUsers.push_back(pair.first);
    }
    
    return topUsers;
}

std::vector<std::string> SocialMediaFeed::getTopPosts() const {
    std::vector<std::pair<std::string, int>> postScores;
    
    for (const auto& pair : const_cast<SocialMediaFeed*>(this)->posts) {
        const auto& post = pair.second;
        int score = post->getLikeCount() + post->getRetweetCount() * 2 + post->getCommentCount() * 3;
        postScores.push_back({post->getPostId(), score});
    }
    
    std::sort(postScores.begin(), postScores.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    std::vector<std::string> topPosts;
    for (const auto& pair : postScores) {
        topPosts.push_back(pair.first);
    }
    
    return topPosts;
}

std::vector<std::string> SocialMediaFeed::getTopHashtags() const {
    std::vector<std::pair<std::string, int>> hashtagScores;
    
    for (const auto& pair : const_cast<SocialMediaFeed*>(this)->hashtags) {
        const auto& hashtag = pair.second;
        int score = hashtag->getPostCount() + hashtag->getFollowerCount() * 5;
        hashtagScores.push_back({hashtag->getHashtagId(), score});
    }
    
    std::sort(hashtagScores.begin(), hashtagScores.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    std::vector<std::string> topHashtags;
    for (const auto& pair : hashtagScores) {
        topHashtags.push_back(pair.first);
    }
    
    return topHashtags;
}

// Moderation
bool SocialMediaFeed::reportPost(const std::string& postId, const std::string& reporterId, const std::string& reason) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    auto post = getPost(postId);
    if (!post) return false;
    
    post->reportPost();
    return true;
}

bool SocialMediaFeed::reportUser(const std::string& userId, const std::string& reporterId, const std::string& reason) {
    // This would implement user reporting
    return true;
}

bool SocialMediaFeed::blockUser(const std::string& blockerId, const std::string& blockedId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    auto blocker = getUser(blockerId);
    return blocker ? blocker->blockUser(blockedId) : false;
}

bool SocialMediaFeed::unblockUser(const std::string& blockerId, const std::string& blockedId) {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    auto blocker = getUser(blockerId);
    return blocker ? blocker->unblockUser(blockedId) : false;
}

std::vector<std::shared_ptr<Post>> SocialMediaFeed::getReportedPosts() {
    std::lock_guard<std::mutex> lock(systemMutex);
    std::vector<std::shared_ptr<Post>> reported;
    
    for (const auto& pair : posts) {
        if (pair.second->getReportCount() > 0) {
            reported.push_back(pair.second);
        }
    }
    
    return reported;
}

std::vector<std::shared_ptr<User>> SocialMediaFeed::getReportedUsers() {
    // This would return reported users if user reporting was implemented
    return std::vector<std::shared_ptr<User>>();
}

// Rate limiting
bool SocialMediaFeed::canUserPost(const std::string& userId) const {
    auto it = userLastPost.find(userId);
    if (it == userLastPost.end()) return true;
    
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - it->second);
    return duration.count() >= postRateLimitSeconds;
}

bool SocialMediaFeed::canUserComment(const std::string& userId) const {
    auto it = userLastComment.find(userId);
    if (it == userLastComment.end()) return true;
    
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - it->second);
    return duration.count() >= commentRateLimitSeconds;
}

void SocialMediaFeed::updateUserActivity(const std::string& userId, const std::string& activityType) {
    auto now = std::chrono::system_clock::now();
    
    if (activityType == "post") {
        userLastPost[userId] = now;
    } else if (activityType == "comment") {
        userLastComment[userId] = now;
    }
}

// Utility methods
std::string SocialMediaFeed::generateId() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 999999);
    
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    
    std::stringstream ss;
    ss << timestamp << "_" << std::setfill('0') << std::setw(6) << dis(gen);
    return ss.str();
}

bool SocialMediaFeed::isValidPostContent(const std::string& content) const {
    return content.length() <= maxPostLength && !content.empty();
}

bool SocialMediaFeed::isValidCommentContent(const std::string& content) const {
    return content.length() <= maxCommentLength && !content.empty();
}

std::vector<std::string> SocialMediaFeed::extractHashtags(const std::string& content) const {
    std::vector<std::string> hashtags;
    std::regex hashtagRegex("#\\w+");
    
    auto words_begin = std::sregex_iterator(content.begin(), content.end(), hashtagRegex);
    auto words_end = std::sregex_iterator();
    
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::string hashtag = (*i).str();
        hashtags.push_back(hashtag);
    }
    
    return hashtags;
}

std::vector<std::string> SocialMediaFeed::extractMentions(const std::string& content) const {
    std::vector<std::string> mentions;
    std::regex mentionRegex("@\\w+");
    
    auto words_begin = std::sregex_iterator(content.begin(), content.end(), mentionRegex);
    auto words_end = std::sregex_iterator();
    
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::string mention = (*i).str();
        mentions.push_back(mention);
    }
    
    return mentions;
}

void SocialMediaFeed::logActivity(const std::string& userId, const std::string& action, const std::string& details) {
    // This would log user activity for analytics
    // Implementation would depend on the logging system used
}

// Private helper methods
void SocialMediaFeed::runNotificationService() {
    while (isRunning) {
        processNotifications();
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }
}

void SocialMediaFeed::runTrendingService() {
    while (isRunning) {
        updateTrendingTopics();
        std::this_thread::sleep_for(std::chrono::minutes(5));
    }
}

void SocialMediaFeed::runModerationService() {
    while (isRunning) {
        moderateContent();
        std::this_thread::sleep_for(std::chrono::minutes(10));
    }
}

void SocialMediaFeed::runAnalyticsService() {
    while (isRunning) {
        generateAnalytics();
        std::this_thread::sleep_for(std::chrono::minutes(15));
    }
}

void SocialMediaFeed::processNotifications() {
    // Process pending notifications
}

void SocialMediaFeed::updateTrendingTopics() {
    // Update trending hashtags and topics
}

void SocialMediaFeed::moderateContent() {
    // Moderate reported content
}

void SocialMediaFeed::generateAnalytics() {
    // Generate analytics reports
}

void SocialMediaFeed::cleanupExpiredData() {
    // Clean up old data
}

void SocialMediaFeed::validateUserPermissions(const std::string& userId, const std::string& action) {
    // Validate user permissions for actions
}

void SocialMediaFeed::notifyFollowers(const std::string& userId, const std::string& action, const std::string& postId) {
    // Notify followers of user actions
}

void SocialMediaFeed::updateUserFeed(const std::string& userId, std::shared_ptr<Post> post) {
    // Update user's feed with new post
}

void SocialMediaFeed::processHashtags(std::shared_ptr<Post> post) {
    auto hashtags = extractHashtags(post->getContent());
    for (const auto& hashtagText : hashtags) {
        auto hashtag = getOrCreateHashtag(hashtagText);
        if (hashtag) {
            hashtag->addPost(post);
            post->addHashtag(hashtag);
        }
    }
}

void SocialMediaFeed::processMentions(std::shared_ptr<Post> post) {
    auto mentions = extractMentions(post->getContent());
    for (const auto& mention : mentions) {
        post->addMention(mention);
    }
}

std::vector<std::shared_ptr<Post>> SocialMediaFeed::getPostsForFeed(const std::string& userId, FeedType type) {
    // Get posts for specific feed type
    return std::vector<std::shared_ptr<Post>>();
}

double SocialMediaFeed::calculatePostRelevance(std::shared_ptr<Post> post, const std::string& userId) const {
    // Calculate post relevance for user
    return 0.0;
}

void SocialMediaFeed::updatePostScores() {
    // Update post relevance scores
}

void SocialMediaFeed::updateUserScores() {
    // Update user influence scores
}

void SocialMediaFeed::updateHashtagScores() {
    // Update hashtag trending scores
}
