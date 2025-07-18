#include "../include/Media.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>

Media::Media(const std::string& mediaId, const std::string& url, MediaType type)
    : mediaId(mediaId), url(url), type(type), status(MediaStatus::UPLOADING),
      fileSize(0), width(0), height(0), duration(0), hasAudio(false), isAnimated(false),
      processingProgress(0) {
    uploadedAt = std::chrono::system_clock::now();
}

Media::~Media() = default;

// Getters
std::string Media::getMediaId() const { return mediaId; }
std::string Media::getUrl() const { return url; }
std::string Media::getThumbnailUrl() const { return thumbnailUrl; }
std::string Media::getAltText() const { return altText; }
MediaType Media::getType() const { return type; }
MediaStatus Media::getStatus() const { return status; }
std::string Media::getFilename() const { return filename; }
std::string Media::getMimeType() const { return mimeType; }
long Media::getFileSize() const { return fileSize; }
int Media::getWidth() const { return width; }
int Media::getHeight() const { return height; }
int Media::getDuration() const { return duration; }
std::chrono::system_clock::time_point Media::getUploadedAt() const { return uploadedAt; }
std::chrono::system_clock::time_point Media::getProcessedAt() const { return processedAt; }

// Video getters
std::string Media::getVideoUrl() const { return videoUrl; }
std::string Media::getPreviewUrl() const { return previewUrl; }
bool Media::getHasAudio() const { return hasAudio; }
std::string Media::getVideoCodec() const { return videoCodec; }
std::string Media::getAudioCodec() const { return audioCodec; }

// Image getters
std::vector<std::string> Media::getImageVariants() const { return imageVariants; }
bool Media::getIsAnimated() const { return isAnimated; }

// Processing getters
std::string Media::getProcessingError() const { return processingError; }
int Media::getProcessingProgress() const { return processingProgress; }

// Setters
void Media::setUrl(const std::string& newUrl) { url = newUrl; }
void Media::setThumbnailUrl(const std::string& newUrl) { thumbnailUrl = newUrl; }
void Media::setAltText(const std::string& text) { altText = text; }
void Media::setStatus(MediaStatus newStatus) { status = newStatus; }
void Media::setFilename(const std::string& name) { filename = name; }
void Media::setMimeType(const std::string& mime) { mimeType = mime; }
void Media::setFileSize(long size) { fileSize = size; }
void Media::setDimensions(int w, int h) { width = w; height = h; }
void Media::setDuration(int dur) { duration = dur; }

// Video setters
void Media::setVideoUrl(const std::string& newUrl) { videoUrl = newUrl; }
void Media::setPreviewUrl(const std::string& newUrl) { previewUrl = newUrl; }
void Media::setHasAudio(bool audio) { hasAudio = audio; }
void Media::setVideoCodec(const std::string& codec) { videoCodec = codec; }
void Media::setAudioCodec(const std::string& codec) { audioCodec = codec; }

// Image setters
void Media::addImageVariant(const std::string& variantUrl) {
    if (std::find(imageVariants.begin(), imageVariants.end(), variantUrl) == imageVariants.end()) {
        imageVariants.push_back(variantUrl);
    }
}
void Media::setIsAnimated(bool animated) { isAnimated = animated; }

// Processing setters
void Media::setProcessingError(const std::string& error) { processingError = error; }
void Media::setProcessingProgress(int progress) { processingProgress = progress; }

// Status operations
void Media::markAsUploading() { status = MediaStatus::UPLOADING; }
void Media::markAsProcessing() { status = MediaStatus::PROCESSING; }
void Media::markAsReady() { 
    status = MediaStatus::READY; 
    processedAt = std::chrono::system_clock::now();
}
void Media::markAsFailed(const std::string& error) { 
    status = MediaStatus::FAILED; 
    processingError = error;
}
void Media::markAsDeleted() { status = MediaStatus::DELETED; }

// Processing operations
void Media::startProcessing() {
    status = MediaStatus::PROCESSING;
    processingProgress = 0;
}

void Media::updateProgress(int progress) {
    processingProgress = std::min(100, std::max(0, progress));
}

void Media::completeProcessing() {
    status = MediaStatus::READY;
    processingProgress = 100;
    processedAt = std::chrono::system_clock::now();
}

void Media::failProcessing(const std::string& error) {
    status = MediaStatus::FAILED;
    processingError = error;
    processingProgress = 0;
}

// Utility operations
void Media::generateThumbnail() {
    // This would generate a thumbnail for the media
    // Implementation would depend on the media processing library used
    if (type == MediaType::IMAGE || type == MediaType::VIDEO) {
        thumbnailUrl = url + "_thumb.jpg";
    }
}

void Media::optimizeForWeb() {
    // This would optimize the media for web delivery
    // Implementation would depend on the optimization library used
}

void Media::addAltText(const std::string& text) {
    altText = text;
}

// Queries
bool Media::isImage() const { return type == MediaType::IMAGE; }
bool Media::isVideo() const { return type == MediaType::VIDEO; }
bool Media::isGif() const { return type == MediaType::GIF; }
bool Media::isAudio() const { return type == MediaType::AUDIO; }
bool Media::isDocument() const { return type == MediaType::DOCUMENT; }
bool Media::isReady() const { return status == MediaStatus::READY; }
bool Media::isProcessing() const { return status == MediaStatus::PROCESSING; }
bool Media::isFailed() const { return status == MediaStatus::FAILED; }
bool Media::isUploading() const { return status == MediaStatus::UPLOADING; }
bool Media::isDeleted() const { return status == MediaStatus::DELETED; }
bool Media::hasThumbnail() const { return !thumbnailUrl.empty(); }

// Utility methods
std::string Media::toString() const {
    std::stringstream ss;
    ss << "Media{"
       << "mediaId='" << mediaId << "', "
       << "type=" << static_cast<int>(type) << ", "
       << "status=" << static_cast<int>(status) << ", "
       << "url='" << url << "', "
       << "size=" << getFormattedFileSize() << ", "
       << "dimensions=" << width << "x" << height
       << "}";
    return ss.str();
}

bool Media::isValid() const {
    return !mediaId.empty() && !url.empty();
}

std::string Media::getFormattedFileSize() const {
    if (fileSize == 0) return "0 B";
    
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;
    double size = static_cast<double>(fileSize);
    
    while (size >= 1024.0 && unitIndex < 4) {
        size /= 1024.0;
        unitIndex++;
    }
    
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << size << " " << units[unitIndex];
    return ss.str();
}

std::string Media::getFormattedDuration() const {
    if (duration <= 0) return "0:00";
    
    int hours = duration / 3600;
    int minutes = (duration % 3600) / 60;
    int seconds = duration % 60;
    
    std::stringstream ss;
    if (hours > 0) {
        ss << hours << ":" << std::setfill('0') << std::setw(2) << minutes << ":" 
           << std::setfill('0') << std::setw(2) << seconds;
    } else {
        ss << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
    }
    return ss.str();
}

std::string Media::getTypeString() const {
    switch (type) {
        case MediaType::IMAGE: return "IMAGE";
        case MediaType::VIDEO: return "VIDEO";
        case MediaType::GIF: return "GIF";
        case MediaType::AUDIO: return "AUDIO";
        case MediaType::DOCUMENT: return "DOCUMENT";
        default: return "UNKNOWN";
    }
}

std::string Media::getStatusString() const {
    switch (status) {
        case MediaStatus::UPLOADING: return "UPLOADING";
        case MediaStatus::PROCESSING: return "PROCESSING";
        case MediaStatus::READY: return "READY";
        case MediaStatus::FAILED: return "FAILED";
        case MediaStatus::DELETED: return "DELETED";
        default: return "UNKNOWN";
    }
}

double Media::getAspectRatio() const {
    if (height == 0) return 0.0;
    return static_cast<double>(width) / height;
}

bool Media::isPortrait() const {
    return height > width;
}

bool Media::isLandscape() const {
    return width > height;
}

bool Media::isSquare() const {
    return width == height;
}

// Factory methods
std::shared_ptr<Media> Media::createImage(const std::string& mediaId, const std::string& url) {
    auto media = std::make_shared<Media>(mediaId, url, MediaType::IMAGE);
    media->setMimeType("image/jpeg");
    return media;
}

std::shared_ptr<Media> Media::createVideo(const std::string& mediaId, const std::string& url) {
    auto media = std::make_shared<Media>(mediaId, url, MediaType::VIDEO);
    media->setMimeType("video/mp4");
    return media;
}

std::shared_ptr<Media> Media::createGif(const std::string& mediaId, const std::string& url) {
    auto media = std::make_shared<Media>(mediaId, url, MediaType::GIF);
    media->setMimeType("image/gif");
    media->setIsAnimated(true);
    return media;
}

std::shared_ptr<Media> Media::createAudio(const std::string& mediaId, const std::string& url) {
    auto media = std::make_shared<Media>(mediaId, url, MediaType::AUDIO);
    media->setMimeType("audio/mpeg");
    return media;
}

std::shared_ptr<Media> Media::createDocument(const std::string& mediaId, const std::string& url) {
    auto media = std::make_shared<Media>(mediaId, url, MediaType::DOCUMENT);
    media->setMimeType("application/pdf");
    return media;
}
