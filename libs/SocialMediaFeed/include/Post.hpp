#ifndef POST_HPP
#define POST_HPP

#include <string>
#include <vector>
#include <set>
#include <memory>
#include <chrono>
#include <map>

class User;
class Comment;
class Hashtag;
class Media;

enum class PostType {
    TEXT,
    IMAGE,
    VIDEO,
    POLL,
    RETWEET,
    QUOTE_TWEET
};

enum class PostStatus {
    ACTIVE,
    DELETED,
    HIDDEN,
    REPORTED,
    UNDER_REVIEW
};

class Post {
private:
    std::string postId;
    std::string authorId;
    std::string content;
    PostType type;
    PostStatus status;
    
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point updatedAt;
    
    // Engagement metrics
    int likeCount;
    int retweetCount;
    int commentCount;
    int bookmarkCount;
    int viewCount;
    int shareCount;
    
    // Content
    std::vector<std::shared_ptr<Media>> media;
    std::vector<std::shared_ptr<Hashtag>> hashtags;
    std::vector<std::string> mentions;
    std::string location;
    std::string language;
    
    // Social interactions
    std::set<std::string> likedBy;
    std::set<std::string> retweetedBy;
    std::set<std::string> bookmarkedBy;
    std::vector<std::shared_ptr<Comment>> comments;
    
    // Retweet/Quote information
    std::shared_ptr<Post> originalPost;
    std::string quoteText;
    
    // Poll information
    std::map<std::string, int> pollOptions;
    std::set<std::string> pollVoters;
    std::chrono::system_clock::time_point pollEndTime;
    
    // Visibility and privacy
    bool isPublic;
    std::set<std::string> visibleTo;
    std::set<std::string> hiddenFrom;
    
    // Moderation
    bool isSensitive;
    std::vector<std::string> contentWarnings;
    int reportCount;
    
public:
    Post(const std::string& postId, const std::string& authorId, const std::string& content, PostType type = PostType::TEXT);
    ~Post();
    
    // Getters
    std::string getPostId() const;
    std::string getAuthorId() const;
    std::string getContent() const;
    PostType getType() const;
    PostStatus getStatus() const;
    std::chrono::system_clock::time_point getCreatedAt() const;
    std::chrono::system_clock::time_point getUpdatedAt() const;
    
    // Engagement metrics
    int getLikeCount() const;
    int getRetweetCount() const;
    int getCommentCount() const;
    int getBookmarkCount() const;
    int getViewCount() const;
    int getShareCount() const;
    
    // Content getters
    std::vector<std::shared_ptr<Media>> getMedia() const;
    std::vector<std::shared_ptr<Hashtag>> getHashtags() const;
    std::vector<std::string> getMentions() const;
    std::string getLocation() const;
    std::string getLanguage() const;
    
    // Social interaction getters
    std::set<std::string> getLikedBy() const;
    std::set<std::string> getRetweetedBy() const;
    std::set<std::string> getBookmarkedBy() const;
    std::vector<std::shared_ptr<Comment>> getComments() const;
    
    // Retweet/Quote getters
    std::shared_ptr<Post> getOriginalPost() const;
    std::string getQuoteText() const;
    
    // Poll getters
    std::map<std::string, int> getPollOptions() const;
    std::set<std::string> getPollVoters() const;
    std::chrono::system_clock::time_point getPollEndTime() const;
    
    // Visibility getters
    bool getIsPublic() const;
    std::set<std::string> getVisibleTo() const;
    std::set<std::string> getHiddenFrom() const;
    
    // Moderation getters
    bool getIsSensitive() const;
    std::vector<std::string> getContentWarnings() const;
    int getReportCount() const;
    
    // Setters
    void setContent(const std::string& content);
    void setStatus(PostStatus status);
    void setLocation(const std::string& location);
    void setLanguage(const std::string& language);
    void setIsPublic(bool isPublic);
    void setIsSensitive(bool isSensitive);
    
    // Content operations
    void addMedia(std::shared_ptr<Media> media);
    void removeMedia(const std::string& mediaId);
    void addHashtag(std::shared_ptr<Hashtag> hashtag);
    void removeHashtag(const std::string& hashtagText);
    void addMention(const std::string& username);
    void removeMention(const std::string& username);
    
    // Social interaction operations
    bool likePost(const std::string& userId);
    bool unlikePost(const std::string& userId);
    bool retweetPost(const std::string& userId);
    bool unretweetPost(const std::string& userId);
    bool bookmarkPost(const std::string& userId);
    bool unbookmarkPost(const std::string& userId);
    void addComment(std::shared_ptr<Comment> comment);
    void removeComment(const std::string& commentId);
    void incrementViewCount();
    void incrementShareCount();
    
    // Retweet/Quote operations
    void setOriginalPost(std::shared_ptr<Post> originalPost);
    void setQuoteText(const std::string& quoteText);
    
    // Poll operations
    void addPollOption(const std::string& option);
    bool voteInPoll(const std::string& userId, const std::string& option);
    void setPollEndTime(const std::chrono::system_clock::time_point& endTime);
    bool isPollActive() const;
    std::string getWinningPollOption() const;
    
    // Visibility operations
    void addVisibleTo(const std::string& userId);
    void removeVisibleTo(const std::string& userId);
    void addHiddenFrom(const std::string& userId);
    void removeHiddenFrom(const std::string& userId);
    
    // Moderation operations
    void addContentWarning(const std::string& warning);
    void removeContentWarning(const std::string& warning);
    void reportPost();
    void clearReports();
    
    // Queries
    bool isLikedBy(const std::string& userId) const;
    bool isRetweetedBy(const std::string& userId) const;
    bool isBookmarkedBy(const std::string& userId) const;
    bool isVisibleTo(const std::string& userId) const;
    bool isHiddenFrom(const std::string& userId) const;
    bool hasVotedInPoll(const std::string& userId) const;
    bool containsHashtag(const std::string& hashtagText) const;
    bool containsMention(const std::string& username) const;
    
    // Utility methods
    std::string toString() const;
    bool isValid() const;
    void updateStatistics();
    std::string getFormattedTimestamp() const;
    int getAgeInMinutes() const;
    int getAgeInHours() const;
    int getAgeInDays() const;
};

#endif // POST_HPP 