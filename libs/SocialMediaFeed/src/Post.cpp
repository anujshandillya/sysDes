#include "../include/Post.hpp"
#include "../include/Comment.hpp"
#include "../include/Hashtag.hpp"
#include "../include/Media.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>

Post::Post(const std::string& postId, const std::string& authorId, const std::string& content, PostType type)
    : postId(postId), authorId(authorId), content(content), type(type), status(PostStatus::ACTIVE),
      likeCount(0), retweetCount(0), commentCount(0), bookmarkCount(0), viewCount(0), shareCount(0),
      isPublic(true), isSensitive(false), reportCount(0) {
    createdAt = std::chrono::system_clock::now();
    updatedAt = createdAt;
}

Post::~Post() = default;

// Basic getters and setters implementation
std::string Post::getPostId() const { return postId; }
std::string Post::getAuthorId() const { return authorId; }
std::string Post::getContent() const { return content; }
PostType Post::getType() const { return type; }
PostStatus Post::getStatus() const { return status; }
std::chrono::system_clock::time_point Post::getCreatedAt() const { return createdAt; }
std::chrono::system_clock::time_point Post::getUpdatedAt() const { return updatedAt; }

// Engagement metrics
int Post::getLikeCount() const { return likeCount; }
int Post::getRetweetCount() const { return retweetCount; }
int Post::getCommentCount() const { return commentCount; }
int Post::getBookmarkCount() const { return bookmarkCount; }
int Post::getViewCount() const { return viewCount; }
int Post::getShareCount() const { return shareCount; }

// Content getters
std::vector<std::shared_ptr<Media>> Post::getMedia() const { return media; }
std::vector<std::shared_ptr<Hashtag>> Post::getHashtags() const { return hashtags; }
std::vector<std::string> Post::getMentions() const { return mentions; }
std::string Post::getLocation() const { return location; }
std::string Post::getLanguage() const { return language; }

// Social interaction getters
std::set<std::string> Post::getLikedBy() const { return likedBy; }
std::set<std::string> Post::getRetweetedBy() const { return retweetedBy; }
std::set<std::string> Post::getBookmarkedBy() const { return bookmarkedBy; }
std::vector<std::shared_ptr<Comment>> Post::getComments() const { return comments; }

// Retweet/Quote getters
std::shared_ptr<Post> Post::getOriginalPost() const { return originalPost; }
std::string Post::getQuoteText() const { return quoteText; }

// Poll getters
std::map<std::string, int> Post::getPollOptions() const { return pollOptions; }
std::set<std::string> Post::getPollVoters() const { return pollVoters; }
std::chrono::system_clock::time_point Post::getPollEndTime() const { return pollEndTime; }

// Visibility getters
bool Post::getIsPublic() const { return isPublic; }
std::set<std::string> Post::getVisibleTo() const { return visibleTo; }
std::set<std::string> Post::getHiddenFrom() const { return hiddenFrom; }

// Moderation getters
bool Post::getIsSensitive() const { return isSensitive; }
std::vector<std::string> Post::getContentWarnings() const { return contentWarnings; }
int Post::getReportCount() const { return reportCount; }

// Basic setters
void Post::setContent(const std::string& newContent) { 
    content = newContent; 
    updatedAt = std::chrono::system_clock::now();
}
void Post::setStatus(PostStatus newStatus) { status = newStatus; }
void Post::setLocation(const std::string& newLocation) { location = newLocation; }
void Post::setLanguage(const std::string& newLanguage) { language = newLanguage; }
void Post::setIsPublic(bool pub) { isPublic = pub; }
void Post::setIsSensitive(bool sensitive) { isSensitive = sensitive; }

// Social interaction operations
bool Post::likePost(const std::string& userId) {
    if (likedBy.find(userId) != likedBy.end()) return false;
    likedBy.insert(userId);
    likeCount = likedBy.size();
    return true;
}

bool Post::unlikePost(const std::string& userId) {
    auto it = likedBy.find(userId);
    if (it == likedBy.end()) return false;
    likedBy.erase(it);
    likeCount = likedBy.size();
    return true;
}

bool Post::retweetPost(const std::string& userId) {
    if (retweetedBy.find(userId) != retweetedBy.end()) return false;
    retweetedBy.insert(userId);
    retweetCount = retweetedBy.size();
    return true;
}

bool Post::unretweetPost(const std::string& userId) {
    auto it = retweetedBy.find(userId);
    if (it == retweetedBy.end()) return false;
    retweetedBy.erase(it);
    retweetCount = retweetedBy.size();
    return true;
}

bool Post::bookmarkPost(const std::string& userId) {
    if (bookmarkedBy.find(userId) != bookmarkedBy.end()) return false;
    bookmarkedBy.insert(userId);
    bookmarkCount = bookmarkedBy.size();
    return true;
}

bool Post::unbookmarkPost(const std::string& userId) {
    auto it = bookmarkedBy.find(userId);
    if (it == bookmarkedBy.end()) return false;
    bookmarkedBy.erase(it);
    bookmarkCount = bookmarkedBy.size();
    return true;
}

void Post::addComment(std::shared_ptr<Comment> comment) {
    comments.push_back(comment);
    commentCount = comments.size();
}

void Post::removeComment(const std::string& commentId) {
    comments.erase(std::remove_if(comments.begin(), comments.end(),
        [&commentId](const std::shared_ptr<Comment>& comment) {
            return comment->getCommentId() == commentId;
        }), comments.end());
    commentCount = comments.size();
}

void Post::incrementViewCount() { viewCount++; }
void Post::incrementShareCount() { shareCount++; }

// Content operations
void Post::addMedia(std::shared_ptr<Media> mediaItem) { media.push_back(mediaItem); }
void Post::removeMedia(const std::string& mediaId) {
    media.erase(std::remove_if(media.begin(), media.end(),
        [&mediaId](const std::shared_ptr<Media>& mediaItem) {
            return mediaItem->getMediaId() == mediaId;
        }), media.end());
}

void Post::addHashtag(std::shared_ptr<Hashtag> hashtag) { hashtags.push_back(hashtag); }
void Post::removeHashtag(const std::string& hashtagText) {
    hashtags.erase(std::remove_if(hashtags.begin(), hashtags.end(),
        [&hashtagText](const std::shared_ptr<Hashtag>& hashtag) {
            return hashtag->getText() == hashtagText;
        }), hashtags.end());
}

void Post::addMention(const std::string& username) { mentions.push_back(username); }
void Post::removeMention(const std::string& username) {
    mentions.erase(std::remove(mentions.begin(), mentions.end(), username), mentions.end());
}

// Poll operations
void Post::addPollOption(const std::string& option) { pollOptions[option] = 0; }

bool Post::voteInPoll(const std::string& userId, const std::string& option) {
    if (pollVoters.find(userId) != pollVoters.end()) return false; // Already voted
    if (pollOptions.find(option) == pollOptions.end()) return false; // Invalid option
    
    pollVoters.insert(userId);
    pollOptions[option]++;
    return true;
}

void Post::setPollEndTime(const std::chrono::system_clock::time_point& endTime) { pollEndTime = endTime; }

bool Post::isPollActive() const {
    return std::chrono::system_clock::now() < pollEndTime;
}

std::string Post::getWinningPollOption() const {
    if (pollOptions.empty()) return "";
    
    auto maxVotes = std::max_element(pollOptions.begin(), pollOptions.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });
    
    return maxVotes->first;
}

// Visibility operations
void Post::addVisibleTo(const std::string& userId) { visibleTo.insert(userId); }
void Post::removeVisibleTo(const std::string& userId) { visibleTo.erase(userId); }
void Post::addHiddenFrom(const std::string& userId) { hiddenFrom.insert(userId); }
void Post::removeHiddenFrom(const std::string& userId) { hiddenFrom.erase(userId); }

// Moderation operations
void Post::addContentWarning(const std::string& warning) { contentWarnings.push_back(warning); }
void Post::removeContentWarning(const std::string& warning) {
    contentWarnings.erase(std::remove(contentWarnings.begin(), contentWarnings.end(), warning), contentWarnings.end());
}

void Post::reportPost() { reportCount++; }
void Post::clearReports() { reportCount = 0; }

// Queries
bool Post::isLikedBy(const std::string& userId) const {
    return likedBy.find(userId) != likedBy.end();
}

bool Post::isRetweetedBy(const std::string& userId) const {
    return retweetedBy.find(userId) != retweetedBy.end();
}

bool Post::isBookmarkedBy(const std::string& userId) const {
    return bookmarkedBy.find(userId) != bookmarkedBy.end();
}

bool Post::isVisibleTo(const std::string& userId) const {
    if (!isPublic) return visibleTo.find(userId) != visibleTo.end();
    return hiddenFrom.find(userId) == hiddenFrom.end();
}

bool Post::isHiddenFrom(const std::string& userId) const {
    return hiddenFrom.find(userId) != hiddenFrom.end();
}

bool Post::hasVotedInPoll(const std::string& userId) const {
    return pollVoters.find(userId) != pollVoters.end();
}

bool Post::containsHashtag(const std::string& hashtagText) const {
    return std::any_of(hashtags.begin(), hashtags.end(),
        [&hashtagText](const std::shared_ptr<Hashtag>& hashtag) {
            return hashtag->getText() == hashtagText;
        });
}

bool Post::containsMention(const std::string& username) const {
    return std::find(mentions.begin(), mentions.end(), username) != mentions.end();
}

// Utility methods
std::string Post::toString() const {
    std::stringstream ss;
    ss << "Post{"
       << "postId='" << postId << "', "
       << "authorId='" << authorId << "', "
       << "content='" << content.substr(0, 50) << "...', "
       << "type=" << static_cast<int>(type) << ", "
       << "likes=" << likeCount << ", "
       << "retweets=" << retweetCount << ", "
       << "comments=" << commentCount
       << "}";
    return ss.str();
}

bool Post::isValid() const {
    return !postId.empty() && !authorId.empty() && !content.empty();
}

void Post::updateStatistics() {
    likeCount = likedBy.size();
    retweetCount = retweetedBy.size();
    commentCount = comments.size();
    bookmarkCount = bookmarkedBy.size();
}

std::string Post::getFormattedTimestamp() const {
    auto time_t = std::chrono::system_clock::to_time_t(createdAt);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

int Post::getAgeInMinutes() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(now - createdAt);
    return duration.count();
}

int Post::getAgeInHours() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::hours>(now - createdAt);
    return duration.count();
}

int Post::getAgeInDays() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::hours>(now - createdAt);
    return duration.count() / 24;
} 