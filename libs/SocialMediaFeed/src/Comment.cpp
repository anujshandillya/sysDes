#include "../include/Comment.hpp"
#include "../include/Post.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>

Comment::Comment(const std::string& commentId, const std::string& postId, const std::string& authorId, const std::string& content)
    : commentId(commentId), postId(postId), authorId(authorId), content(content), status(CommentStatus::ACTIVE),
      likeCount(0), replyCount(0), reportCount(0), isSensitive(false) {
    createdAt = std::chrono::system_clock::now();
    updatedAt = createdAt;
}

Comment::~Comment() = default;

// Getters
std::string Comment::getCommentId() const { return commentId; }
std::string Comment::getPostId() const { return postId; }
std::string Comment::getAuthorId() const { return authorId; }
std::string Comment::getContent() const { return content; }
CommentStatus Comment::getStatus() const { return status; }
std::chrono::system_clock::time_point Comment::getCreatedAt() const { return createdAt; }
std::chrono::system_clock::time_point Comment::getUpdatedAt() const { return updatedAt; }

// Engagement metrics
int Comment::getLikeCount() const { return likeCount; }
int Comment::getReplyCount() const { return replyCount; }
int Comment::getReportCount() const { return reportCount; }

// Social interaction getters
std::set<std::string> Comment::getLikedBy() const { return likedBy; }
std::vector<std::shared_ptr<Comment>> Comment::getReplies() const { return replies; }

// Parent comment getters
std::shared_ptr<Comment> Comment::getParentComment() const { return parentComment; }

// Content getters
std::vector<std::string> Comment::getMentions() const { return mentions; }
std::vector<std::string> Comment::getHashtags() const { return hashtags; }

// Moderation getters
bool Comment::getIsSensitive() const { return isSensitive; }
std::vector<std::string> Comment::getContentWarnings() const { return contentWarnings; }

// Setters
void Comment::setContent(const std::string& newContent) { 
    content = newContent; 
    updatedAt = std::chrono::system_clock::now();
}
void Comment::setStatus(CommentStatus newStatus) { status = newStatus; }
void Comment::setIsSensitive(bool sensitive) { isSensitive = sensitive; }

// Social interaction operations
bool Comment::likeComment(const std::string& userId) {
    if (likedBy.find(userId) != likedBy.end()) return false;
    likedBy.insert(userId);
    likeCount = likedBy.size();
    return true;
}

bool Comment::unlikeComment(const std::string& userId) {
    auto it = likedBy.find(userId);
    if (it == likedBy.end()) return false;
    likedBy.erase(it);
    likeCount = likedBy.size();
    return true;
}

void Comment::addReply(std::shared_ptr<Comment> reply) {
    replies.push_back(reply);
    replyCount = replies.size();
    reply->setParentComment(std::shared_ptr<Comment>(this, [](Comment*) {})); // Weak reference
}

void Comment::removeReply(const std::string& replyId) {
    replies.erase(std::remove_if(replies.begin(), replies.end(),
        [&replyId](const std::shared_ptr<Comment>& reply) {
            return reply->getCommentId() == replyId;
        }), replies.end());
    replyCount = replies.size();
}

// Parent comment operations
void Comment::setParentComment(std::shared_ptr<Comment> parent) {
    parentComment = parent;
}

// Content operations
void Comment::addMention(const std::string& username) {
    if (std::find(mentions.begin(), mentions.end(), username) == mentions.end()) {
        mentions.push_back(username);
    }
}

void Comment::removeMention(const std::string& username) {
    mentions.erase(std::remove(mentions.begin(), mentions.end(), username), mentions.end());
}

void Comment::addHashtag(const std::string& hashtag) {
    if (std::find(hashtags.begin(), hashtags.end(), hashtag) == hashtags.end()) {
        hashtags.push_back(hashtag);
    }
}

void Comment::removeHashtag(const std::string& hashtag) {
    hashtags.erase(std::remove(hashtags.begin(), hashtags.end(), hashtag), hashtags.end());
}

// Moderation operations
void Comment::addContentWarning(const std::string& warning) {
    if (std::find(contentWarnings.begin(), contentWarnings.end(), warning) == contentWarnings.end()) {
        contentWarnings.push_back(warning);
    }
}

void Comment::removeContentWarning(const std::string& warning) {
    contentWarnings.erase(std::remove(contentWarnings.begin(), contentWarnings.end(), warning), contentWarnings.end());
}

void Comment::reportComment() { reportCount++; }

void Comment::clearReports() { reportCount = 0; }

// Queries
bool Comment::isLikedBy(const std::string& userId) const {
    return likedBy.find(userId) != likedBy.end();
}

bool Comment::isReply() const {
    return parentComment != nullptr;
}

bool Comment::hasReplies() const {
    return !replies.empty();
}

bool Comment::containsMention(const std::string& username) const {
    return std::find(mentions.begin(), mentions.end(), username) != mentions.end();
}

bool Comment::containsHashtag(const std::string& hashtag) const {
    return std::find(hashtags.begin(), hashtags.end(), hashtag) != hashtags.end();
}

// Utility methods
std::string Comment::toString() const {
    std::stringstream ss;
    ss << "Comment{"
       << "commentId='" << commentId << "', "
       << "postId='" << postId << "', "
       << "authorId='" << authorId << "', "
       << "content='" << content.substr(0, 50) << "...', "
       << "likes=" << likeCount << ", "
       << "replies=" << replyCount << ", "
       << "isReply=" << (isReply() ? "true" : "false")
       << "}";
    return ss.str();
}

bool Comment::isValid() const {
    return !commentId.empty() && !postId.empty() && !authorId.empty() && !content.empty();
}

void Comment::updateStatistics() {
    likeCount = likedBy.size();
    replyCount = replies.size();
}

std::string Comment::getFormattedTimestamp() const {
    auto time_t = std::chrono::system_clock::to_time_t(createdAt);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

int Comment::getAgeInMinutes() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(now - createdAt);
    return duration.count();
}

int Comment::getAgeInHours() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::hours>(now - createdAt);
    return duration.count();
}

int Comment::getAgeInDays() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::hours>(now - createdAt);
    return duration.count() / 24;
}

int Comment::getDepth() const {
    int depth = 0;
    auto current = parentComment;
    while (current) {
        depth++;
        current = current->getParentComment();
    }
    return depth;
}
