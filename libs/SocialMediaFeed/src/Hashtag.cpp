#include "../include/Hashtag.hpp"
#include "../include/Post.hpp"
#include <algorithm>
#include <sstream>

Hashtag::Hashtag(const std::string& hashtagId, const std::string& text)
    : hashtagId(hashtagId), text(text), postCount(0), followerCount(0), viewCount(0), trendScore(0),
      isTrending(false), trendingRank(0), isBlocked(false), isSensitive(false) {
    createdAt = std::chrono::system_clock::now();
    lastUsed = createdAt;
}

Hashtag::~Hashtag() = default;

// Getters
std::string Hashtag::getHashtagId() const { return hashtagId; }
std::string Hashtag::getText() const { return text; }
std::string Hashtag::getDescription() const { return description; }
std::chrono::system_clock::time_point Hashtag::getCreatedAt() const { return createdAt; }
std::chrono::system_clock::time_point Hashtag::getLastUsed() const { return lastUsed; }

// Statistics getters
int Hashtag::getPostCount() const { return postCount; }
int Hashtag::getFollowerCount() const { return followerCount; }
int Hashtag::getViewCount() const { return viewCount; }
int Hashtag::getTrendScore() const { return trendScore; }

// Content getters
std::vector<std::shared_ptr<Post>> Hashtag::getPosts() const { return posts; }
std::set<std::string> Hashtag::getFollowers() const { return followers; }

// Trending getters
bool Hashtag::getIsTrending() const { return isTrending; }
int Hashtag::getTrendingRank() const { return trendingRank; }
std::chrono::system_clock::time_point Hashtag::getTrendingStartTime() const { return trendingStartTime; }
std::chrono::system_clock::time_point Hashtag::getTrendingEndTime() const { return trendingEndTime; }

// Moderation getters
bool Hashtag::getIsBlocked() const { return isBlocked; }
bool Hashtag::getIsSensitive() const { return isSensitive; }
std::vector<std::string> Hashtag::getContentWarnings() const { return contentWarnings; }

// Setters
void Hashtag::setDescription(const std::string& desc) { description = desc; }
void Hashtag::setIsTrending(bool trending) { isTrending = trending; }
void Hashtag::setTrendingRank(int rank) { trendingRank = rank; }
void Hashtag::setIsBlocked(bool blocked) { isBlocked = blocked; }
void Hashtag::setIsSensitive(bool sensitive) { isSensitive = sensitive; }

// Content operations
void Hashtag::addPost(std::shared_ptr<Post> post) {
    if (std::find(posts.begin(), posts.end(), post) == posts.end()) {
        posts.push_back(post);
        postCount = posts.size();
        updateLastUsed();
    }
}

void Hashtag::removePost(const std::string& postId) {
    posts.erase(std::remove_if(posts.begin(), posts.end(),
        [&postId](const std::shared_ptr<Post>& post) {
            return post->getPostId() == postId;
        }), posts.end());
    postCount = posts.size();
}

void Hashtag::addFollower(const std::string& userId) {
    if (followers.find(userId) == followers.end()) {
        followers.insert(userId);
        followerCount = followers.size();
    }
}

void Hashtag::removeFollower(const std::string& userId) {
    auto it = followers.find(userId);
    if (it != followers.end()) {
        followers.erase(it);
        followerCount = followers.size();
    }
}

// Trending operations
void Hashtag::startTrending(int rank) {
    isTrending = true;
    trendingRank = rank;
    trendingStartTime = std::chrono::system_clock::now();
}

void Hashtag::stopTrending() {
    isTrending = false;
    trendingEndTime = std::chrono::system_clock::now();
}

void Hashtag::updateTrendScore(int score) {
    trendScore = score;
}

// Moderation operations
void Hashtag::addContentWarning(const std::string& warning) {
    if (std::find(contentWarnings.begin(), contentWarnings.end(), warning) == contentWarnings.end()) {
        contentWarnings.push_back(warning);
    }
}

void Hashtag::removeContentWarning(const std::string& warning) {
    contentWarnings.erase(std::remove(contentWarnings.begin(), contentWarnings.end(), warning), contentWarnings.end());
}

// Statistics operations
void Hashtag::incrementPostCount() { postCount++; }
void Hashtag::decrementPostCount() { postCount = std::max(0, postCount - 1); }
void Hashtag::incrementViewCount() { viewCount++; }
void Hashtag::updateLastUsed() { lastUsed = std::chrono::system_clock::now(); }

// Queries
bool Hashtag::hasFollower(const std::string& userId) const {
    return followers.find(userId) != followers.end();
}

bool Hashtag::containsPost(const std::string& postId) const {
    return std::any_of(posts.begin(), posts.end(),
        [&postId](const std::shared_ptr<Post>& post) {
            return post->getPostId() == postId;
        });
}

bool Hashtag::isCurrentlyTrending() const {
    return isTrending && std::chrono::system_clock::now() < trendingEndTime;
}

bool Hashtag::hasContentWarning(const std::string& warning) const {
    return std::find(contentWarnings.begin(), contentWarnings.end(), warning) != contentWarnings.end();
}

// Utility methods
std::string Hashtag::toString() const {
    std::stringstream ss;
    ss << "Hashtag{"
       << "hashtagId='" << hashtagId << "', "
       << "text='" << text << "', "
       << "posts=" << postCount << ", "
       << "followers=" << followerCount << ", "
       << "views=" << viewCount << ", "
       << "trending=" << (isTrending ? "true" : "false") << ", "
       << "rank=" << trendingRank
       << "}";
    return ss.str();
}

bool Hashtag::isValid() const {
    return !hashtagId.empty() && !text.empty();
}

void Hashtag::updateStatistics() {
    postCount = posts.size();
    followerCount = followers.size();
}

std::string Hashtag::getFormattedText() const {
    return "#" + text;
}

int Hashtag::getAgeInDays() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::hours>(now - createdAt);
    return duration.count() / 24;
}

double Hashtag::getTrendingDuration() const {
    if (!isTrending) return 0.0;
    
    auto endTime = (trendingEndTime.time_since_epoch().count() > 0) ? trendingEndTime : std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::hours>(endTime - trendingStartTime);
    return duration.count();
}
