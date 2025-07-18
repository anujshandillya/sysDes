#include "../include/Feed.hpp"
#include "../include/User.hpp"
#include "../include/Post.hpp"
#include "../include/Hashtag.hpp"
#include <algorithm>
#include <sstream>

Feed::Feed(const std::string& feedId, const std::string& userId, FeedType type)
    : feedId(feedId), userId(userId), type(type), algorithm(FeedAlgorithm::CHRONOLOGICAL),
      pageSize(20), currentPage(0), hasMorePosts(true), cacheExpiryMinutes(30),
      includeRetweets(true), includeReplies(true), includeSensitiveContent(false) {
    lastRefresh = std::chrono::system_clock::now();
}

Feed::~Feed() = default;

// Getters
std::string Feed::getFeedId() const { return feedId; }
std::string Feed::getUserId() const { return userId; }
FeedType Feed::getType() const { return type; }
FeedAlgorithm Feed::getAlgorithm() const { return algorithm; }
int Feed::getPageSize() const { return pageSize; }
int Feed::getCurrentPage() const { return currentPage; }
bool Feed::getHasMorePosts() const { return hasMorePosts; }
std::string Feed::getLastPostId() const { return lastPostId; }

// Setters
void Feed::setAlgorithm(FeedAlgorithm newAlgorithm) { algorithm = newAlgorithm; }
void Feed::setPageSize(int size) { pageSize = size; }
void Feed::setCacheExpiry(int minutes) { cacheExpiryMinutes = minutes; }

// Feed operations
std::vector<std::shared_ptr<Post>> Feed::getPosts(int page) {
    std::lock_guard<std::mutex> lock(feedMutex);
    
    if (page == 0) {
        // Check cache first
        if (isCacheValid() && pageCache.find(0) != pageCache.end()) {
            return pageCache[0];
        }
        
        // Fetch fresh posts
        auto freshPosts = fetchPostsFromSource();
        auto filteredPosts = applyFilters(freshPosts);
        auto algorithmPosts = applyAlgorithm(filteredPosts);
        
        posts = algorithmPosts;
        updateCache(0, posts);
        currentPage = 0;
        hasMorePosts = posts.size() >= pageSize;
        
        return posts;
    } else {
        // Check cache for specific page
        if (pageCache.find(page) != pageCache.end()) {
            return pageCache[page];
        }
        
        // Calculate pagination
        int startIndex = page * pageSize;
        if (startIndex >= posts.size()) {
            hasMorePosts = false;
            return {};
        }
        
        int endIndex = std::min(startIndex + pageSize, static_cast<int>(posts.size()));
        std::vector<std::shared_ptr<Post>> pagePosts(posts.begin() + startIndex, posts.begin() + endIndex);
        
        updateCache(page, pagePosts);
        hasMorePosts = endIndex < posts.size();
        
        return pagePosts;
    }
}

std::vector<std::shared_ptr<Post>> Feed::getNextPage() {
    return getPosts(currentPage + 1);
}

std::vector<std::shared_ptr<Post>> Feed::refreshFeed() {
    std::lock_guard<std::mutex> lock(feedMutex);
    
    clearCache();
    lastRefresh = std::chrono::system_clock::now();
    
    auto freshPosts = fetchPostsFromSource();
    auto filteredPosts = applyFilters(freshPosts);
    auto algorithmPosts = applyAlgorithm(filteredPosts);
    
    posts = algorithmPosts;
    updateCache(0, posts);
    currentPage = 0;
    hasMorePosts = posts.size() >= pageSize;
    
    return posts;
}

void Feed::clearFeed() {
    std::lock_guard<std::mutex> lock(feedMutex);
    posts.clear();
    postScores.clear();
    seenPosts.clear();
    clearCache();
    currentPage = 0;
    hasMorePosts = false;
}

void Feed::addPost(std::shared_ptr<Post> post) {
    std::lock_guard<std::mutex> lock(feedMutex);
    posts.push_back(post);
    updatePostScores();
}

void Feed::removePost(const std::string& postId) {
    std::lock_guard<std::mutex> lock(feedMutex);
    posts.erase(std::remove_if(posts.begin(), posts.end(),
        [&postId](const std::shared_ptr<Post>& post) {
            return post->getPostId() == postId;
        }), posts.end());
    postScores.erase(postId);
}

void Feed::markPostAsSeen(const std::string& postId) {
    std::lock_guard<std::mutex> lock(feedMutex);
    seenPosts.insert(postId);
}

// Filtering operations
void Feed::excludeUser(const std::string& userId) {
    excludedUsers.insert(userId);
}

void Feed::includeUser(const std::string& userId) {
    excludedUsers.erase(userId);
}

void Feed::excludeHashtag(const std::string& hashtag) {
    excludedHashtags.insert(hashtag);
}

void Feed::includeHashtag(const std::string& hashtag) {
    excludedHashtags.erase(hashtag);
    includedHashtags.insert(hashtag);
}

void Feed::setIncludeRetweets(bool include) { includeRetweets = include; }
void Feed::setIncludeReplies(bool include) { includeReplies = include; }
void Feed::setIncludeSensitiveContent(bool include) { includeSensitiveContent = include; }

// Personalization operations
void Feed::setUserWeight(const std::string& userId, double weight) {
    userWeights[userId] = weight;
}

void Feed::setHashtagWeight(const std::string& hashtag, double weight) {
    hashtagWeights[hashtag] = weight;
}

void Feed::setTopicWeight(const std::string& topic, double weight) {
    topicWeights[topic] = weight;
}

void Feed::updateUserPreferences(const std::map<std::string, double>& preferences) {
    userWeights.insert(preferences.begin(), preferences.end());
}

// Algorithm operations
void Feed::switchToChronological() {
    algorithm = FeedAlgorithm::CHRONOLOGICAL;
    sortPostsByTime();
}

void Feed::switchToRelevance() {
    algorithm = FeedAlgorithm::RELEVANCE;
    sortPostsByScore();
}

void Feed::switchToEngagement() {
    algorithm = FeedAlgorithm::ENGAGEMENT;
    sortPostsByEngagement();
}

void Feed::switchToPersonalized() {
    algorithm = FeedAlgorithm::PERSONALIZED;
    sortPostsByScore();
}

void Feed::switchToTrending() {
    algorithm = FeedAlgorithm::TRENDING;
    sortPostsByScore();
}

// Scoring and ranking
double Feed::calculatePostScore(std::shared_ptr<Post> post) const {
    switch (algorithm) {
        case FeedAlgorithm::CHRONOLOGICAL:
            return static_cast<double>(post->getCreatedAt().time_since_epoch().count());
        case FeedAlgorithm::RELEVANCE:
            return calculateRelevanceScore(post);
        case FeedAlgorithm::ENGAGEMENT:
            return calculateEngagementScore(post);
        case FeedAlgorithm::PERSONALIZED:
            return calculatePersonalizationScore(post);
        case FeedAlgorithm::TRENDING:
            return calculateTrendingScore(post);
        default:
            return 0.0;
    }
}

void Feed::updatePostScores() {
    postScores.clear();
    for (const auto& post : posts) {
        postScores[post->getPostId()] = calculatePostScore(post);
    }
}

void Feed::sortPostsByScore() {
    std::sort(posts.begin(), posts.end(),
        [this](const std::shared_ptr<Post>& a, const std::shared_ptr<Post>& b) {
            return postScores[a->getPostId()] > postScores[b->getPostId()];
        });
}

void Feed::sortPostsByTime() {
    std::sort(posts.begin(), posts.end(),
        [](const std::shared_ptr<Post>& a, const std::shared_ptr<Post>& b) {
            return a->getCreatedAt() > b->getCreatedAt();
        });
}

void Feed::sortPostsByEngagement() {
    std::sort(posts.begin(), posts.end(),
        [](const std::shared_ptr<Post>& a, const std::shared_ptr<Post>& b) {
            int engagementA = a->getLikeCount() + a->getRetweetCount() * 2 + a->getCommentCount() * 3;
            int engagementB = b->getLikeCount() + b->getRetweetCount() * 2 + b->getCommentCount() * 3;
            return engagementA > engagementB;
        });
}

// Queries
bool Feed::hasSeenPost(const std::string& postId) const {
    return seenPosts.find(postId) != seenPosts.end();
}

bool Feed::isUserExcluded(const std::string& userId) const {
    return excludedUsers.find(userId) != excludedUsers.end();
}

bool Feed::isHashtagExcluded(const std::string& hashtag) const {
    return excludedHashtags.find(hashtag) != excludedHashtags.end();
}

bool Feed::isHashtagIncluded(const std::string& hashtag) const {
    return includedHashtags.find(hashtag) != includedHashtags.end();
}

bool Feed::shouldIncludePost(std::shared_ptr<Post> post) const {
    // Check if user is excluded
    if (isUserExcluded(post->getAuthorId())) return false;
    
    // Check if post is sensitive and we don't include sensitive content
    if (post->getIsSensitive() && !includeSensitiveContent) return false;
    
    // Check post type filters
    if (post->getType() == PostType::RETWEET && !includeRetweets) return false;
    if (post->getType() == PostType::TEXT && !includeReplies) return false;
    
    return true;
}

// Statistics
int Feed::getTotalPosts() const { return posts.size(); }
int Feed::getSeenPostsCount() const { return seenPosts.size(); }
int Feed::getUnseenPostsCount() const { return posts.size() - seenPosts.size(); }

double Feed::getAverageEngagement() const {
    if (posts.empty()) return 0.0;
    
    double totalEngagement = 0.0;
    for (const auto& post : posts) {
        totalEngagement += post->getLikeCount() + post->getRetweetCount() + post->getCommentCount();
    }
    return totalEngagement / posts.size();
}

std::map<std::string, int> Feed::getTopHashtags() const {
    std::map<std::string, int> hashtagCounts;
    for (const auto& post : posts) {
        for (const auto& hashtag : post->getHashtags()) {
            hashtagCounts[hashtag->getText()]++;
        }
    }
    return hashtagCounts;
}

std::map<std::string, int> Feed::getTopUsers() const {
    std::map<std::string, int> userCounts;
    for (const auto& post : posts) {
        userCounts[post->getAuthorId()]++;
    }
    return userCounts;
}

// Cache operations
void Feed::clearCache() {
    pageCache.clear();
}

void Feed::refreshCache() {
    clearCache();
    lastRefresh = std::chrono::system_clock::now();
}

bool Feed::isCacheValid() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(now - lastRefresh);
    return duration.count() < cacheExpiryMinutes;
}

void Feed::preloadNextPage() {
    if (hasMorePosts) {
        getPosts(currentPage + 1);
    }
}

// Utility methods
std::string Feed::toString() const {
    std::stringstream ss;
    ss << "Feed{"
       << "feedId='" << feedId << "', "
       << "userId='" << userId << "', "
       << "type=" << static_cast<int>(type) << ", "
       << "algorithm=" << static_cast<int>(algorithm) << ", "
       << "posts=" << posts.size() << ", "
       << "pageSize=" << pageSize << ", "
       << "hasMorePosts=" << (hasMorePosts ? "true" : "false")
       << "}";
    return ss.str();
}

bool Feed::isValid() const {
    return !feedId.empty() && !userId.empty();
}

void Feed::resetPagination() {
    currentPage = 0;
    hasMorePosts = true;
}

void Feed::optimizeForUser(const std::string& targetUserId) {
    // This would implement user-specific optimizations
    // For now, just update the user weights
    userWeights[targetUserId] = 1.5; // Give higher weight to target user
}

// Private helper methods
std::vector<std::shared_ptr<Post>> Feed::fetchPostsFromSource() {
    // This is a placeholder implementation
    // In a real system, this would fetch posts from the database or external API
    return posts;
}

std::vector<std::shared_ptr<Post>> Feed::applyFilters(const std::vector<std::shared_ptr<Post>>& inputPosts) {
    std::vector<std::shared_ptr<Post>> filteredPosts;
    for (const auto& post : inputPosts) {
        if (shouldIncludePost(post)) {
            filteredPosts.push_back(post);
        }
    }
    return filteredPosts;
}

std::vector<std::shared_ptr<Post>> Feed::applyAlgorithm(const std::vector<std::shared_ptr<Post>>& inputPosts) {
    std::vector<std::shared_ptr<Post>> algorithmPosts = inputPosts;
    
    switch (algorithm) {
        case FeedAlgorithm::CHRONOLOGICAL:
            std::sort(algorithmPosts.begin(), algorithmPosts.end(),
                [](const std::shared_ptr<Post>& a, const std::shared_ptr<Post>& b) {
                    return a->getCreatedAt() > b->getCreatedAt();
                });
            break;
        case FeedAlgorithm::RELEVANCE:
            std::sort(algorithmPosts.begin(), algorithmPosts.end(),
                [this](const std::shared_ptr<Post>& a, const std::shared_ptr<Post>& b) {
                    return calculateRelevanceScore(a) > calculateRelevanceScore(b);
                });
            break;
        case FeedAlgorithm::ENGAGEMENT:
            std::sort(algorithmPosts.begin(), algorithmPosts.end(),
                [](const std::shared_ptr<Post>& a, const std::shared_ptr<Post>& b) {
                    int engagementA = a->getLikeCount() + a->getRetweetCount() * 2 + a->getCommentCount() * 3;
                    int engagementB = b->getLikeCount() + b->getRetweetCount() * 2 + b->getCommentCount() * 3;
                    return engagementA > engagementB;
                });
            break;
        case FeedAlgorithm::PERSONALIZED:
            std::sort(algorithmPosts.begin(), algorithmPosts.end(),
                [this](const std::shared_ptr<Post>& a, const std::shared_ptr<Post>& b) {
                    return calculatePersonalizationScore(a) > calculatePersonalizationScore(b);
                });
            break;
        case FeedAlgorithm::TRENDING:
            std::sort(algorithmPosts.begin(), algorithmPosts.end(),
                [this](const std::shared_ptr<Post>& a, const std::shared_ptr<Post>& b) {
                    return calculateTrendingScore(a) > calculateTrendingScore(b);
                });
            break;
    }
    
    return algorithmPosts;
}

void Feed::updateCache(int page, const std::vector<std::shared_ptr<Post>>& pagePosts) {
    pageCache[page] = pagePosts;
}

double Feed::calculateRelevanceScore(std::shared_ptr<Post> post) const {
    double score = 0.0;
    
    // Base score from engagement
    score += post->getLikeCount() * 0.1;
    score += post->getRetweetCount() * 0.2;
    score += post->getCommentCount() * 0.3;
    
    // Recency bonus
    auto age = std::chrono::duration_cast<std::chrono::hours>(
        std::chrono::system_clock::now() - post->getCreatedAt()).count();
    score += std::max(0.0, 24.0 - age) * 0.1;
    
    return score;
}

double Feed::calculateEngagementScore(std::shared_ptr<Post> post) const {
    return post->getLikeCount() + post->getRetweetCount() * 2 + post->getCommentCount() * 3;
}

double Feed::calculatePersonalizationScore(std::shared_ptr<Post> post) const {
    double score = calculateRelevanceScore(post);
    
    // User weight bonus
    auto userWeightIt = userWeights.find(post->getAuthorId());
    if (userWeightIt != userWeights.end()) {
        score *= userWeightIt->second;
    }
    
    // Hashtag weight bonus
    for (const auto& hashtag : post->getHashtags()) {
        auto hashtagWeightIt = hashtagWeights.find(hashtag->getText());
        if (hashtagWeightIt != hashtagWeights.end()) {
            score += hashtagWeightIt->second * 0.5;
        }
    }
    
    return score;
}

double Feed::calculateTrendingScore(std::shared_ptr<Post> post) const {
    double score = 0.0;
    
    // Viral coefficient
    double viralCoeff = static_cast<double>(post->getRetweetCount()) / 
                       std::max(1, post->getLikeCount());
    score += viralCoeff * 10.0;
    
    // Velocity (engagement over time)
    auto age = std::chrono::duration_cast<std::chrono::hours>(
        std::chrono::system_clock::now() - post->getCreatedAt()).count();
    if (age > 0) {
        double velocity = static_cast<double>(post->getLikeCount() + post->getRetweetCount()) / age;
        score += velocity * 5.0;
    }
    
    return score;
}

bool Feed::isPostEligible(std::shared_ptr<Post> post) const {
    return post->getStatus() == PostStatus::ACTIVE && !post->getIsSensitive();
}

void Feed::logFeedActivity(const std::string& action, const std::string& details) {
    // This would log feed activity for analytics
    // Implementation would depend on the logging system used
}
