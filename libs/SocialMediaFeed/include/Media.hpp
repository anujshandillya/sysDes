#ifndef MEDIA_HPP
#define MEDIA_HPP

#include <string>
#include <vector>
#include <chrono>

enum class MediaType {
    IMAGE,
    VIDEO,
    GIF,
    AUDIO,
    DOCUMENT
};

enum class MediaStatus {
    UPLOADING,
    PROCESSING,
    READY,
    FAILED,
    DELETED
};

class Media {
private:
    std::string mediaId;
    std::string url;
    std::string thumbnailUrl;
    std::string altText;
    MediaType type;
    MediaStatus status;
    
    std::string filename;
    std::string mimeType;
    long fileSize;
    int width;
    int height;
    int duration; // in seconds
    
    std::chrono::system_clock::time_point uploadedAt;
    std::chrono::system_clock::time_point processedAt;
    
    // Video specific
    std::string videoUrl;
    std::string previewUrl;
    bool hasAudio;
    std::string videoCodec;
    std::string audioCodec;
    
    // Image specific
    std::vector<std::string> imageVariants; // different sizes
    bool isAnimated;
    
    // Processing
    std::string processingError;
    int processingProgress; // 0-100
    
public:
    Media(const std::string& mediaId, const std::string& url, MediaType type);
    ~Media();
    
    // Getters
    std::string getMediaId() const;
    std::string getUrl() const;
    std::string getThumbnailUrl() const;
    std::string getAltText() const;
    MediaType getType() const;
    MediaStatus getStatus() const;
    std::string getFilename() const;
    std::string getMimeType() const;
    long getFileSize() const;
    int getWidth() const;
    int getHeight() const;
    int getDuration() const;
    std::chrono::system_clock::time_point getUploadedAt() const;
    std::chrono::system_clock::time_point getProcessedAt() const;
    
    // Video getters
    std::string getVideoUrl() const;
    std::string getPreviewUrl() const;
    bool getHasAudio() const;
    std::string getVideoCodec() const;
    std::string getAudioCodec() const;
    
    // Image getters
    std::vector<std::string> getImageVariants() const;
    bool getIsAnimated() const;
    
    // Processing getters
    std::string getProcessingError() const;
    int getProcessingProgress() const;
    
    // Setters
    void setUrl(const std::string& url);
    void setThumbnailUrl(const std::string& url);
    void setAltText(const std::string& altText);
    void setStatus(MediaStatus status);
    void setFilename(const std::string& filename);
    void setMimeType(const std::string& mimeType);
    void setFileSize(long size);
    void setDimensions(int width, int height);
    void setDuration(int duration);
    
    // Video setters
    void setVideoUrl(const std::string& url);
    void setPreviewUrl(const std::string& url);
    void setHasAudio(bool hasAudio);
    void setVideoCodec(const std::string& codec);
    void setAudioCodec(const std::string& codec);
    
    // Image setters
    void addImageVariant(const std::string& url);
    void setIsAnimated(bool animated);
    
    // Processing setters
    void setProcessingError(const std::string& error);
    void setProcessingProgress(int progress);
    
    // Status operations
    void markAsUploading();
    void markAsProcessing();
    void markAsReady();
    void markAsFailed(const std::string& error);
    void markAsDeleted();
    
    // Processing operations
    void startProcessing();
    void updateProgress(int progress);
    void completeProcessing();
    void failProcessing(const std::string& error);
    
    // Utility operations
    void generateThumbnail();
    void optimizeForWeb();
    void addAltText(const std::string& altText);
    
    // Queries
    bool isImage() const;
    bool isVideo() const;
    bool isGif() const;
    bool isAudio() const;
    bool isDocument() const;
    bool isReady() const;
    bool isProcessing() const;
    bool isFailed() const;
    bool isUploading() const;
    bool isDeleted() const;
    bool hasThumbnail() const;
    
    // Utility methods
    std::string toString() const;
    bool isValid() const;
    std::string getFormattedFileSize() const;
    std::string getFormattedDuration() const;
    std::string getTypeString() const;
    std::string getStatusString() const;
    double getAspectRatio() const;
    bool isPortrait() const;
    bool isLandscape() const;
    bool isSquare() const;
    
    // Factory methods
    static std::shared_ptr<Media> createImage(const std::string& mediaId, const std::string& url);
    static std::shared_ptr<Media> createVideo(const std::string& mediaId, const std::string& url);
    static std::shared_ptr<Media> createGif(const std::string& mediaId, const std::string& url);
    static std::shared_ptr<Media> createAudio(const std::string& mediaId, const std::string& url);
    static std::shared_ptr<Media> createDocument(const std::string& mediaId, const std::string& url);
};

#endif // MEDIA_HPP 