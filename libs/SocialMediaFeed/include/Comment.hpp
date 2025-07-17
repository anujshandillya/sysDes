#ifndef COMMENT_HPP
#define COMMENT_HPP

#include <string>
#include <vector>
#include <set>
#include <memory>
#include <chrono>

class User;
class Post;

enum class CommentStatus {
    ACTIVE,
    DELETED,
    HIDDEN,
    REPORTED,
    UNDER_REVIEW
};

class Comment {
private:
    std::string commentId;
    std::string postId;
    std::string authorId;
    std::string content;
    CommentStatus status;
    
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point updatedAt;
    
    // Engagement metrics
    int likeCount;
    int replyCount;
    int reportCount;
    
    // Social interactions
    std::set<std::string> likedBy;
    std::vector<std::shared_ptr<Comment>> replies;
    
    // Parent comment for replies
    std::shared_ptr<Comment> parentComment;
    
    // Mentions and hashtags
    std::vector<std::string> mentions;
    std::vector<std::string> hashtags;
    
    // Moderation
    bool isSensitive;
    std::vector<std::string> contentWarnings;
    
public:
    Comment(const std::string& commentId, const std::string& postId, const std::string& authorId, const std::string& content);
    ~Comment();
    
    // Getters
    std::string getCommentId() const;
    std::string getPostId() const;
    std::string getAuthorId() const;
    std::string getContent() const;
    CommentStatus getStatus() const;
    std::chrono::system_clock::time_point getCreatedAt() const;
    std::chrono::system_clock::time_point getUpdatedAt() const;
    
    // Engagement metrics
    int getLikeCount() const;
    int getReplyCount() const;
    int getReportCount() const;
    
    // Social interaction getters
    std::set<std::string> getLikedBy() const;
    std::vector<std::shared_ptr<Comment>> getReplies() const;
    
    // Parent comment getters
    std::shared_ptr<Comment> getParentComment() const;
    
    // Content getters
    std::vector<std::string> getMentions() const;
    std::vector<std::string> getHashtags() const;
    
    // Moderation getters
    bool getIsSensitive() const;
    std::vector<std::string> getContentWarnings() const;
    
    // Setters
    void setContent(const std::string& content);
    void setStatus(CommentStatus status);
    void setIsSensitive(bool isSensitive);
    
    // Social interaction operations
    bool likeComment(const std::string& userId);
    bool unlikeComment(const std::string& userId);
    void addReply(std::shared_ptr<Comment> reply);
    void removeReply(const std::string& replyId);
    
    // Parent comment operations
    void setParentComment(std::shared_ptr<Comment> parent);
    
    // Content operations
    void addMention(const std::string& username);
    void removeMention(const std::string& username);
    void addHashtag(const std::string& hashtag);
    void removeHashtag(const std::string& hashtag);
    
    // Moderation operations
    void addContentWarning(const std::string& warning);
    void removeContentWarning(const std::string& warning);
    void reportComment();
    void clearReports();
    
    // Queries
    bool isLikedBy(const std::string& userId) const;
    bool isReply() const;
    bool hasReplies() const;
    bool containsMention(const std::string& username) const;
    bool containsHashtag(const std::string& hashtag) const;
    
    // Utility methods
    std::string toString() const;
    bool isValid() const;
    void updateStatistics();
    std::string getFormattedTimestamp() const;
    int getAgeInMinutes() const;
    int getAgeInHours() const;
    int getAgeInDays() const;
    int getDepth() const;
};

#endif // COMMENT_HPP 