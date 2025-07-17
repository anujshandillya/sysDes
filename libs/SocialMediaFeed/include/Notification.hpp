#ifndef NOTIFICATION_HPP
#define NOTIFICATION_HPP

#include <string>
#include <chrono>
#include <map>

enum class NotificationType {
    LIKE,
    RETWEET,
    COMMENT,
    REPLY,
    FOLLOW,
    MENTION,
    HASHTAG,
    SYSTEM,
    SECURITY,
    TRENDING
};

enum class NotificationStatus {
    UNREAD,
    READ,
    ARCHIVED,
    DELETED
};

enum class NotificationPriority {
    LOW,
    NORMAL,
    HIGH,
    URGENT
};

class Notification {
private:
    std::string notificationId;
    std::string userId;
    std::string senderId;
    NotificationType type;
    NotificationStatus status;
    NotificationPriority priority;
    
    std::string title;
    std::string message;
    std::string actionUrl;
    
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point readAt;
    
    // Additional data
    std::map<std::string, std::string> metadata;
    std::string relatedPostId;
    std::string relatedCommentId;
    std::string relatedUserId;
    
    // Delivery
    bool isDelivered;
    bool isPushed;
    bool isEmailed;
    std::chrono::system_clock::time_point deliveredAt;
    
public:
    Notification(const std::string& notificationId, const std::string& userId, const std::string& senderId, 
                 NotificationType type, const std::string& title, const std::string& message);
    ~Notification();
    
    // Getters
    std::string getNotificationId() const;
    std::string getUserId() const;
    std::string getSenderId() const;
    NotificationType getType() const;
    NotificationStatus getStatus() const;
    NotificationPriority getPriority() const;
    std::string getTitle() const;
    std::string getMessage() const;
    std::string getActionUrl() const;
    std::chrono::system_clock::time_point getCreatedAt() const;
    std::chrono::system_clock::time_point getReadAt() const;
    
    // Additional data getters
    std::map<std::string, std::string> getMetadata() const;
    std::string getMetadataValue(const std::string& key) const;
    std::string getRelatedPostId() const;
    std::string getRelatedCommentId() const;
    std::string getRelatedUserId() const;
    
    // Delivery getters
    bool getIsDelivered() const;
    bool getIsPushed() const;
    bool getIsEmailed() const;
    std::chrono::system_clock::time_point getDeliveredAt() const;
    
    // Setters
    void setStatus(NotificationStatus status);
    void setPriority(NotificationPriority priority);
    void setTitle(const std::string& title);
    void setMessage(const std::string& message);
    void setActionUrl(const std::string& url);
    
    // Additional data setters
    void setMetadata(const std::string& key, const std::string& value);
    void setRelatedPostId(const std::string& postId);
    void setRelatedCommentId(const std::string& commentId);
    void setRelatedUserId(const std::string& userId);
    
    // Status operations
    void markAsRead();
    void markAsUnread();
    void archive();
    void deleteNotification();
    
    // Delivery operations
    void markAsDelivered();
    void markAsPushed();
    void markAsEmailed();
    
    // Queries
    bool isRead() const;
    bool isUnread() const;
    bool isArchived() const;
    bool isDeleted() const;
    bool hasMetadata(const std::string& key) const;
    
    // Utility methods
    std::string toString() const;
    bool isValid() const;
    std::string getFormattedTimestamp() const;
    int getAgeInMinutes() const;
    int getAgeInHours() const;
    int getAgeInDays() const;
    std::string getTypeString() const;
    std::string getStatusString() const;
    std::string getPriorityString() const;
    
    // Factory methods
    static std::shared_ptr<Notification> createLikeNotification(const std::string& userId, const std::string& senderId, 
                                                               const std::string& postId);
    static std::shared_ptr<Notification> createRetweetNotification(const std::string& userId, const std::string& senderId, 
                                                                  const std::string& postId);
    static std::shared_ptr<Notification> createCommentNotification(const std::string& userId, const std::string& senderId, 
                                                                  const std::string& postId, const std::string& commentId);
    static std::shared_ptr<Notification> createFollowNotification(const std::string& userId, const std::string& senderId);
    static std::shared_ptr<Notification> createMentionNotification(const std::string& userId, const std::string& senderId, 
                                                                  const std::string& postId);
    static std::shared_ptr<Notification> createSystemNotification(const std::string& userId, const std::string& title, 
                                                                 const std::string& message);
    static std::shared_ptr<Notification> createSecurityNotification(const std::string& userId, const std::string& title, 
                                                                   const std::string& message);
};

#endif // NOTIFICATION_HPP 