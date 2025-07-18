#include "../include/Notification.hpp"
#include <sstream>
#include <iomanip>

Notification::Notification(const std::string& notificationId, const std::string& userId, const std::string& senderId, 
                         NotificationType type, const std::string& title, const std::string& message)
    : notificationId(notificationId), userId(userId), senderId(senderId), type(type), 
      status(NotificationStatus::UNREAD), priority(NotificationPriority::NORMAL),
      title(title), message(message), isDelivered(false), isPushed(false), isEmailed(false) {
    createdAt = std::chrono::system_clock::now();
}

Notification::~Notification() = default;

// Getters
std::string Notification::getNotificationId() const { return notificationId; }
std::string Notification::getUserId() const { return userId; }
std::string Notification::getSenderId() const { return senderId; }
NotificationType Notification::getType() const { return type; }
NotificationStatus Notification::getStatus() const { return status; }
NotificationPriority Notification::getPriority() const { return priority; }
std::string Notification::getTitle() const { return title; }
std::string Notification::getMessage() const { return message; }
std::string Notification::getActionUrl() const { return actionUrl; }
std::chrono::system_clock::time_point Notification::getCreatedAt() const { return createdAt; }
std::chrono::system_clock::time_point Notification::getReadAt() const { return readAt; }

// Additional data getters
std::map<std::string, std::string> Notification::getMetadata() const { return metadata; }

std::string Notification::getMetadataValue(const std::string& key) const {
    auto it = metadata.find(key);
    return it != metadata.end() ? it->second : "";
}

std::string Notification::getRelatedPostId() const { return relatedPostId; }
std::string Notification::getRelatedCommentId() const { return relatedCommentId; }
std::string Notification::getRelatedUserId() const { return relatedUserId; }

// Delivery getters
bool Notification::getIsDelivered() const { return isDelivered; }
bool Notification::getIsPushed() const { return isPushed; }
bool Notification::getIsEmailed() const { return isEmailed; }
std::chrono::system_clock::time_point Notification::getDeliveredAt() const { return deliveredAt; }

// Setters
void Notification::setStatus(NotificationStatus newStatus) { status = newStatus; }
void Notification::setPriority(NotificationPriority newPriority) { priority = newPriority; }
void Notification::setTitle(const std::string& newTitle) { title = newTitle; }
void Notification::setMessage(const std::string& newMessage) { message = newMessage; }
void Notification::setActionUrl(const std::string& url) { actionUrl = url; }

// Additional data setters
void Notification::setMetadata(const std::string& key, const std::string& value) {
    metadata[key] = value;
}

void Notification::setRelatedPostId(const std::string& postId) { relatedPostId = postId; }
void Notification::setRelatedCommentId(const std::string& commentId) { relatedCommentId = commentId; }
void Notification::setRelatedUserId(const std::string& userId) { relatedUserId = userId; }

// Status operations
void Notification::markAsRead() {
    status = NotificationStatus::READ;
    readAt = std::chrono::system_clock::now();
}

void Notification::markAsUnread() {
    status = NotificationStatus::UNREAD;
    readAt = std::chrono::system_clock::time_point();
}

void Notification::archive() {
    status = NotificationStatus::ARCHIVED;
}

void Notification::deleteNotification() {
    status = NotificationStatus::DELETED;
}

// Delivery operations
void Notification::markAsDelivered() {
    isDelivered = true;
    deliveredAt = std::chrono::system_clock::now();
}

void Notification::markAsPushed() {
    isPushed = true;
}

void Notification::markAsEmailed() {
    isEmailed = true;
}

// Queries
bool Notification::isRead() const {
    return status == NotificationStatus::READ;
}

bool Notification::isUnread() const {
    return status == NotificationStatus::UNREAD;
}

bool Notification::isArchived() const {
    return status == NotificationStatus::ARCHIVED;
}

bool Notification::isDeleted() const {
    return status == NotificationStatus::DELETED;
}

bool Notification::hasMetadata(const std::string& key) const {
    return metadata.find(key) != metadata.end();
}

// Utility methods
std::string Notification::toString() const {
    std::stringstream ss;
    ss << "Notification{"
       << "notificationId='" << notificationId << "', "
       << "userId='" << userId << "', "
       << "senderId='" << senderId << "', "
       << "type=" << static_cast<int>(type) << ", "
       << "status=" << static_cast<int>(status) << ", "
       << "priority=" << static_cast<int>(priority) << ", "
       << "title='" << title << "', "
       << "isRead=" << (isRead() ? "true" : "false")
       << "}";
    return ss.str();
}

bool Notification::isValid() const {
    return !notificationId.empty() && !userId.empty() && !title.empty() && !message.empty();
}

std::string Notification::getFormattedTimestamp() const {
    auto time_t = std::chrono::system_clock::to_time_t(createdAt);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

int Notification::getAgeInMinutes() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(now - createdAt);
    return duration.count();
}

int Notification::getAgeInHours() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::hours>(now - createdAt);
    return duration.count();
}

int Notification::getAgeInDays() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::hours>(now - createdAt);
    return duration.count() / 24;
}

std::string Notification::getTypeString() const {
    switch (type) {
        case NotificationType::LIKE: return "LIKE";
        case NotificationType::RETWEET: return "RETWEET";
        case NotificationType::COMMENT: return "COMMENT";
        case NotificationType::REPLY: return "REPLY";
        case NotificationType::FOLLOW: return "FOLLOW";
        case NotificationType::MENTION: return "MENTION";
        case NotificationType::HASHTAG: return "HASHTAG";
        case NotificationType::SYSTEM: return "SYSTEM";
        case NotificationType::SECURITY: return "SECURITY";
        case NotificationType::TRENDING: return "TRENDING";
        default: return "UNKNOWN";
    }
}

std::string Notification::getStatusString() const {
    switch (status) {
        case NotificationStatus::UNREAD: return "UNREAD";
        case NotificationStatus::READ: return "READ";
        case NotificationStatus::ARCHIVED: return "ARCHIVED";
        case NotificationStatus::DELETED: return "DELETED";
        default: return "UNKNOWN";
    }
}

std::string Notification::getPriorityString() const {
    switch (priority) {
        case NotificationPriority::LOW: return "LOW";
        case NotificationPriority::NORMAL: return "NORMAL";
        case NotificationPriority::HIGH: return "HIGH";
        case NotificationPriority::URGENT: return "URGENT";
        default: return "UNKNOWN";
    }
}

// Factory methods
std::shared_ptr<Notification> Notification::createLikeNotification(const std::string& userId, const std::string& senderId, 
                                                                  const std::string& postId) {
    auto notification = std::make_shared<Notification>(
        "notif_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()),
        userId, senderId, NotificationType::LIKE, "New Like", "Someone liked your post"
    );
    notification->setRelatedPostId(postId);
    notification->setPriority(NotificationPriority::LOW);
    return notification;
}

std::shared_ptr<Notification> Notification::createRetweetNotification(const std::string& userId, const std::string& senderId, 
                                                                     const std::string& postId) {
    auto notification = std::make_shared<Notification>(
        "notif_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()),
        userId, senderId, NotificationType::RETWEET, "New Retweet", "Someone retweeted your post"
    );
    notification->setRelatedPostId(postId);
    notification->setPriority(NotificationPriority::NORMAL);
    return notification;
}

std::shared_ptr<Notification> Notification::createCommentNotification(const std::string& userId, const std::string& senderId, 
                                                                     const std::string& postId, const std::string& commentId) {
    auto notification = std::make_shared<Notification>(
        "notif_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()),
        userId, senderId, NotificationType::COMMENT, "New Comment", "Someone commented on your post"
    );
    notification->setRelatedPostId(postId);
    notification->setRelatedCommentId(commentId);
    notification->setPriority(NotificationPriority::NORMAL);
    return notification;
}

std::shared_ptr<Notification> Notification::createFollowNotification(const std::string& userId, const std::string& senderId) {
    auto notification = std::make_shared<Notification>(
        "notif_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()),
        userId, senderId, NotificationType::FOLLOW, "New Follower", "Someone started following you"
    );
    notification->setRelatedUserId(senderId);
    notification->setPriority(NotificationPriority::NORMAL);
    return notification;
}

std::shared_ptr<Notification> Notification::createMentionNotification(const std::string& userId, const std::string& senderId, 
                                                                     const std::string& postId) {
    auto notification = std::make_shared<Notification>(
        "notif_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()),
        userId, senderId, NotificationType::MENTION, "Mention", "Someone mentioned you in a post"
    );
    notification->setRelatedPostId(postId);
    notification->setPriority(NotificationPriority::HIGH);
    return notification;
}

std::shared_ptr<Notification> Notification::createSystemNotification(const std::string& userId, const std::string& title, 
                                                                    const std::string& message) {
    auto notification = std::make_shared<Notification>(
        "notif_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()),
        userId, "system", NotificationType::SYSTEM, title, message
    );
    notification->setPriority(NotificationPriority::LOW);
    return notification;
}

std::shared_ptr<Notification> Notification::createSecurityNotification(const std::string& userId, const std::string& title, 
                                                                      const std::string& message) {
    auto notification = std::make_shared<Notification>(
        "notif_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()),
        userId, "system", NotificationType::SECURITY, title, message
    );
    notification->setPriority(NotificationPriority::URGENT);
    return notification;
}
